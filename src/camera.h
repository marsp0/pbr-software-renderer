#pragma once

#include <stdbool.h>

#include "math.h"
#include "linux/input.h"

typedef struct
{
    float   pitch;
    float   yaw;

    vec4_t  forward;  // points inward
    vec4_t  side;
    vec4_t  up;
    vec4_t  position;

    float   fov_x;
    float   asp_ratio;
    float   n_dist;
    float   f_dist;
    float   t_dist;
    float   b_dist;
    float   l_dist;
    float   r_dist;

} camera_t;

camera_t*   camera_new(vec4_t position,
                       float pitch,
                       float yaw,
                       float fov_x,
                       float near,
                       float far,
                       float aspect_ratio);
void        camera_update(camera_t* cam, input_t input, float dt);
mat_t       camera_view_transform(camera_t* cam);
mat_t       camera_proj_transform(camera_t* cam);
void        camera_free(camera_t* cam);