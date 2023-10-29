#pragma once

#include "math.h"
#include "framebuffer.h"
#include "depthbuffer.h"

void rasterizer_init();

void rasterizer_draw_line(vec_t p0,
                          vec_t p1,
                          uint32_t color,
                          framebuffer_t* framebuffer);

void rasterizer_draw_triangle(vec_t p0,
                              vec_t p1,
                              vec_t p2,
                              uint32_t color,
                              framebuffer_t* framebuffer,
                              depthbuffer_t* depthbuffer);

void rasterizer_free();