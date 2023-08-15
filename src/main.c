#include <unistd.h>
#include <stdlib.h>

#include "renderer.h"
#include "time_utils.h"

int32_t main()
{
    time_init();

    renderer_t* renderer = renderer_new(800, 600, "./assets/WaterBottle.glb");

    renderer_run(renderer);

    renderer_free(renderer);

    return 0;
}