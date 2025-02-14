#pragma once
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>
#include <locale.h>
#include <errno.h>
#include <signal.h>
#include <stdbool.h>

// Program version
#define VERSION "1.3.1"

// Constants for program configuration
#define MAX_SPEED 500
#define MIN_SPEED 1
#define BASE_NANOSECONDS 10000
#define INITIAL_BUFFER_SIZE 1024
#define DISPLAY_PAUSE_SECONDS 2

// Color constants
#define MAX_COLOR_VALUE 255
#define DEFAULT_COLOR "white"

// Return codes
#define SUCCESS 0
#define ERROR_MEMORY -1
#define ERROR_FILE -2
#define ERROR_INPUT -3

// Function types
typedef void (*shuffle_func)(int *, size_t);

// Structure for program configuration
typedef struct
{
    int width;
    int height;
    int speed;
    char *color;
    bool is_help;
    const wchar_t *input_text;
} ShuffleConfig;

// Structure for color
typedef struct
{
    int r;
    int g;
    int b;
    bool is_random;
} Color;

// Function declarations
void shuffle_array(int *array, size_t n);
int load_ascii(const char *filename, wchar_t **output);
bool is_valid_color(const char *color);
int show_shuffled(const ShuffleConfig *config);
Color parse_color(const char *color_str);
void cleanup_resources(void);

// Inline functions
static inline void swap_ints(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
