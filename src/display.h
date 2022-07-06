#pragma once

#include <SDL2/SDL.h>

#include "frame_buffer.h"

typedef struct
{

    uint16_t        width;
    uint16_t        height;
    SDL_Surface*    surface;
    SDL_Window*     window;

} display;

display display_new(uint16_t width, uint16_t height);
void display_swap_buffers(display dsp, framebuffer* buffer);
void display_free();