#pragma once

#include <stdbool.h>

#include "scene.h"
#include "framebuffer.h"
#include "depthbuffer.h"
#include "linux/display.h"

void renderer_init(const uint32_t w, const uint32_t h);
void renderer_load(const char* file_path);
void renderer_run();
void renderer_free();
