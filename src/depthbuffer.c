#include "depthbuffer.h"

#include <stdlib.h>
#include <assert.h>

/********************
 *  Notes
 *
 ********************/

/********************/
/*      defines     */
/********************/

/********************/
/* static variables */
/********************/

/********************/
/* static functions */
/********************/

/********************/
/* public functions */
/********************/

depthbuffer_t* depthbuffer_new(uint32_t width, uint32_t height)
{
    assert(width > 0 && height > 0);

    depthbuffer_t* buffer = malloc(sizeof(depthbuffer_t));
    buffer->width = width;
    buffer->height = height;
    buffer->origin = width * height - width;
    buffer->data = malloc(width * height * sizeof(float));

    depthbuffer_clear(buffer);

    return buffer;
}

void depthbuffer_set(depthbuffer_t* buffer, uint32_t x, uint32_t y, float val)
{
    uint32_t index = buffer->origin - y * buffer->width + x;
    buffer->data[index] = val;
}

float depthbuffer_get(depthbuffer_t* buffer, uint32_t x, uint32_t y)
{
    uint32_t index = buffer->origin - y * buffer->width + x;
    return buffer->data[index];
}

void depthbuffer_clear(depthbuffer_t* buffer)
{
    uint32_t i      = 0;
    float* data     = buffer->data;
    uint32_t size   = buffer->width * buffer->height;

    // clear up to size - 7
    while (i + 8 < size)
    {
        data[i + 0] = MAX_DEPTH;
        data[i + 1] = MAX_DEPTH;
        data[i + 2] = MAX_DEPTH;
        data[i + 3] = MAX_DEPTH;
        data[i + 4] = MAX_DEPTH;
        data[i + 5] = MAX_DEPTH;
        data[i + 6] = MAX_DEPTH;
        data[i + 7] = MAX_DEPTH;

        i += 8;
    }

    // clear the rest
    while (i < size)
    {
        data[i] = MAX_DEPTH;
        i++;
    }
}

void depthbuffer_free(depthbuffer_t* buffer)
{
    free(buffer->data);
    free(buffer);
}