#pragma once

#include <stdint.h>

typedef struct
{
	uint32_t width;
	uint32_t height;
	uint32_t stride;
	unsigned char* data; /* R G B (A) */
} texture_t;

texture_t* 	texture_new(uint32_t width, uint32_t height, uint32_t stride);
void 		texture_free(texture_t* texture);