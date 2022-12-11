#pragma once

typedef struct
{
	int width;
	int height;
	int stride;
	unsigned char* data; /* R G B (A) */
} texture_t;

texture_t* 	texture_new(int width, int height, int stride);
void 		texture_free(texture_t* texture);