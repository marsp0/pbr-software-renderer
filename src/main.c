#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "parsers/glb.h"
#include "linux/display.h"
#include "renderer.h"

int main()
{
    const char* file_name = "/home/martin/Documents/Projects/pbr-software-renderer/assets/WaterBottle.glb";
    renderer_t* renderer = renderer_new(800, 600, file_name);
    renderer->mesh = parse_scene(file_name);

    renderer_free(renderer);

    /*display_t* dsp = display_new(800, 600);
    frame_buffer_t* buf = frame_buffer_new(800, 600);

    XEvent e;

    while (1) {
        
        XNextEvent(dsp->display, &e);

        if (e.type == KeyPress && e.xkey.keycode == 0x09)
            break;
        display_draw(dsp, buf);
    }

    
    display_free(dsp);*/
}