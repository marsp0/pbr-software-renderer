#include "vector.h"
#include "stdio.h"

int main()
{
    vec v1;
    v1 = vec_new(1.f, 2.f, 3.f);
    vec v2 = vec_new(2.f, 3.5f, 14.2f);
    vec v3 = vec_cross(v1, v2);
    printf("%f, %f, %f\n", v1.x, v1.y, v1.z);
    printf("%f, %f, %f\n", v2.x, v2.y, v2.z);
    printf("%f, %f, %f\n", v3.x, v3.y, v3.z);
}