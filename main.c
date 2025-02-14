#include "shuffle.h"
#include <sys/stat.h>
#include <getopt.h>

static const wchar_t *HELP_TEXT = L"---------------------------------------------------------------------\n"
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

// Signal handler for cleanup
static void handle_interrupt(int sig)
{
    (void)sig; // Unused parameter
    wprintf(L"\033[2J\033[1;1H\033[?25h");
    cleanup_resources();
    exit(EXIT_SUCCESS);
}

static void show_help(void)
{
    ShuffleConfig config = {
        .speed = 50,
        .color = "white",
        .is_help = true,
        .input_text = HELP_TEXT};
    show_shuffled(&config);
}

static bool is_pipe_input(void)
{
    struct stat st;
    return (fstat(STDIN_FILENO, &st) == 0 && S_ISFIFO(st.st_mode));
}

static bool validate_speed(const char *speed_str, int *speed_val)
{
    char *endptr;
    long speed = strtol(speed_str, &endptr, 10);

    if (*endptr != '\0' || speed < MIN_SPEED || speed > MAX_SPEED)
    {
        wprintf(L"Invalid speed value. Must be between %d and %d.\n",
                MIN_SPEED, MAX_SPEED);
        return false;
    }

    *speed_val = (int)speed;
    return true;
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "");
    signal(SIGINT, handle_interrupt);

    int option;
    char *color_arg = NULL;
    char *speed_str = NULL;
    int speed_val = 0;
    bool using_pipe = is_pipe_input();

    // Parse command line options
    while ((option = getopt(argc, argv, "hvs:c:")) != -1)
    {
        switch (option)
        {
        case 'h':
            show_help();
            return EXIT_SUCCESS;

        case 'v':
            wprintf(L"Version: " VERSION "\n");
            return EXIT_SUCCESS;

        case 's':
            speed_str = optarg;
            if (!validate_speed(speed_str, &speed_val))
            {
                return EXIT_FAILURE;
            }
            break;

        case 'c':
            color_arg = optarg;
            if (!is_valid_color(color_arg))
            {
                wprintf(L"Invalid color format. Must be \"r;g;b\" or a standard color.\n");
                return EXIT_FAILURE;
            }
            break;

        default:
            wprintf(L"Wrong arguments: Type -h for help\n");
            return EXIT_FAILURE;
        }
    }

    // Validate required arguments
    if (!color_arg || !speed_str)
    {
        wprintf(L"Missing required arguments. Type -h for help\n");
        return EXIT_FAILURE;
    }

    // Load ASCII content
    wchar_t *content = NULL;
    int load_result;

    if (using_pipe)
    {
        load_result = load_ascii("-", &content);
    }
    else if (optind < argc)
    {
        load_result = load_ascii(argv[optind], &content);
    }
    else
    {
        wprintf(L"No input provided. Type -h for help\n");
        return EXIT_FAILURE;
    }

    if (load_result != SUCCESS)
    {
        wprintf(L"Error loading input: %d\n", load_result);
        return EXIT_FAILURE;
    }

    // Configure and run shuffle
    ShuffleConfig config = {
        .speed = speed_val,
        .color = color_arg,
        .is_help = false,
        .input_text = content};

    int result = show_shuffled(&config);

    // Cleanup
    free(content);

    if (result != SUCCESS)
    {
        wprintf(L"Error during shuffle operation: %d\n", result);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
