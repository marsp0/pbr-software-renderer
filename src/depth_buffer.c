#include "depth_buffer.h"

#include <stdlib.h>
#include <assert.h>

depth_buffer_t* depth_buffer_new(int width, int height)
{
    assert(width > 0 && height > 0);

    depth_buffer_t* buffer = malloc(sizeof(depth_buffer_t));
    buffer->width = width;
    buffer->height = height;
    buffer->data = malloc(width * height * sizeof(float));

    return buffer;
}

void depth_buffer_set(depth_buffer_t* buffer, int x, int y, float val)
{

}

float depth_buffer_get(depth_buffer_t* buffer, int x, int y)
{
    return 0.f;
}

void depth_buffer_free(depth_buffer_t* buffer)
{
    free(buffer->data);
    free(buffer);
}