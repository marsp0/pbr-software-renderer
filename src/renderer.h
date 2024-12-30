#pragma once

#include <stdbool.h>

#include "scene.h"
#include "framebuffer.h"
#include "depthbuffer.h"
#include "linux/display.h"

void renderer_init();
void renderer_load(const char* file_path);
void renderer_run();
void renderer_free();
