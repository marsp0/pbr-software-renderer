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

void texture_free(texture_t* texture)
{
	free(texture->data);
	free(texture);
}