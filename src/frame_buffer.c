#include "frame_buffer.h"

#include <assert.h>
#include <stdlib.h>

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
    
    return buffer;
}

void frame_buffer_free(frame_buffer_t* buffer)
{
    free(buffer->data);
    free(buffer);
}