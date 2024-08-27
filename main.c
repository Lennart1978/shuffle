#include "shuffle.h"

const char *help = "----------------------------------------------------------------------\n"
                   "\033[1;35mshuffle: This command prints an ASCII art picture with shuffle effect.\n\033[0m"
                   "---------------------------------v"VERSION"-------------------------------\n\033[1m"
                   "Usage: shuffle <ascii picture> <speed> <\"color\">\n"
                   "speed: 1 is fast and >500 very slow, -1 = immediately.\n"
                   "color: Must be in quotation marks of format: \"r;g;b\" or just \"random\".\n"
                   "Example: shuffle file.ascii 10 \"50;255;50\" - prints \"file.ascii\" in bright green.\n\033[0m"
                   "\033[1;32mLeave a star if you like it: https://github.com/lennart1978/shuffle\033[0m\n";

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("%s\n", help);
        return 1;
    }

    if (load_ascii(argv[1]) != 0)
    {
        fprintf(stderr, "Error loading ascii.\n");
        return 1;
    }

    if (show_shuffled(ascii_pic, atoi(argv[2]), argv[3]) != 0)
    {
        fprintf(stderr, "Error shuffling ascii picture.\n");
        free(ascii_pic);
        return 1;
    }

    free(ascii_pic);

    return 0;
}
