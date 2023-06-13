#pragma once

#include "math.h"

typedef struct
{
    float pitch;
    float yaw;
    vec_t forward;
    vec_t position;
    mat_t view;
    mat_t projection;
} camera_t;

camera_t*   camera_new(vec_t position);
void        camera_update(camera_t* cam);
void        camera_free(camera_t* cam);