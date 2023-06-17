#include "framebuffer.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "rasterizer_constants.h"

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

framebuffer_t* framebuffer_new(uint32_t width, uint32_t height)
{
    assert(width > 0 && height > 0);

    framebuffer_t* buffer = malloc(sizeof(framebuffer_t));
    buffer->width = width;
    buffer->height = height;
    buffer->data = malloc(width * height * sizeof(unsigned char) * RGB_CHANNELS);

    framebuffer_clear(buffer);

    return buffer;
}

void framebuffer_set(framebuffer_t* buffer, uint32_t x, uint32_t y, uint32_t val)
{
    uint32_t width = buffer->width * RGB_CHANNELS;
    uint32_t height = buffer->height;
    uint32_t index = (width * height - width) - y * width + x * RGB_CHANNELS;

    buffer->data[index + 0] = (unsigned char)(val >> 24);
    buffer->data[index + 1] = (unsigned char)(val >> 16);
    buffer->data[index + 2] = (unsigned char)(val >> 8);
    buffer->data[index + 3] = (unsigned char)(val);
}

void framebuffer_clear(framebuffer_t* buffer)
{
    memset(buffer->data, 0, buffer->width * buffer->height * sizeof(unsigned char) * RGB_CHANNELS);
}

void framebuffer_free(framebuffer_t* buffer)
{
    free(buffer->data);
    free(buffer);
}