#include "shuffle.h"

wchar_t *help = L"---------------------------------------------------------------------\n"
                "shuffle: This command prints an ASCII art picture with shuffle effect.\n"
                "--------------------------V.:"VERSION"-----------------------------------\n"
                "Usage:\nshuffle SOURCE -s speed -c color\n\n"
                "-s  speed: 1 is fast and >500 very slow, -1 = immediately.\n"
                "-c  color: Must be in format: \"r;g;b\" or one of the standard colors:\n"
                "    red, green, yellow, blue, magenta, cyan, orange, white, black, grey or just random.\n"
                "-h  show this help\n"
                "-v  show version\n\n"
                "Example: shuffle file.ascii -s 10 -c \"50;255;50\"  prints file.ascii in bright green.\n"
                "         shuffle file.ascii -s 50 -c yellow  prints it in standard color (yellow).\n"
                "         You can shuffle every kind of text file (with Unicode characters).\n\n"
                "License: MIT 2024 Lennart Martens https://github.com/lennart1978/shuffle\n";

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

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");
    int option = 0;
    char *rgbColors = NULL;
    char *speed = NULL;

    // Pointer to the shuffle function, maybe I'll add more effects...
    void (*p_effect)(int *, int) = &shuffle;

    // Register SIGINT signal
    signal(SIGINT, handle_sigint);

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
            break;
        case 'c':
            rgbColors = optarg;
            break;
        case '?':
            wprintf(L"Wrong argumets: Type -h for help\n");
            return EXIT_FAILURE;
        }
    }

    if (!rgbColors || !speed)
    {
        wprintf(L"Type -h for help\n");
        return EXIT_FAILURE;
    }

    if (load_ascii(argv[optind]) != 0)
    {
        wprintf(L"Error loading ASCII file.\n");
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
