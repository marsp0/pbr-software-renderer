#pragma once

#include "math.h"
#include "mesh.h"
#include "framebuffer.h"
#include "depthbuffer.h"

void rasterizer_init();

void rasterizer_draw_line(vec_t p0,
                          vec_t p1,
                          uint32_t color,
                          framebuffer_t* framebuffer);

void rasterizer_draw_triangle(triangle_t* triangle,
                              framebuffer_t* framebuffer,
                              depthbuffer_t* depthbuffer);

void rasterizer_free();