#pragma once

#include "math.h"
#include "frame_buffer.h"

void rasterize_line(vec_t p1, 
                    vec_t p2, 
                    uint32_t color, 
                    frame_buffer_t* frame_buffer);

void rasterize_triangle();