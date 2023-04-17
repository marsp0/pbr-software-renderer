#pragma once

#include "math.h"

typedef struct
{
	float pitch;
	float yaw;
	vec_t position;
} camera_t;

camera_t* 	camera_new(vec_t position);
void 		camera_free(camera_t* camera);