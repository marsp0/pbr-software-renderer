#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "parsers/glb.h"
#include "linux/display.h"
#include "renderer.h"

int main()
{
    const char* file_path = "/home/martin/Documents/Projects/pbr-software-renderer/assets/WaterBottle.glb";
    renderer_t* renderer = renderer_new(800, 600, file_path);

    renderer_run(renderer);

    renderer_free(renderer);
}