#pragma once

#include "camera.h"

#include "texture.h"

void        shader_set_uniforms(camera_t* cam,
                                texture_t* a,
                                texture_t* m,
                                texture_t* n,
                                vec4_t v0, 
                                vec4_t v1, 
                                vec4_t v2,
                                vec2_t t0,
                                vec2_t t1,
                                vec2_t t2);
vec4_t      shader_vertex(vec4_t v);
uint32_t    shader_fragment(float w0, float w1, float w2);
