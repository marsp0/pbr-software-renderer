#include "frame_buffer.h"

#include <assert.h>
#include <stdlib.h>

framebuffer* framebuffer_new(int width, int height)
{
    assert(width > 0 && height > 0);
    
    framebuffer* buffer = malloc(sizeof(framebuffer));
    buffer->width = width;
    buffer->height = height;
    buffer->data = malloc(width * height * sizeof(color));
    
    return buffer;
}

void framebuffer_set(framebuffer* buffer, int x, int y, color val)
{

}

color framebuffer_get(framebuffer* buffer, int x, int y)
{
    return color_new_f(0.f, 0.f, 0.f);
}

void framebuffer_destroy(framebuffer* buffer)
{
    free(buffer->data);
    free(buffer);
}