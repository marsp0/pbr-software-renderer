#include "depthbuffer.h"

#include <stdlib.h>
#include <assert.h>

depthbuffer_t* depthbuffer_new(uint32_t width, uint32_t height)
{
    assert(width > 0 && height > 0);

    depthbuffer_t* buffer = malloc(sizeof(depthbuffer_t));
    buffer->width = width;
    buffer->height = height;
    buffer->data = malloc(width * height * sizeof(float));

    return buffer;
}
#if 0
void depthbuffer_set(depthbuffer_t* buffer, int x, int y, float val)
{

}

float depthbuffer_get(depthbuffer_t* buffer, int x, int y)
{
    return 0.f;
}
#endif
void depthbuffer_free(depthbuffer_t* buffer)
{
    free(buffer->data);
    free(buffer);
}