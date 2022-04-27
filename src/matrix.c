#include "matrix.h"

mat mat_new(float a, float b, float c, float d,
            float e, float f, float g, float h,
            float i, float j, float k, float l,
            float m, float n, float o, float p)
{
    mat result;
    result.data[0][0] = a;
    result.data[0][1] = b;
    result.data[0][2] = c;
    result.data[0][3] = d;

    result.data[1][0] = e;
    result.data[1][1] = f;
    result.data[1][2] = g;
    result.data[1][3] = h;

    result.data[2][0] = i;
    result.data[2][1] = j;
    result.data[2][2] = k;
    result.data[2][3] = l;

    result.data[3][0] = m;
    result.data[3][1] = n;
    result.data[3][2] = o;
    result.data[3][3] = p;
    return result;
}

mat mat_new_identity()
{
    mat result;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            float val = 0.f;
            if (i == j)
                val = 1.f;
            result.data[i][j] = val;
        }
    }
    return result;
}

mat mat_add(mat m1, mat m2)
{
    mat result;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result.data[i][j] = m1.data[i][j] + m2.data[i][j];
        }
    }
    return result;
}

mat mat_sub(mat m1, mat m2)
{
    mat result;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result.data[i][j] = m1.data[i][j] - m2.data[i][j];
        }
    }
    return result;
}

mat mat_mul_mat(mat m1, mat m2)
{
    mat result;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            float sum = 0.f;
            for (int x = 0; x < 4; x++)
            {
                sum += m1.data[i][x] * m2.data[x][j];
            }
            result.data[i][j] = sum;
        }
    }
    return result;
}

vec mat_mul_vec(mat m, vec v)
{
    vec result;
    result.x = m.data[0][0] * v.x + m.data[0][1] * v.y + m.data[0][2] * v.z + m.data[0][3] * v.w;
    result.y = m.data[1][0] * v.x + m.data[1][1] * v.y + m.data[1][2] * v.z + m.data[1][3] * v.w;
    result.z = m.data[2][0] * v.x + m.data[2][1] * v.y + m.data[2][2] * v.z + m.data[2][3] * v.w;
    result.w = m.data[3][0] * v.x + m.data[3][1] * v.y + m.data[3][2] * v.z + m.data[3][3] * v.w;
    return result;
}

mat mat_inverse(mat m)
{
    // https://stackoverflow.com/questions/1148309/inverting-a-4x4-matrix
    
    mat result;
    
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

    result.data[0][0] = det *   ( m.data[1][1] * a2323 - m.data[1][2] * a1323 + m.data[1][3] * a1223 );
    result.data[0][1] = det * - ( m.data[0][1] * a2323 - m.data[0][2] * a1323 + m.data[0][3] * a1223 );
    result.data[0][2] = det *   ( m.data[0][1] * a2313 - m.data[0][2] * a1313 + m.data[0][3] * a1213 );
    result.data[0][3] = det * - ( m.data[0][1] * a2312 - m.data[0][2] * a1312 + m.data[0][3] * a1212 );
    result.data[1][0] = det * - ( m.data[1][0] * a2323 - m.data[1][2] * a0323 + m.data[1][3] * a0223 );
    result.data[1][1] = det *   ( m.data[0][0] * a2323 - m.data[0][2] * a0323 + m.data[0][3] * a0223 );
    result.data[1][2] = det * - ( m.data[0][0] * a2313 - m.data[0][2] * a0313 + m.data[0][3] * a0213 );
    result.data[1][3] = det *   ( m.data[0][0] * a2312 - m.data[0][2] * a0312 + m.data[0][3] * a0212 );
    result.data[2][0] = det *   ( m.data[1][0] * a1323 - m.data[1][1] * a0323 + m.data[1][3] * a0123 );
    result.data[2][1] = det * - ( m.data[0][0] * a1323 - m.data[0][1] * a0323 + m.data[0][3] * a0123 );
    result.data[2][2] = det *   ( m.data[0][0] * a1313 - m.data[0][1] * a0313 + m.data[0][3] * a0113 );
    result.data[2][3] = det * - ( m.data[0][0] * a1312 - m.data[0][1] * a0312 + m.data[0][3] * a0112 );
    result.data[3][0] = det * - ( m.data[1][0] * a1223 - m.data[1][1] * a0223 + m.data[1][2] * a0123 );
    result.data[3][1] = det *   ( m.data[0][0] * a1223 - m.data[0][1] * a0223 + m.data[0][2] * a0123 );
    result.data[3][2] = det * - ( m.data[0][0] * a1213 - m.data[0][1] * a0213 + m.data[0][2] * a0113 );
    result.data[3][3] = det *   ( m.data[0][0] * a1212 - m.data[0][1] * a0212 + m.data[0][2] * a0112 );

    return result;
}

mat mat_transpose(mat m)
{
    mat result;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result.data[i][j] = m.data[j][i];
        }
    }
    return result;
}