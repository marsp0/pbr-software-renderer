#pragma once

#include <stdint.h>

typedef struct
{
    unsigned char* data;
    uint32_t cursor;
    uint32_t size;
} file_t;

file_t* file_new(const char* file_path);
void    file_free(file_t* buffer);