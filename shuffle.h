#pragma once
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#define HELP 1
#define NO_HELP 0

#define VERSION "1.1.2"

// Nanoseconds multiplied with speed
#define NSECONDS 10000

// The last bit as flag
#define IS_END_FLAG 0x01

// Hide the cursor
#define INVISIBLE "\033[?25l"

// Show the cursor
#define VISIBLE "\033[?25h"

// Clear screen and jump to position 1,1 (ANSI position starts at 1)
#define CLRJ "\033[2J\033[1;1H"

// Clear screen
#define CLR "\033[2J"

// Reset text mode
#define RST "\033[0m"

// Dimension of the ASCII picture
extern int width, height;

// Structure for 'nanosleep' function
extern struct timespec req;

// Allocated memory for the ASCII picture
extern char *ascii_pic;

void shuffle(int *array, int n);
int load_ascii(const char *filename);
int show_shuffled(char *ansi_pic, int speed, char *rgb, int is_help);

// Swap a and b
static inline void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
