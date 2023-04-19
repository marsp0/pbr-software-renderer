#pragma once

#include <stdint.h>

#include "display.h"

#define QUIT 1L << 0

typedef uint64_t input_t;

void handle_input(display_t* dsp, input_t* input);

