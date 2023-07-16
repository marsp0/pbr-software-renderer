#pragma once

#include <stdbool.h>

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
    float asp_ratio;
    float n_dist;
    float f_dist;
    float t_dist;
    float b_dist;
    float l_dist;
    float r_dist;

    plane_t n_plane;
    plane_t f_plane;
    plane_t t_plane;
    plane_t b_plane;
    plane_t r_plane;
    plane_t l_plane;

} camera_t;

camera_t*   camera_new(vec_t position,
                       float pitch,
                       float yaw,
                       float fov_x,
                       float near,
                       float far,
                       float aspect_ratio);
void        camera_update(camera_t* cam, input_t input);
bool        camera_is_mesh_visible(camera_t* cam, sphere_t sphere);
mat_t       camera_view_transform(camera_t* cam);
mat_t       camera_proj_transform(camera_t* cam);
void        camera_free(camera_t* cam);