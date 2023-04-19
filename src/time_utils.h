#pragma once

#include <time.h>

typedef struct timespec timepoint_t; // 16 bytes

timepoint_t time_now();
timepoint_t time_diff(const timepoint_t before, const timepoint_t after);