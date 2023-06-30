#pragma once

#include "math.h"

typedef struct
{
    float   pitch;
    float   yaw;
    vec_t   forward;
    vec_t   position;

    float   fov_x;
    float   aspect_ratio;
    float   near_dist;
    float   far_dist;
    float   top_dist;
    float   bottom_dist;
    float   left_dist;
    float   right_dist;

    mat_t   view;
    mat_t   proj;
} camera_t;

camera_t*   camera_new(vec_t position,
                       vec_t forward,
                       float fov_x,
                       float near,
                       float far,
                       float aspect_ratio);
void        camera_update(camera_t* cam);
void        camera_free(camera_t* cam);