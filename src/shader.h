#pragma once

#include <stdbool.h>

#include "math.h"
#include "mesh.h"
#include "camera.h"

void process_vertex(triangle_t* t, camera_t* c);
uint32_t process_fragment(triangle_t* t, float w0, float w1, float w2);