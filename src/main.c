#include <unistd.h>
#include <stdlib.h>

#include "renderer.h"
#include "rasterizer.h"
#include "time_utils.h"

int32_t main()
{
    time_init();
    rasterizer_init();

    renderer_t* renderer = renderer_new(800, 600, "./assets/WaterBottle.glb");

    renderer_run(renderer);

    renderer_free(renderer);
    rasterizer_free();

    return 0;
}