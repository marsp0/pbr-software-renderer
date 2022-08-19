#include "frame_buffer.h"

#include <assert.h>
#include <stdlib.h>

frame_buffer_t* frame_buffer_new(int width, int height)
{
    assert(width > 0 && height > 0);
    
    frame_buffer_t* buffer = malloc(sizeof(frame_buffer_t));
    buffer->width = width;
    buffer->height = height;
    buffer->data = malloc(width * height * sizeof(uint32_t));
    
    return buffer;
}

void frame_buffer_set(frame_buffer_t* buffer, int x, int y, color val)
{

}

uint32_t frame_buffer_get(frame_buffer_t* buffer, int x, int y)
{
    return 0;
}

void frame_buffer_free(frame_buffer_t* buffer)
{
    free(buffer->data);
    free(buffer);
}