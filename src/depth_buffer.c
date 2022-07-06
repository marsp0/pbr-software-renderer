#include "depth_buffer.h"

#include <stdlib.h>
#include <assert.h>

depthbuffer* depthbuffer_new(uint16_t width, uint16_t height)
{
    assert(width > 0 && height > 0);

    depthbuffer* buffer = malloc(sizeof(depthbuffer));
    buffer->width = width;
    buffer->height = height;
    buffer->data = malloc(width * height * sizeof(float));

    return buffer;
}

void depthbuffer_set(depthbuffer* buffer, int x, int y, float val)
{

}

float depthbuffer_get(depthbuffer* buffer, int x, int y)
{
    return 0.f;
}

void depthbuffer_free(depthbuffer* buffer)
{
    free(buffer->data);
    free(buffer);
}