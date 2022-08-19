#pragma once

#include <X11/Xlib.h>
#include <stdint.h>

#include "../frame_buffer.h"

typedef struct
{

    int             width;
    int             height;
    Display*        display;
    Window          window;
    int             screen;
    XImage*         ximage;
    unsigned char*  buffer;

} display_t;

display_t* display_new(int width, int height);
void display_draw(display_t* dsp, frame_buffer_t* frame_buffer);
void display_free(display_t* dsp);