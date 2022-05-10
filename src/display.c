#include "display.h"

#include "stdlib.h"

display display_new(uint16_t width, uint16_t height)
{
    display result;
    result.width = width;
    result.height = height;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) 
    {
        printf("SDL failed to initialise: %s", SDL_GetError());
        abort();
    }

    result.window = SDL_CreateWindow("PBR Software Renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
    if (result.window == NULL) 
    {
        printf("SDL window failed to initialise: %s", SDL_GetError());
        abort();
    }

    result.surface = SDL_GetWindowSurface(result.window);
    if (result.surface == NULL) 
    {
        printf("SDL failed to get window surface: %s", SDL_GetError());
        abort();
    }

    SDL_SetRelativeMouseMode(SDL_TRUE);
}

void display_swap_buffers(display dsp, framebuffer* buffer)
{

}

void display_destroy(display dsp)
{
    SDL_DestroyWindow(dsp.window);
    SDL_Quit();
}