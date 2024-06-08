#pragma once

#include <X11/Xlib.h>
#include <stdint.h>

#include "../framebuffer.h"

typedef struct
{

    uint32_t        width;
    uint32_t        height;
    Display*        display;
    Window          window;
    int32_t         screen;
    XImage*         ximage;
    unsigned char*  buffer;

} display_t;

display_t* display_new(uint32_t width, uint32_t height);
void display_draw(display_t* dsp, const framebuffer_t* framebuffer);
void display_clear(display_t* dsp);
void display_free(display_t* dsp);