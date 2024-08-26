#include "shuffle.h"

int width, height;
char *ascii_pic;
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

int show_shuffled(const char *ansi_pic, int speed, char *rgb)
{
    req.tv_nsec = speed * 10000;
    req.tv_sec = 0;
    int shuffle_index, shuffled_row, shuffled_col, row, col, total_pixels;
    bool is_end = false;

    row = col = 0;

    // count rows and columns
    for (int i = 0; i < (int)strlen(ansi_pic); i++)
    {
        if (ansi_pic[i] == '\n')
        {
            row++;
            // Get the longest row of the picture
            width = (col > width) ? col : width;
            col = 0;
        }
        col++;
    }

    height = row;

    // Or total_pixels = (int)strlen(ansi_pic) is the same
    total_pixels = height * width;

    /* With exact width and height,
     * we can declare the perfect buffer now: */
    char pic_array[height][width];

    row = col = 0;

    // Store the ASCII picture in 2D array
    for (int i = 0; i < total_pixels; i++)
    {
        if (ansi_pic[i] == '\n')
        {
            row++;
            col = 0;
        }
        pic_array[row][col++] = ansi_pic[i];
    }

    // Delete screen and go to position 1, 1
    printf(CLRJ);

    int shuffle_array[total_pixels];

    // Fill the shuffle array with ascending numbers.
    for (int i = 0; i < total_pixels; i++)
    {
        shuffle_array[i] = i;
    }

    // Shuffle all the numbers in the array
    shuffle(shuffle_array, total_pixels);

    if (strcmp(rgb, "random") == 0)
    {
        srand(time(NULL));

        int r = rand() % 255;
        int g = rand() % 255;
        int b = rand() % 255;

        // Use random RGB colors
        printf("\033[38;2;%d;%d;%dm", r, g, b);
    }
    else
    {
        // Set text color to <color> argument
        printf("\033[38;2;%sm", rgb);
    }

    // Hide the cursor
    printf(INVISIBLE);

    // Show and delete the ASCII picture with amazing shuffle effect
    for (int p = 0; p < 2; p++)
    {
        for (int i = 0; i < total_pixels; i++)
        {
            shuffle_index = shuffle_array[i];
            shuffled_row = shuffle_index / width;
            shuffled_col = shuffle_index % width;

            // Move Cursor to shuffled position
            printf("\033[%d;%dH", shuffled_row + 1, shuffled_col + 1);
            // Print char at shuffled position
            printf("%c", pic_array[shuffled_row][shuffled_col]);
            // Wait some nanoseconds after every char
            nanosleep(&req, NULL);
            fflush(stdout);
        }

        // Show the ASCII art for 2 seconds at first glance
        !is_end ? (sleep(2), is_end = true) : 0;

        // Shuffle the array again for deletion effect
        shuffle(shuffle_array, total_pixels);

        // Delete the 2D array (Fill with blank spaces)
        for (int r = 0; r < height; r++)
        {
            for (int q = 0; q < width; q++)
                pic_array[r][q] = ' ';
        }
    }

    // Reset text mode
    printf(RST);

    // Delete screen and go to position 1, 1
    printf(CLRJ);

    // Show the cursor again
    printf(VISIBLE);

    return 0;
}

int load_ascii(const char *filename)
{
    long file_len;
    FILE *file;
    int c;

    file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "Can't load file.\n");
        return 1;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    file_len = ftell(file);
    rewind(file);

    // Allocate memory with size of file + 1 for null termination
    ascii_pic = malloc((size_t)file_len + 1);
    if (!ascii_pic)
    {
        fprintf(stderr, "Can't allocate memory for 'ansi_pic'");
        return 1;
    }

    // Pointer to ascii_pic
    char *temp = ascii_pic;

    // Read all chars from file and store them in ascii_pic
    while ((c = getc(file)) != EOF)
        *temp++ = (char)c;

    // NULL termination
    *temp = '\0';

    return 0;
}
