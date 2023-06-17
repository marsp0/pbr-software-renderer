#include "frame_buffer.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>


/********************
 *  Notes
 *
 ********************/

/********************/
/*      defines     */
/********************/

#define CHANNELS 4

/********************/
/* static variables */
/********************/

/********************/
/* static functions */
/********************/

/********************/
/* public functions */
/********************/

frame_buffer_t* frame_buffer_new(uint32_t width, uint32_t height)
{
    assert(width > 0 && height > 0);
    
    frame_buffer_t* buffer = malloc(sizeof(frame_buffer_t));
    buffer->width = width;
    buffer->height = height;
    buffer->data = malloc(width * height * sizeof(unsigned char) * CHANNELS);
    
    frame_buffer_clear(buffer);
    
    return buffer;
}

void frame_buffer_set(frame_buffer_t* buffer, uint32_t x, uint32_t y, uint32_t val)
{
    uint32_t width = buffer->width * CHANNELS;
    uint32_t height = buffer->height;
    uint32_t index = (width * height - width) - y * width + x * CHANNELS;

    buffer->data[index + 0] = (unsigned char)(val >> 24);
    buffer->data[index + 1] = (unsigned char)(val >> 16);
    buffer->data[index + 2] = (unsigned char)(val >> 8);
    buffer->data[index + 3] = (unsigned char)(val);
}

void frame_buffer_clear(frame_buffer_t* buffer)
{
    memset(buffer->data, 0, buffer->width * buffer->height * sizeof(unsigned char) * CHANNELS);
}

void frame_buffer_free(frame_buffer_t* buffer)
{
    free(buffer->data);
    free(buffer);
}