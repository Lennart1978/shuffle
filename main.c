#include "shuffle.h"
#include <sys/stat.h>

const wchar_t *help = L"---------------------------------------------------------------------\n"
                      "shuffle: This command prints an ASCII art picture with shuffle effect.\n"
                      "--------------------------V.:" VERSION "-----------------------------------\n"
                      "Usage:\nshuffle SOURCE -s speed -c color\n\n"
                      "-s  speed: from 1 fast to 500 very slow.\n"
                      "-c  color: Must be in format: \"r;g;b\" or one of the standard colors:\n"
                      "    red, green, yellow, blue, magenta, cyan, orange, white, black, grey or just random.\n"
                      "-h  show this help\n"
                      "-v  show version\n\n"
                      "Example: shuffle file.ascii -s 10 -c \"50;255;50\"  prints file.ascii in bright green.\n"
                      "         shuffle file.ascii -s 50 -c yellow  prints it in standard color (yellow).\n"
                      "         You can shuffle every kind of text file (with Unicode characters).\n\n"
                      "         Now you can also use it with a pipe:\n"
                      "         cat file.ascii | shuffle -s 100 -c random\n"
                      "---------------------------------------------------------------------\n"
                      "License: MIT 2025 Lennart Martens https://github.com/lennart1978/shuffle\n";

// Handle SIGINT signal: Clean up & exit.
static void handle_sigint(int sig)
{
    wprintf(L"\033[2J\033[1;1H");

    wprintf(L"\033[?25h");

    wprintf(L"\nSIGINT: %d Clean up and exit.\n", sig);

    if (ascii_pic)
        free(ascii_pic);

    exit(0);
}

int is_valid_color(const char *color)
{
    // Check if it's a standard color
    if (!strcmp(color, "red") || !strcmp(color, "green") || !strcmp(color, "yellow") ||
        !strcmp(color, "blue") || !strcmp(color, "magenta") || !strcmp(color, "cyan") ||
        !strcmp(color, "orange") || !strcmp(color, "white") || !strcmp(color, "black") ||
        !strcmp(color, "grey") || !strcmp(color, "random"))
    {
        return 1;
    }

    // Check if it's in the "r;g;b" format
    int r, g, b;
    if (sscanf(color, "%d;%d;%d", &r, &g, &b) == 3)
    {
        if (r >= 0 && r <= 255 && g >= 0 && g <= 255 && b >= 0 && b <= 255)
        {
            return 1;
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");
    int option = 0;
    char *rgbColors = NULL;
    char *speed = NULL;

    // Pointer to the shuffle function
    void (*p_effect)(int *, int) = shuffle;

    // Register SIGINT signal
    signal(SIGINT, handle_sigint);

    // Check if input is from a pipe
    struct stat st;
    int using_pipe = (fstat(STDIN_FILENO, &st) == 0 && S_ISFIFO(st.st_mode));

    while ((option = getopt(argc, argv, "hvs:c:")) >= 0)
    {
        switch (option)
        {
        case 'h':
            show_shuffled(p_effect, help, 50, "white", HELP);
            return EXIT_SUCCESS;
        case 'v':
            wprintf(L"Version:" VERSION "\n");
            return EXIT_SUCCESS;
        case 's':
            speed = optarg;
            // Check if speed is a valid integer and within the allowed range
            int speed_val = atoi(speed);
            if (speed_val <= 0 || speed_val > 500)
            {
                wprintf(L"Invalid speed value. Must be between 1 and 500.\n");
                return EXIT_FAILURE;
            }
            break;
        case 'c':
            rgbColors = optarg;
            // Check if rgbColors is a valid color format
            if (!is_valid_color(rgbColors))
            {
                wprintf(L"Invalid color format. Must be \"r;g;b\" or a standard color.\n");
                return EXIT_FAILURE;
            }
            break;
        case '?':
            wprintf(L"Wrong arguments: Type -h for help\n");
            return EXIT_FAILURE;
        }
    }

    if (!rgbColors || !speed)
    {
        wprintf(L"Type -h for help\n");
        return EXIT_FAILURE;
    }

    if (using_pipe)
    {
        if (load_ascii("-") != 0)
        {
            wprintf(L"Error loading input from pipe.\n");
            return EXIT_FAILURE;
        }
    }
    else if (optind < argc)
    {
        // Data comes from a file
        if (load_ascii(argv[optind]) != 0)
        {
            wprintf(L"Error loading file %s.\n", argv[optind]);
            return EXIT_FAILURE;
        }
    }
    else
    {
        wprintf(L"No input provided. Type -h for help\n");
        return EXIT_FAILURE;
    }

    if (show_shuffled(p_effect, ascii_pic, atoi(speed), rgbColors, NO_HELP) != 0)
    {
        wprintf(L"Error shuffling ASCII picture.\n");
        free(ascii_pic);
        return EXIT_FAILURE;
    }

    free(ascii_pic);

    return EXIT_SUCCESS;
}
