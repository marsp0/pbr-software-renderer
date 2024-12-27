#pragma once

#include <stdint.h>

#include "display.h"

#define QUIT    1L << 0
#define KEY_W   1L << 1
#define KEY_A   1L << 2
#define KEY_S   1L << 3
#define KEY_D   1L << 4
#define KEY_1   1L << 5
#define KEY_2   1L << 6
#define KEY_3   1L << 7
#define KEY_4   1L << 8

typedef struct
{
    uint64_t keys;
    int32_t  dx;
    int32_t  dy;
    
    int32_t     x;
    int32_t     y;
    int32_t     prev_x;
    int32_t     prev_y;
} input_t;

input_t handle_input(display_t* dsp);
