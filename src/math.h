#pragma once

#include <stdint.h>

/********************/
/*      Vector      */
/********************/

typedef struct
{
    float x;
    float y;
    float z;
    float w;
} vec_t;

vec_t vec_new(float x, float y, float z);
vec_t vec_from_bgra(uint32_t c);
vec_t vec_add(vec_t v1, vec_t v2);
vec_t vec_sub(vec_t v1, vec_t v2);
vec_t vec_cross(vec_t v1, vec_t v2);
vec_t vec_scale(vec_t v, float scale);
vec_t vec_negate(vec_t v);
vec_t vec_normalize(vec_t v);
vec_t vec_hadamard(vec_t v1, vec_t v2);
vec_t vec_clamp(vec_t v, float min, float max);
float vec_dot(vec_t v1, vec_t v2);
float vec_magnitude(vec_t v);
float vec_magnitude_sq(vec_t v);
void  vec_print(vec_t v);

/********************/
/*       Matrix     */
/********************/

typedef struct
{
    float data[4][4];
} mat_t;

mat_t mat_new(float a, float b, float c, float d,
              float e, float f, float g, float h,
              float i, float j, float k, float l,
              float m, float n, float o, float p);
mat_t mat_new_identity();
mat_t mat_add(mat_t m1, mat_t m2);
mat_t mat_sub(mat_t m1, mat_t m2);
mat_t mat_mul_mat(mat_t m1, mat_t m2);
vec_t mat_mul_vec(mat_t m, vec_t v);
mat_t mat_inverse(mat_t m);
mat_t mat_transpose(mat_t m);
void  mat_print(mat_t m);

/********************/
/*  Rotation Utils  */
/********************/

mat_t x_axis_rotation(float rad);
mat_t y_axis_rotation(float rad);
mat_t z_axis_rotation(float rad);
float deg_to_rad(float deg);
float rad_to_deg(float rad);

/********************/
/*       Plane      */
/********************/

typedef struct
{
    vec_t p;
    vec_t n;
} plane_t;

/********************/
/*      Sphere      */
/********************/

typedef struct
{
    vec_t c;
    float r;
} sphere_t;

/********************/
/*    Utilities     */
/********************/

float f_min(float a, float b);
float f_max(float a, float b);
int32_t i_max(int32_t a, int32_t b);
int32_t i_min(int32_t a, int32_t b);
uint32_t u_max(uint32_t a, uint32_t b);
uint32_t u_min(uint32_t a, uint32_t b);