#include "shuffle.h"

static wchar_t *ascii_pic = NULL;
static struct timespec sleep_time = {0, 0};

void cleanup_resources(void)
{
    free(ascii_pic);
    ascii_pic = NULL;
    wprintf(L"\033[?25h"); // Show cursor
    wprintf(L"\033[0m");   // Reset text mode
}

void shuffle_array(int *array, size_t n)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    srand((unsigned int)ts.tv_nsec); // Better randomization using nanoseconds

    for (size_t i = n - 1; i > 0; i--)
    {
        size_t j = rand() % (i + 1);
        swap_ints(&array[i], &array[j]);
    }
}

Color parse_color(const char *color_str)
{
    Color color = {0, 0, 0, false};

    if (strcmp(color_str, "random") == 0)
    {
        color.is_random = true;
        return color;
    }

    // Standard colors
    static const struct
    {
        const char *name;
        Color color;
    } standard_colors[] = {
        {"red", {255, 0, 0, false}},
        {"green", {0, 255, 0, false}},
        {"yellow", {255, 255, 0, false}},
        {"blue", {0, 0, 255, false}},
        {"magenta", {255, 0, 255, false}},
        {"cyan", {0, 255, 255, false}},
        {"white", {255, 255, 255, false}},
        {"black", {0, 0, 0, false}},
        {"orange", {255, 165, 0, false}},
        {"grey", {127, 127, 127, false}},
    };

    // Check standard colors
    for (size_t i = 0; i < sizeof(standard_colors) / sizeof(standard_colors[0]); i++)
    {
        if (strcmp(color_str, standard_colors[i].name) == 0)
        {
            return standard_colors[i].color;
        }
    }

    // Parse RGB format
    int r, g, b;
    if (sscanf(color_str, "%d;%d;%d", &r, &g, &b) == 3)
    {
        if (r >= 0 && r <= MAX_COLOR_VALUE &&
            g >= 0 && g <= MAX_COLOR_VALUE &&
            b >= 0 && b <= MAX_COLOR_VALUE)
        {
            color.r = r;
            color.g = g;
            color.b = b;
        }
    }

    return color;
}

bool is_valid_color(const char *color)
{
    Color c = parse_color(color);
    return c.is_random || (c.r >= 0 && c.g >= 0 && c.b >= 0);
}

int load_ascii(const char *filename, wchar_t **output)
{
    if (!filename || !output)
    {
        return ERROR_INPUT;
    }

    FILE *input_stream = strcmp(filename, "-") == 0 ? stdin : fopen(filename, "r");
    if (!input_stream)
    {
        wprintf(L"Cannot open input file: %s\n", strerror(errno));
        return ERROR_FILE;
    }

    size_t buffer_size = INITIAL_BUFFER_SIZE;
    size_t read_chars = 0;
    wchar_t *buffer = calloc(buffer_size, sizeof(wchar_t));

    if (!buffer)
    {
        if (input_stream != stdin)
            fclose(input_stream);
        return ERROR_MEMORY;
    }

    wint_t wc;
    while ((wc = fgetwc(input_stream)) != WEOF)
    {
        if (read_chars >= buffer_size - 1)
        {
            buffer_size *= 2;
            wchar_t *new_buffer = realloc(buffer, buffer_size * sizeof(wchar_t));
            if (!new_buffer)
            {
                free(buffer);
                if (input_stream != stdin)
                    fclose(input_stream);
                return ERROR_MEMORY;
            }
            buffer = new_buffer;
        }
        buffer[read_chars++] = (wchar_t)wc;
    }

    if (ferror(input_stream))
    {
        free(buffer);
        if (input_stream != stdin)
            fclose(input_stream);
        return ERROR_FILE;
    }

    buffer[read_chars] = L'\0';
    *output = buffer;

    if (input_stream != stdin)
    {
        fclose(input_stream);
    }

    return SUCCESS;
}

