#pragma once
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

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
int show_shuffled(const char *ansi_pic, int speed, char *rgb);

// Swap a and b very fast (inline Assembly test: not necessary, C is really fast enough)
static inline void swap(int *a, int *b)
{
    __asm__(
        "movl (%0), %%eax;\n"
        "movl (%1), %%ebx;\n"
        "movl %%ebx, (%0);\n"
        "movl %%eax, (%1);"
        :
        : "r"(a), "r"(b)
        : "%eax", "%ebx");
}
