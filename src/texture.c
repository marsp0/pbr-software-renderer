#include "texture.h"

texture_t* texture_new(int width, int height, int stride)
{
	texture_t* texture = malloc(sizeof(texture_t));
	texture->width = width;
	texture->height = height;
	texture->stride = stride;
	texture->data = malloc(width * height * stride);
}

void texture_free(texture_t* texture)
{
	free(texture->data);
	free(texture);
}