#include "math.h"

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/********************
 *  Notes
 *
 ********************/

/********************/
/*      defines     */
/********************/

#define M_PI_OVER_180 0.0174532925199f
#define M_180_OVER_PI 57.2957795131f

/********************/
/* static variables */
/********************/

/********************/
/* static functions */
/********************/

/********************/
/* public functions */
/********************/

/*      Vector      */

vec_t vec_new(float x, float y, float z)
{
    vec_t vec;
    vec.x = x;
    vec.y = y;
    vec.z = z;
    vec.w = 1.f;
    return vec;
}

vec_t vec_from_bgra(uint32_t c)
{
    float d = 1.f / 255.f;
    float b = (float)((c >> 24) & 0xFF);
    float g = (float)((c >> 16) & 0xFF);
    float r = (float)((c >>  8) & 0xFF);
    float a = (float)((c >>  0) & 0xFF);

    vec_t v = { .x = b * d,
                .y = g * d,
                .z = r * d,
                .w = a * d };
    return v;
}

vec_t vec_add(vec_t v1, vec_t v2)
{
    return vec_new(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

vec_t vec_sub(vec_t v1, vec_t v2)
{
    return vec_new(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

vec_t vec_cross(vec_t v1, vec_t v2)
{
    float x = v1.y * v2.z - v1.z * v2.y;
    float y = v1.z * v2.x - v1.x * v2.z;
    float z = v1.x * v2.y - v1.y * v2.x;
    return vec_new(x, y, z);
}

vec_t vec_scale(vec_t v, float scale)
{
    return vec_new(v.x * scale, v.y * scale, v.z * scale);
}

vec_t vec_negate(vec_t v)
{
    return vec_new(-v.x, -v.y, -v.z);
}

vec_t vec_normalize(vec_t v)
{
    float magnitude = vec_magnitude(v);
    return vec_scale(v, 1.f/magnitude);
}

vec_t vec_hadamard(vec_t v1, vec_t v2)
{
    return vec_new(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

vec_t vec_clamp(vec_t v, float min, float max)
{
    float x = f_max(f_min(v.x, max), min);
    float y = f_max(f_min(v.y, max), min);
    float z = f_max(f_min(v.z, max), min);
    return vec_new(x, y, z);
}

float vec_dot(vec_t v1, vec_t v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float vec_magnitude(vec_t v)
{
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

float vec_magnitude_sq(vec_t v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

void vec_print(vec_t v)
{
    printf("vec(%f, %f, %f, %f)\n", v.x, v.y, v.z, v.w);
}

/*
 * Matrix
 */

mat_t mat_new(float a, float b, float c, float d,
              float e, float f, float g, float h,
              float i, float j, float k, float l,
              float m, float n, float o, float p)
{
    mat_t mat;
    mat.data[0][0] = a;
    mat.data[0][1] = b;
    mat.data[0][2] = c;
    mat.data[0][3] = d;

    mat.data[1][0] = e;
    mat.data[1][1] = f;
    mat.data[1][2] = g;
    mat.data[1][3] = h;

    mat.data[2][0] = i;
    mat.data[2][1] = j;
    mat.data[2][2] = k;
    mat.data[2][3] = l;

    mat.data[3][0] = m;
    mat.data[3][1] = n;
    mat.data[3][2] = o;
    mat.data[3][3] = p;
    return mat;
}

mat_t mat_new_identity()
{
    mat_t mat;
    for (uint32_t i = 0; i < 4; i++)
    {
        for (uint32_t j = 0; j < 4; j++)
        {
            float val = 0.f;
            if (i == j)
                val = 1.f;
            mat.data[i][j] = val;
        }
    }
    return mat;
}

mat_t mat_add(mat_t m1, mat_t m2)
{
    mat_t mat;
    for (uint32_t i = 0; i < 4; i++)
    {
        for (uint32_t j = 0; j < 4; j++)
        {
            mat.data[i][j] = m1.data[i][j] + m2.data[i][j];
        }
    }
    return mat;
}

mat_t mat_sub(mat_t m1, mat_t m2)
{
    mat_t mat;
    for (uint32_t i = 0; i < 4; i++)
    {
        for (uint32_t j = 0; j < 4; j++)
        {
            mat.data[i][j] = m1.data[i][j] - m2.data[i][j];
        }
    }
    return mat;
}

mat_t mat_mul_mat(mat_t m1, mat_t m2)
{
    mat_t mat;
    for (uint32_t i = 0; i < 4; i++)
    {
        for (uint32_t j = 0; j < 4; j++)
        {
            float sum = 0.f;
            for (uint32_t x = 0; x < 4; x++)
            {
                sum += m1.data[i][x] * m2.data[x][j];
            }
            mat.data[i][j] = sum;
        }
    }
    return mat;
}

vec_t mat_mul_vec(mat_t m, vec_t v)
{
    vec_t vec;
    vec.x = m.data[0][0] * v.x + m.data[0][1] * v.y + m.data[0][2] * v.z + m.data[0][3] * v.w;
    vec.y = m.data[1][0] * v.x + m.data[1][1] * v.y + m.data[1][2] * v.z + m.data[1][3] * v.w;
    vec.z = m.data[2][0] * v.x + m.data[2][1] * v.y + m.data[2][2] * v.z + m.data[2][3] * v.w;
    vec.w = m.data[3][0] * v.x + m.data[3][1] * v.y + m.data[3][2] * v.z + m.data[3][3] * v.w;
    return vec;
}

mat_t mat_inverse(mat_t m)
{
    /* https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix */
    
    mat_t mat;
    
    float a2323 = m.data[2][2] * m.data[3][3] - m.data[2][3] * m.data[3][2];
    float a1323 = m.data[2][1] * m.data[3][3] - m.data[2][3] * m.data[3][1];
    float a1223 = m.data[2][1] * m.data[3][2] - m.data[2][2] * m.data[3][1];
    float a0323 = m.data[2][0] * m.data[3][3] - m.data[2][3] * m.data[3][0];
    float a0223 = m.data[2][0] * m.data[3][2] - m.data[2][2] * m.data[3][0];
    float a0123 = m.data[2][0] * m.data[3][1] - m.data[2][1] * m.data[3][0];
    float a2313 = m.data[1][2] * m.data[3][3] - m.data[1][3] * m.data[3][2];
    float a1313 = m.data[1][1] * m.data[3][3] - m.data[1][3] * m.data[3][1];
    float a1213 = m.data[1][1] * m.data[3][2] - m.data[1][2] * m.data[3][1];
    float a2312 = m.data[1][2] * m.data[2][3] - m.data[1][3] * m.data[2][2];
    float a1312 = m.data[1][1] * m.data[2][3] - m.data[1][3] * m.data[2][1];
    float a1212 = m.data[1][1] * m.data[2][2] - m.data[1][2] * m.data[2][1];
    float a0313 = m.data[1][0] * m.data[3][3] - m.data[1][3] * m.data[3][0];
    float a0213 = m.data[1][0] * m.data[3][2] - m.data[1][2] * m.data[3][0];
    float a0312 = m.data[1][0] * m.data[2][3] - m.data[1][3] * m.data[2][0];
    float a0212 = m.data[1][0] * m.data[2][2] - m.data[1][2] * m.data[2][0];
    float a0113 = m.data[1][0] * m.data[3][1] - m.data[1][1] * m.data[3][0];
    float a0112 = m.data[1][0] * m.data[2][1] - m.data[1][1] * m.data[2][0];

    float det = m.data[0][0] * ( m.data[1][1] * a2323 - m.data[1][2] * a1323 + m.data[1][3] * a1223 ) 
                - m.data[0][1] * ( m.data[1][0] * a2323 - m.data[1][2] * a0323 + m.data[1][3] * a0223 ) 
                + m.data[0][2] * ( m.data[1][0] * a1323 - m.data[1][1] * a0323 + m.data[1][3] * a0123 ) 
                - m.data[0][3] * ( m.data[1][0] * a1223 - m.data[1][1] * a0223 + m.data[1][2] * a0123 ) ;
    det = 1.f / det;

    mat.data[0][0] = det *   ( m.data[1][1] * a2323 - m.data[1][2] * a1323 + m.data[1][3] * a1223 );
    mat.data[0][1] = det * - ( m.data[0][1] * a2323 - m.data[0][2] * a1323 + m.data[0][3] * a1223 );
    mat.data[0][2] = det *   ( m.data[0][1] * a2313 - m.data[0][2] * a1313 + m.data[0][3] * a1213 );
    mat.data[0][3] = det * - ( m.data[0][1] * a2312 - m.data[0][2] * a1312 + m.data[0][3] * a1212 );
    mat.data[1][0] = det * - ( m.data[1][0] * a2323 - m.data[1][2] * a0323 + m.data[1][3] * a0223 );
    mat.data[1][1] = det *   ( m.data[0][0] * a2323 - m.data[0][2] * a0323 + m.data[0][3] * a0223 );
    mat.data[1][2] = det * - ( m.data[0][0] * a2313 - m.data[0][2] * a0313 + m.data[0][3] * a0213 );
    mat.data[1][3] = det *   ( m.data[0][0] * a2312 - m.data[0][2] * a0312 + m.data[0][3] * a0212 );
    mat.data[2][0] = det *   ( m.data[1][0] * a1323 - m.data[1][1] * a0323 + m.data[1][3] * a0123 );
    mat.data[2][1] = det * - ( m.data[0][0] * a1323 - m.data[0][1] * a0323 + m.data[0][3] * a0123 );
    mat.data[2][2] = det *   ( m.data[0][0] * a1313 - m.data[0][1] * a0313 + m.data[0][3] * a0113 );
    mat.data[2][3] = det * - ( m.data[0][0] * a1312 - m.data[0][1] * a0312 + m.data[0][3] * a0112 );
    mat.data[3][0] = det * - ( m.data[1][0] * a1223 - m.data[1][1] * a0223 + m.data[1][2] * a0123 );
    mat.data[3][1] = det *   ( m.data[0][0] * a1223 - m.data[0][1] * a0223 + m.data[0][2] * a0123 );
    mat.data[3][2] = det * - ( m.data[0][0] * a1213 - m.data[0][1] * a0213 + m.data[0][2] * a0113 );
    mat.data[3][3] = det *   ( m.data[0][0] * a1212 - m.data[0][1] * a0212 + m.data[0][2] * a0112 );

    return mat;
}

mat_t mat_transpose(mat_t m)
{
    mat_t mat;
    for (uint32_t i = 0; i < 4; i++)
    {
        for (uint32_t j = 0; j < 4; j++)
        {
            mat.data[i][j] = m.data[j][i];
        }
    }
    return mat;
}

void mat_print(mat_t m)
{
    printf("mat\n");
    printf("|%f, %f, %f, %f|\n", m.data[0][0], m.data[0][1], m.data[0][2], m.data[0][3]);
    printf("|%f, %f, %f, %f|\n", m.data[1][0], m.data[1][1], m.data[1][2], m.data[1][3]);
    printf("|%f, %f, %f, %f|\n", m.data[2][0], m.data[2][1], m.data[2][2], m.data[2][3]);
    printf("|%f, %f, %f, %f|\n", m.data[3][0], m.data[3][1], m.data[3][2], m.data[3][3]);
}

mat_t x_axis_rotation(float rad)
{
    float sin_val   = sinf(rad);
    float cos_val   = cosf(rad);
    
    mat_t mat       = mat_new_identity();

    mat.data[1][1]  = cos_val;
    mat.data[1][2]  = -sin_val;

    mat.data[2][1]  = sin_val;
    mat.data[2][2]  = cos_val;

    return mat;
}

mat_t y_axis_rotation(float rad)
{
    float sin_val   = sinf(rad);
    float cos_val   = cosf(rad);
    
    mat_t mat       = mat_new_identity();

    mat.data[0][0]  = cos_val;
    mat.data[0][2]  = sin_val;

    mat.data[2][0]  = -sin_val;
    mat.data[2][2]  = cos_val;

    return mat;
}

mat_t z_axis_rotation(float rad)
{
    float sin_val   = sinf(rad);
    float cos_val   = cosf(rad);

    mat_t mat       = mat_new_identity();

    mat.data[0][0]  = cos_val;
    mat.data[0][1]  = -sin_val;

    mat.data[1][0]  = sin_val;
    mat.data[1][1]  = cos_val;

    return mat;
}

float deg_to_rad(float deg)
{
    return deg * M_PI_OVER_180;
}

float rad_to_deg(float rad)
{
    return rad * M_180_OVER_PI;
}

/********************/
/*    Utilities     */
/********************/

float f_min(float a, float b)
{
    return a < b ? a : b;
}

float f_max(float a, float b)
{
    return a > b ? a : b;
}

int32_t i_max(int32_t a, int32_t b)
{
    return a > b ? a : b;
}

int32_t i_min(int32_t a, int32_t b)
{
    return a < b ? a : b;
}

uint32_t u_max(uint32_t a, uint32_t b)
{
    return a > b ? a : b;
}

uint32_t u_min(uint32_t a, uint32_t b)
{
    return a < b ? a : b;
}