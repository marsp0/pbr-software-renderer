#include <unistd.h>
#include <stdlib.h>

#include "renderer.h"
#include "rasterizer.h"
#include "time_utils.h"

int32_t main()
{
    const char* scene   = "/home/martin/Documents/Projects/pbr-software-renderer/assets/waterbottle.glb";

    // initialize
    time_init();
    renderer_init();
    renderer_load(scene);

    // run
    renderer_run();

    // clean up
    renderer_free();

    return 0;
}
