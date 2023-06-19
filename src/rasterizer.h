#pragma once

#include "math.h"
#include "framebuffer.h"
#include "depthbuffer.h"

void rasterize_line(vec_t p0, 
                    vec_t p1, 
                    uint32_t color, 
                    framebuffer_t* framebuffer);

void rasterize_triangle(vec_t p0,
                        vec_t p1,
                        vec_t p2,
                        uint32_t color, 
                        framebuffer_t* framebuffer,
                        depthbuffer_t* depthbuffer);