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

vec4_t texture_sample(texture_t* texture, float u, float v)
{
    // this function converts rgba from image to bgra

    uint32_t x          = (uint32_t)(u * (float)texture->width) - 1;
    uint32_t y          = (uint32_t)(v * (float)texture->height) - 1;
    uint32_t index      = (x + texture->width * y) * texture->stride;
    unsigned char* data = texture->data;

    float d = 1.f / 255.f;
    float b = (float)(data[index + 2] & 0xFF);
    float g = (float)(data[index + 1] & 0xFF);
    float r = (float)(data[index + 0] & 0xFF);

    return vec4_new(b * d, g * d, r * d);
}

void texture_free(texture_t* texture)
{
	free(texture->data);
	free(texture);
}