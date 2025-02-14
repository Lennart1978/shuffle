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

    // Initialize shuffle array
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

    for (int i = 0; i < total_pixels; i++)
    {
        indices[i] = i;
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
        shuffle_array(indices, total_pixels);

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
