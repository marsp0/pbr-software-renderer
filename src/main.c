#include <unistd.h>
#include <stdlib.h>

#include "renderer.h"
#include "rasterizer.h"
#include "time_utils.h"

int32_t main()
{
    uint32_t width      = 800;
    uint32_t height     = 600;
    const char* scene   = "/home/martin/Documents/Projects/pbr-software-renderer/assets/WaterBottle.glb";

    // initialize
    time_init();
    renderer_init(width, height);
    renderer_load(scene);

    // run
    renderer_run();

    // clean up
    renderer_free();

    return 0;
}
