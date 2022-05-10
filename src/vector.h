#pragma once

typedef struct
{
    float x;
    float y;
    float z;
    float w;
} vec;

vec vec_new(float x, float y, float z);
vec vec_add(vec v1, vec v2);
vec vec_sub(vec v1, vec v2);
vec vec_cross(vec v1, vec v2);
vec vec_scale(vec v, float scale);
vec vec_negate(vec v);
vec vec_normalize(vec v);
float vec_dot(vec v1, vec v2);
float vec_magnitude(vec v);
void vec_print(vec v);