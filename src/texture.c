#include "texture.h"

#include <stdlib.h>

texture_t* texture_new(uint32_t width, uint32_t height, uint32_t stride)
{
	texture_t* texture = malloc(sizeof(texture_t));
	texture->width = width;
	texture->height = height;
	texture->stride = stride;
	texture->data = malloc(width * height * stride);
	return texture;
}

uint32_t texture_get(texture_t* texture, uint32_t x, uint32_t y)
{
    // TODO: BGRA vs RGBA????? GOOD PLACE FOR BOUNDRY? Loader / texture / screen ? ????

    uint32_t index = (x + texture->width * y) * texture->stride;
    unsigned char* data = texture->data;

    uint32_t result = 0;
    result += data[index + 0] << 8;     // R
    result += data[index + 1] << 16;    // G
    result += data[index + 2] << 24;    // B
    if (texture->stride == 4)
    {
        result += data[index + 3];          // A
    }

    return result;
}

void texture_free(texture_t* texture)
{
	free(texture->data);
	free(texture);
}