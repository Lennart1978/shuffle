#include "shuffle.h"

int width, height;
wchar_t *ascii_pic;
struct timespec req;

// The Fischer - Yates shuffle algorithm
void shuffle(int *array, int n)
{
    srand((unsigned int)time(NULL));
    for (int i = n - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        swap(&array[i], &array[j]);
    }
}

int show_shuffled(wchar_t *ansi_pic, int speed, char *rgb, int is_help)
{
    // The wait time between each printed char is:  speed * 10000 nanoseconds
    req.tv_nsec = speed * NSECONDS;
    req.tv_sec = 0;
    int shuffle_index = 0, shuffled_row = 0, shuffled_col = 0, row = 0, col = 0, total_pixels = 0, max_length = 0;
    wchar_t c;
    int flag = 0;

    // Pointer to the loaded string
    wchar_t *textfile;
    textfile = ansi_pic;

    // Get 2D dimension of the text file (x = max_length, y = rows)
    while ((c = *textfile++) != L'\0')
    {
        if (c == L'\n')
        {
            row++;
            max_length = col > max_length ? col : max_length;
            col = -1;
        }
        col++;
    }

    // Reset pointer
    textfile = ansi_pic;

    // Create 2D text array
    wchar_t **pic_array = malloc(((long unsigned int)(row + 1) * sizeof(wchar_t *)));

    for (int i = 0; i < row + 1; i++)
    {
        pic_array[i] = malloc((long unsigned int)(max_length + 1) * sizeof(wchar_t));
    }

    if (pic_array == NULL)
    {
        wprintf(L"Can't allocate memory for 'pic_array'.\n");

        return EXIT_FAILURE;
    }

    width = max_length;
    height = row;

    total_pixels = height * width;

    row = col = 0;

    /*Fill shorter rows with blank spaces and replace tabs with blank space.
     *Store everything in 2D array */
    while ((c = *textfile++) != L'\0')
    {
        pic_array[row][col] = c;
        if (c == '\n')
        {
            if (col < max_length - 1)
            {
                for (c = L' '; col < max_length; pic_array[row][col++] = c)
                    ;
            }
            col = -1;
            row++;
        }
        else if (c == L'\t')
        {
            pic_array[row][col] = L' ';
        }
        col++;
    }

    // Delete screen and go to position 1, 1
    wprintf(L"\033[2J\033[1;1H");

    int *shuffle_array = malloc((long unsigned int)total_pixels * sizeof(int));

    // Fill the shuffle array with ascending numbers.
    for (int i = 0; i < total_pixels; i++)
    {
        shuffle_array[i] = i;
    }

    // Shuffle all the numbers in the array
    shuffle(shuffle_array, total_pixels);

    int r, g, b;

    if (!strcmp(rgb, "random")) // Print with random RGB color
    {
        srand((unsigned int)time(NULL));
        r = rand() % 255;
        g = rand() % 255;
        b = rand() % 255;
    }
    else if (!strcmp(rgb, "red"))
        r = 255, g = 0, b = 0;
    else if (!strcmp(rgb, "green"))
        r = 0, g = 255, b = 0;
    else if (!strcmp(rgb, "yellow"))
        r = 255, g = 255, b = 0;
    else if (!strcmp(rgb, "blue"))
        r = 0, g = 0, b = 255;
    else if (!strcmp(rgb, "magenta"))
        r = 255, g = 0, b = 255;
    else if (!strcmp(rgb, "cyan"))
        r = 0, g = 255, b = 255;
    else if (!strcmp(rgb, "white"))
        r = 255, g = 255, b = 255;
    else if (!strcmp(rgb, "black"))
        r = 0, g = 0, b = 0;
    else if (!strcmp(rgb, "orange"))
        r = 255, g = 165, b = 0;
    else if (!strcmp(rgb, "grey"))
        r = 127, g = 127, b = 127;
    else
    {
        wprintf(L"\033[38;2;%sm", rgb); // Print with given RGB values
        goto jump;
    }

    wprintf(L"\033[38;2;%d;%d;%dm", r, g, b); // Print with one of the 'standard' colors

jump:

    // Hide the cursor
    wprintf(L"\033[?25l");

    // Show and delete the ASCII picture with amazing shuffle effect
    for (int p = 0; p < (is_help ? 1 : 2); p++)
    {
        for (int i = 0; i < total_pixels; i++)
        {
            shuffle_index = shuffle_array[i];
            shuffled_row = shuffle_index / width;
            shuffled_col = shuffle_index % width;

            // Move Cursor to shuffled position
            wprintf(L"\033[%d;%dH", shuffled_row + 1, shuffled_col + 1);
            // Print char at shuffled position
            wprintf(L"%lc", pic_array[shuffled_row][shuffled_col]);
            // Wait some nanoseconds after every char
            nanosleep(&req, NULL);
            fflush(stdout);
        }

        // Show the ASCII art for 2 seconds at first glance (with bit flag)
        ((!(flag & IS_END_FLAG)) && !is_help) ? sleep(2), flag |= IS_END_FLAG : 0;

        if (!is_help)
        {
            // Shuffle the array again for deletion effect
            shuffle(shuffle_array, total_pixels);

            // Delete the 2D array (Fill with blank spaces)
            for (int r2 = 0; r2 < height; r2++)
            {
                for (int q = 0; q < width; pic_array[r2][q++] = L' ')
                    ;
            }
        }
    }

    // Show the cursor again
    wprintf(L"\033[?25h");

    // Reset text mode
    wprintf(L"\033[0m");

    // Delete screen and go to position 1, 1 (only if not showing help)
    !is_help ? wprintf(L"0\33[2J\033[1;1H") : wprintf(L"\033[19;1H");

    // free the allocated memory
    for (int i = 0; i < row + 1; i++)
    {
        free(pic_array[i]);
    }

    free(pic_array);

    free(shuffle_array);

    return EXIT_SUCCESS;
}

int load_ascii(const char *filename)
{
    FILE *file = NULL;
    wint_t wc;
    size_t read_chars = 0;
    size_t buffer_size = 1024;

    file = fopen(filename, "r");
    if (!file)
    {
        wprintf(L"Can't load file %s: %s\n", filename, strerror(errno));
        return EXIT_FAILURE;
    }

    ascii_pic = malloc(buffer_size * sizeof(wchar_t));

    if (!ascii_pic)
    {
        wprintf(L"Can't allocate memory for 'ascii_pic'\n");
        fclose(file);
        return EXIT_FAILURE;
    }

    while ((wc = fgetwc(file)) != WEOF)
    {
        if (read_chars >= buffer_size - 1)
        {
            buffer_size *= 2;
            wchar_t *new_buffer = realloc(ascii_pic, buffer_size * sizeof(wchar_t));
            if (!new_buffer)
            {
                wprintf(L"Memory reallocation failed\n");
                free(ascii_pic);
                fclose(file);
                return EXIT_FAILURE;
            }
            ascii_pic = new_buffer;
        }
        ascii_pic[read_chars++] = (wchar_t)wc;
    }

    if (ferror(file))
    {
        wprintf(L"Error reading file: %s\n", strerror(errno));
        free(ascii_pic);
        fclose(file);
        return EXIT_FAILURE;
    }

    ascii_pic[read_chars] = L'\0';

    fclose(file);
    return EXIT_SUCCESS;
}
