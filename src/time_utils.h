#pragma once

#include <time.h>
#include <stdint.h>

#define SECOND      1000000000
#define MILLISECOND 1000000
#define MICROSECOND 1000

typedef int64_t timestamp_t;

void        time_init();
timestamp_t time_now();