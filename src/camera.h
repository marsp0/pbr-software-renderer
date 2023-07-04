#pragma once

#include "math.h"
#include "linux/input.h"

typedef struct
{
    float pitch;
    float yaw;

    vec_t forward;  // points inward
    vec_t side;
    vec_t up;
    vec_t position;

    float fov_x;
    float aspect_ratio;
    float near_dist;
    float far_dist;
    float top_dist;
    float bottom_dist;
    float left_dist;
    float right_dist;

} camera_t;

camera_t*   camera_new(vec_t position,
                       float pitch,
                       float yaw,
                       float fov_x,
                       float near,
                       float far,
                       float aspect_ratio);
void        camera_update(camera_t* cam, input_t input);
mat_t       camera_view_transform(camera_t* cam);
mat_t       camera_proj_transform(camera_t* cam);
void        camera_free(camera_t* cam);