// New functions for shuffle effects
void generate_effect_indices(int *indices, int width, int height, ShuffleEffect effect)
{
    int total_pixels = width * height;

    // Initialize all indices sequentially
    for (int i = 0; i < total_pixels; i++)
    {
        indices[i] = i;
    }

    switch (effect)
    {
    case EFFECT_RANDOM:
        // Normal random shuffling
        shuffle_array(indices, total_pixels);
        break;

    case EFFECT_SPIRAL:
    {
        // Spiral effect from center
        int center_x = width / 2;
        int center_y = height / 2;
        int idx = 0;

        // Each point gets an index based on its distance from the center
        for (int radius = 0; radius <= width + height; radius++)
        {
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    int dx = abs(x - center_x);
                    int dy = abs(y - center_y);
                    int dist = dx + dy;

                    if (dist == radius)
                    {
                        indices[idx++] = y * width + x;
                    }
                }
            }
        }

        // Randomize within each radius
        for (int r = 0; r <= width + height; r++)
        {
            int start_idx = 0;
            int count = 0;

            // Count how many points have this radius
            for (int i = 0; i < total_pixels; i++)
            {
                int x = indices[i] % width;
                int y = indices[i] / width;
                int dx = abs(x - center_x);
                int dy = abs(y - center_y);

                if (dx + dy == r)
                {
                    if (count == 0)
                        start_idx = i;
                    count++;
                }
            }

            // Shuffle within this radius
            if (count > 1)
            {
                shuffle_array(indices + start_idx, count);
            }
        }
    }
    break;

    case EFFECT_WIPE:
    {
        // Wipe effect from left to right
        int idx = 0;

        // Sort indices by x-coordinate (from left to right)
        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                indices[idx++] = y * width + x;
            }
        }

        // Add some randomness within each column
        for (int x = 0; x < width; x++)
        {
            shuffle_array(indices + x * height, height);
        }
    }
    break;

    case EFFECT_SNAKE:
    {
        // Snake effect (zig-zag through the screen)
        int idx = 0;

        for (int y = 0; y < height; y++)
        {
            if (y % 2 == 0)
            {
                // Even rows from left to right
                for (int x = 0; x < width; x++)
                {
                    indices[idx++] = y * width + x;
                }
            }
            else
            {
                // Odd rows from right to left
                for (int x = width - 1; x >= 0; x--)
                {
                    indices[idx++] = y * width + x;
                }
            }
        }

        // Slight variation in each row
        for (int y = 0; y < height; y++)
        {
            int start = y * width;
            int len = width;

            // Shuffle each row slightly
            for (int i = 0; i < len / 3; i++)
            {
                int idx1 = start + rand() % len;
                int idx2 = start + rand() % len;
                swap_ints(&indices[idx1], &indices[idx2]);
            }
        }
    }
    break;

    case EFFECT_BLOCKS:
    {
        // Block-wise appearance (in random blocks)
        const int block_size = 3; // Size of blocks

        // Create a list of blocks
        int blocks_x = (width + block_size - 1) / block_size;
        int blocks_y = (height + block_size - 1) / block_size;
        int total_blocks = blocks_x * blocks_y;
        int *block_order = malloc(total_blocks * sizeof(int));

        if (!block_order)
        {
            shuffle_array(indices, total_pixels); // Fallback
            break;
        }

        // Initialize block order
        for (int i = 0; i < total_blocks; i++)
        {
            block_order[i] = i;
        }

        // Shuffle the blocks
        shuffle_array(block_order, total_blocks);

        // Create the final index array, block by block
        int idx = 0;
        for (int b = 0; b < total_blocks; b++)
        {
            int block_idx = block_order[b];
            int block_y = block_idx / blocks_x;
            int block_x = block_idx % blocks_x;

            // Within a block, all pixels in random order
            int *block_pixels = malloc(block_size * block_size * sizeof(int));
            if (!block_pixels)
            {
                free(block_order);
                shuffle_array(indices, total_pixels); // Fallback
                return;
            }

            int pixel_count = 0;
            for (int y = 0; y < block_size; y++)
            {
                int real_y = block_y * block_size + y;
                if (real_y >= height)
                    continue;

                for (int x = 0; x < block_size; x++)
                {
                    int real_x = block_x * block_size + x;
                    if (real_x >= width)
                        continue;

                    block_pixels[pixel_count++] = real_y * width + real_x;
                }
            }

            // Shuffle pixels within the block
            shuffle_array(block_pixels, pixel_count);

            // Add to the overall list
            for (int i = 0; i < pixel_count; i++)
            {
                indices[idx++] = block_pixels[i];
            }

            free(block_pixels);
        }

        free(block_order);
    }
    break;
    }
}

