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
#define VERSION "1.3.2"

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

// Shuffle effect types
typedef enum
{
    EFFECT_RANDOM, // Standard random shuffle
    EFFECT_SPIRAL, // Spiral from center
    EFFECT_WIPE,   // Left to right wipe
    EFFECT_SNAKE,  // Snake-like pattern
    EFFECT_BLOCKS  // Random blocks
} ShuffleEffect;

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
    ShuffleEffect effect; // Added effect parameter
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
void generate_effect_indices(int *indices, int width, int height, ShuffleEffect effect);
int load_ascii(const char *filename, wchar_t **output);
bool is_valid_color(const char *color);
bool is_valid_effect(const char *effect);
ShuffleEffect parse_effect(const char *effect_str);
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
