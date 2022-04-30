#pragma once

#include <SDL2/SDL.h>

#include "frame_buffer.h"

typedef struct display
{
    int width;
    int height;
    SDL_Surface* surface;
    SDL_Window* window;
} display;

display display_new(int width, int height);
void display_swap_buffers(display dsp, framebuffer* buffer);
void display_destroy();