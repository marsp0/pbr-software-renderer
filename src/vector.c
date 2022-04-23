#include "vector.h"
#include "math.h"

vec vec_new(float x, float y, float z)
{
	vec result;
	result.x = x;
	result.y = y;
	result.z = z;
	return result;
}

vec vec_add(vec v1, vec v2)
{
	return vec_new(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

vec vec_sub(vec v1, vec v2)
{
	return vec_new(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

vec vec_cross(vec v1, vec v2)
{
	float x = v1.y * v2.z - v1.z * v2.y;
	float y = v1.z * v2.x - v1.x * v2.z;
	float z = v1.x * v2.y - v1.y * v2.x;
	return vec_new(x, y, z);
}

vec vec_scale(vec v, float scale)
{
	return vec_new(v.x * scale, v.y * scale, v.z * scale);
}

vec vec_negate(vec v)
{
	return vec_new(-v.x, -v.y, -v.z);
}

vec vec_normalize(vec v)
{
	float magnitude = vec_magnitude(v);
	return vec_scale(v, 1.f/magnitude);
}

float vec_dot(vec v1, vec v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float vec_magnitude(vec v)
{
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}