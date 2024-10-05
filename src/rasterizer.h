#pragma once

#include "math.h"
#include "framebuffer.h"
#include "depthbuffer.h"

void rasterize_line(vec4_t p0, 
                    vec4_t p1, 
                    uint32_t color, 
                    framebuffer_t* framebuffer);

void rasterize_triangle(vec4_t p0,
                        vec4_t p1,
                        vec4_t p2,
                        uint32_t color, 
                        framebuffer_t* framebuffer,
                        depthbuffer_t* depthbuffer);