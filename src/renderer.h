#pragma once

#include <stdbool.h>

#include "scene.h"
#include "framebuffer.h"
#include "depthbuffer.h"
#include "thread_pool.h"
#include "linux/display.h"

void renderer_init(uint32_t width, uint32_t height, const char* file_path);
void renderer_run();
void renderer_free();
