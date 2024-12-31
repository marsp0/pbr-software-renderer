#pragma once

#include <stdint.h>

#include "display.h"

#define QUIT            1L << 0
#define KEY_W           1L << 1
#define KEY_A           1L << 2
#define KEY_S           1L << 3
#define KEY_D           1L << 4
#define KEY_1           1L << 5
#define KEY_2           1L << 6
#define KEY_3           1L << 7
#define KEY_4           1L << 8
#define SCROLL_UP       1L << 9     // BUTTON_4
#define SCROLL_DOWN     1L << 10    // BUTTON 5
#define LEFT_CLICK      1L << 11    // left click
#define BUTTON_2        1L << 12    // mid click
#define RIGHT_CLICK     1L << 13    // right click

typedef struct
{
    uint64_t    keys;
    int32_t     curr_x;
    int32_t     prev_x;
    int32_t     curr_y;
    int32_t     prev_y;
} input_t;

input_t handle_input(display_t* dsp);
