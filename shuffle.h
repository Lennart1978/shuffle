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

#define HELP 1
#define NO_HELP 0

#define VERSION "1.2.1"

// Nanoseconds multiplied with speed
#define NSECONDS 10000

// The last bit as flag
#define IS_END_FLAG 0x01

// Dimension of the ASCII picture
extern int width, height;

// Structure for 'nanosleep' function
extern struct timespec req;

// Allocated memory for the ASCII picture
extern wchar_t *ascii_pic;

void shuffle(int *array, int n);
int load_ascii(const char *filename);
int show_shuffled(wchar_t *ansi_pic, int speed, char *rgb, int is_help);

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
