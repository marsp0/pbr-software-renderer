#pragma once

#include <stdbool.h>

#include "math.h"
#include "mesh.h"
#include "camera.h"

void process_vertex(triangle_t* t, camera_t* c, vec_t* l, uint32_t l_size);
void process_fragment();