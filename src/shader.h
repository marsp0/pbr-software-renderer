#pragma once

#include <stdbool.h>

#include "math.h"
#include "mesh.h"
#include "camera.h"

void shader_set_camera(camera_t* c);
void shader_process_triangle(triangle_t* t);
uint32_t shader_process_fragment(triangle_t* tri, float w0, float w1, float w2);