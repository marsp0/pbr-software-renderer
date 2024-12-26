#pragma once

#include <stdint.h>

typedef struct
{
	uint32_t width;
	uint32_t height;
	uint32_t stride;
	unsigned char* data; /* R G B (A) */
} texture_t;

texture_t*  texture_new(uint32_t width, uint32_t height, uint32_t stride);
uint32_t    texture_get(texture_t* texture, uint32_t x, uint32_t y);
void        texture_free(texture_t* texture);