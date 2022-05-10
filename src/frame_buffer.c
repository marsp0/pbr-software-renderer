#include "frame_buffer.h"

#include <assert.h>
#include <stdlib.h>

framebuffer* framebuffer_new(uint16_t width, uint16_t height)
{
    assert(width > 0 && height > 0);
    
    framebuffer* buffer = malloc(sizeof(framebuffer));
    buffer->width = width;
    buffer->height = height;
    buffer->data = malloc(width * height * sizeof(uint32_t));
    
    return buffer;
}

void framebuffer_set(framebuffer* buffer, int x, int y, color val)
{

}

uint32_t framebuffer_get(framebuffer* buffer, int x, int y)
{
    return 0;
}

void framebuffer_destroy(framebuffer* buffer)
{
    free(buffer->data);
    free(buffer);
}