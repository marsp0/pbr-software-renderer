#pragma once

#include <stddef.h>
#include "../texture.h"

texture_t* parse_png(const unsigned char* buffer, uint32_t size);