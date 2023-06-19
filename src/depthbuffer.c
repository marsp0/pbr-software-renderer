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
    for (uint32_t i = 0; i < buffer->width; i++)
    {
        for (uint32_t j = 0; j < buffer->height; j++)
        {
            buffer->data[j * buffer->width + i] = MAX_DEPTH;
        }
    }
}

void depthbuffer_free(depthbuffer_t* buffer)
{
    free(buffer->data);
    free(buffer);
}