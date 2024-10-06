#pragma once

#include "camera.h"

void        shader_set_camera(camera_t* cam);
vec4_t      shader_vertex(vec4_t v);
uint32_t    shader_fragment();