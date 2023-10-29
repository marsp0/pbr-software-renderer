#include <unistd.h>
#include <stdlib.h>

#include "renderer.h"
#include "rasterizer.h"
#include "time_utils.h"

int32_t main()
{
    // initialize
    time_init();
    rasterizer_init();
    renderer_init(800, 600, "/home/martin/Documents/Projects/pbr-software-renderer/assets/WaterBottle.glb");

    // run
    renderer_run();

    // clean up
    renderer_free();
    rasterizer_free();

    return 0;
}