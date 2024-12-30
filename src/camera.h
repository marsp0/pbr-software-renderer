#pragma once

#include <stdbool.h>

#include "math.h"
#include "linux/input.h"

typedef struct
{
    float   phi;
    float   theta;
    float   radius;
    vec4_t  position_w;
    vec4_t  target_w;
    vec4_t  forward; // points inward
    vec4_t  left;
    vec4_t  up;

    float   fov_x;
    float   asp_ratio;
    float   n_dist;
    float   f_dist;
    float   t_dist;
    float   b_dist;
    float   l_dist;
    float   r_dist;

} camera_t;

camera_t*   camera_new(vec4_t target,
                       float phi,
                       float theta,
                       float radius,
                       float fov_x,
                       float near,
                       float far,
                       float aspect_ratio);
void        camera_update(camera_t* cam, input_t input, float dt);
mat_t       camera_view_transform(camera_t* cam);
mat_t       camera_proj_transform(camera_t* cam);
void        camera_free(camera_t* cam);