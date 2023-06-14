#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int** colors;
    int color_count;
} color_scheme;

int get_linecount(const char* str);

#endif