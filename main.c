#include "shuffle.h"

char *help = "----------------------------------------------------------------------\n"
                   "shuffle: This command prints an ASCII art picture with shuffle effect.\n"
                   "---------------------------------v" VERSION "-------------------------------\n"
                   "Usage:\nshuffle SOURCE -s speed -c color\n\n"
                   "-s  speed: 1 is fast and >500 very slow, -1 = immediately.\n"
                   "-s  color: Must be in format: \"r;g;b\" or one of the standard colors:\n"
                   "    red, green, yellow, blue, magenta, cyan, orange, white, black, grey or just random.\n"
                   "-h  show this help.\n\n"
                   "Example: shuffle file.ascii -s 10 -c \"50;255;50\"  prints file.ascii in bright green.\n"
                   "         shuffle file.ascii -s 50 -c yellow  prints it in standard color (yellow).\n\n"
                   "Leave a star if you like it: https://github.com/lennart1978/shuffle\n\n";

int main(int argc, char *argv[])
{
    int option = 0;
    char *rgbColors = NULL;
    char *speed = NULL;

    while ((option = getopt(argc, argv, "hvs:c:")) >= 0)
    {
        switch (option)
        {
        case 'h':
            show_shuffled(help, 50, "white", HELP);
            return EXIT_SUCCESS;
        case 'v':
            printf("Version:" VERSION "\n");
            return EXIT_SUCCESS;
        case 's':
            speed = optarg;
            break;
        case 'c':
            rgbColors = optarg;
            break;
        case '?':
            fprintf(stderr, "Wrong argumets: Type -h for help\n");
            return EXIT_FAILURE;
        }
    }

    if (!rgbColors || !speed)
    {
        fprintf(stderr, "Type -h for help\n");
        return EXIT_FAILURE;
    }

    if (load_ascii(argv[optind]) != 0)
    {
        fprintf(stderr, "Error loading ASCII file.\n");
        return EXIT_FAILURE;
    }

    if (show_shuffled(ascii_pic, atoi(speed), rgbColors, NO_HELP) != 0)
    {
        fprintf(stderr, "Error shuffling ASCII picture.\n");
        free(ascii_pic);
        return EXIT_FAILURE;
    }

    free(ascii_pic);

    return EXIT_SUCCESS;
}
