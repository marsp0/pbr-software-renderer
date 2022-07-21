#pragma once

#include <X11/Xlib.h>
#include <stdint.h>

#include "../frame_buffer.h"

typedef struct
{

    Display*        display;
    Window          window;
    int             screen;
    XImage*         ximage;
    unsigned char*  buffer;

} display;

display* display_new(int width, int height);
void display_draw(display* dsp, framebuffer* frame_buffer);
void display_free(display* dsp);