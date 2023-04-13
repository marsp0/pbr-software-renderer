#pragma once

#include <X11/Xlib.h>
#include <stdint.h>

#include "../frame_buffer.h"

#define RGB_CHANNELS 4 /* XCreateImage fails if this is different than 1, 2 or 4 */

typedef struct
{

    uint32_t        width;
    uint32_t        height;
    Display*        display;
    Window          window;
    int             screen;
    XImage*         ximage;
    unsigned char*  buffer;

} display_t;

display_t* display_new(uint32_t width, uint32_t height);
void display_draw(display_t* dsp, const frame_buffer_t* frame_buffer);
void display_free(display_t* dsp);