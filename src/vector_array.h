#pragma once

#include <stdint.h>

#include "vector.h"

typedef struct
{
    uint32_t size;
    uint32_t capacity;
    vec* data;
} vec_array;

vec_array* vec_array_new();
void vec_array_add(vec_array* array, vec element);
void vec_array_destroy(vec_array* array);