ShuffleEffect parse_effect(const char *effect_str)
{
    if (!effect_str)
        return EFFECT_RANDOM;

    if (strcmp(effect_str, "random") == 0)
        return EFFECT_RANDOM;
    if (strcmp(effect_str, "spiral") == 0)
        return EFFECT_SPIRAL;
    if (strcmp(effect_str, "wipe") == 0)
        return EFFECT_WIPE;
    if (strcmp(effect_str, "snake") == 0)
        return EFFECT_SNAKE;
    if (strcmp(effect_str, "blocks") == 0)
        return EFFECT_BLOCKS;

    // Fallback to the default effect
    return EFFECT_RANDOM;
}

bool is_valid_effect(const char *effect)
{
    return strcmp(effect, "random") == 0 ||
           strcmp(effect, "spiral") == 0 ||
           strcmp(effect, "wipe") == 0 ||
           strcmp(effect, "snake") == 0 ||
           strcmp(effect, "blocks") == 0;
}

int show_shuffled(const ShuffleConfig *config)
{
    if (!config || !config->input_text)
    {
        return ERROR_INPUT;
    }

    sleep_time.tv_nsec = config->speed * BASE_NANOSECONDS;
    sleep_time.tv_sec = 0;

    // Calculate dimensions
    int max_length = 0;
    int current_row = 0;
    int current_col = 0;
    const wchar_t *text_ptr = config->input_text;

    while (*text_ptr)
    {
        if (*text_ptr == L'\n')
        {
            max_length = current_col > max_length ? current_col : max_length;
            current_row++;
            current_col = 0;
        }
        else
        {
            current_col++;
        }
        text_ptr++;
    }

    int width = max_length;
    int height = current_row + 1;
    int total_pixels = height * width;

    // Allocate and initialize 2D array
    wchar_t **pic_array = calloc(height, sizeof(wchar_t *));
    if (!pic_array)
    {
        return ERROR_MEMORY;
    }

    for (int i = 0; i < height; i++)
    {
        pic_array[i] = calloc(width, sizeof(wchar_t));
        if (!pic_array[i])
        {
            for (int j = 0; j < i; j++)
            {
                free(pic_array[j]);
            }
            free(pic_array);
            return ERROR_MEMORY;
        }
        // Initialize with spaces
        wmemset(pic_array[i], L' ', width);
    }

    // Fill array with content
    text_ptr = config->input_text;
    current_row = current_col = 0;

    while (*text_ptr)
    {
        if (*text_ptr == L'\n')
        {
            current_row++;
            current_col = 0;
        }
        else if (*text_ptr == L'\t')
        {
            pic_array[current_row][current_col++] = L' ';
        }
        else
        {
            pic_array[current_row][current_col++] = *text_ptr;
        }
        text_ptr++;
    }

    // Clear screen and hide cursor
    wprintf(L"\033[2J\033[1;1H\033[?25l");

    // Initialize indices array
    int *indices = calloc(total_pixels, sizeof(int));
    if (!indices)
    {
        for (int i = 0; i < height; i++)
        {
            free(pic_array[i]);
        }
        free(pic_array);
        return ERROR_MEMORY;
    }

    // Set color
    Color color = parse_color(config->color);
    if (color.is_random)
    {
        color.r = rand() % (MAX_COLOR_VALUE + 1);
        color.g = rand() % (MAX_COLOR_VALUE + 1);
        color.b = rand() % (MAX_COLOR_VALUE + 1);
    }
    wprintf(L"\033[38;2;%d;%d;%dm", color.r, color.g, color.b);

    // Display loop
    for (int pass = 0; pass < (config->is_help ? 1 : 2); pass++)
    {
        // Generate indices based on the selected effect
        generate_effect_indices(indices, width, height, config->effect);

        for (int i = 0; i < total_pixels; i++)
        {
            int idx = indices[i];
            int row = idx / width;
            int col = idx % width;

            wprintf(L"\033[%d;%dH%lc", row + 1, col + 1,
                    pic_array[row][col]);

            nanosleep(&sleep_time, NULL);
            fflush(stdout);
        }

        if (!config->is_help && pass == 0)
        {
            sleep(DISPLAY_PAUSE_SECONDS);

            // Clear for second pass
            for (int r = 0; r < height; r++)
            {
                wmemset(pic_array[r], L' ', width);
            }
        }
    }

    // Cleanup
    for (int i = 0; i < height; i++)
    {
        free(pic_array[i]);
    }
    free(pic_array);
    free(indices);

    if (!config->is_help)
    {
        wprintf(L"\033[2J\033[1;1H");
    }
    else
    {
        wprintf(L"\033[%d;1H", height + 1);
    }

    wprintf(L"\033[?25h\033[0m");

    return SUCCESS;
}
