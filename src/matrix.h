#pragma once

#include "vector.h"

typedef struct
{
    float data[4][4];
} mat;

mat mat_new(float a, float b, float c, float d,
            float e, float f, float g, float h,
            float i, float j, float k, float l,
            float m, float n, float o, float p);
mat mat_new_identity();
mat mat_add(mat m1, mat m2);
mat mat_sub(mat m1, mat m2);
mat mat_mul_mat(mat m1, mat m2);
vec mat_mul_vec(mat m, vec v);
mat mat_inverse(mat m);
mat mat_transpose(mat m);