#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "file_parser.h"
#include "linux/display.h"

int main()
{
    // const char* file_name = "/home/martin/Documents/Projects/pbr-software-renderer/assets/test.obj";
    // parse_obj_scene(file_name);

    display* dsp = display_new(800, 600);

    // xcb_generic_event_t *e;
    // while ((e = xcb_wait_for_event(dsp.conn))) {
    //     switch (e->response_type & ~0x80) {

    //       /* ESC to exit */
    //     case XCB_KEY_PRESS: {
    //       xcb_key_press_event_t* ev;
    //       ev = (xcb_key_press_event_t*)e;
    //       if (ev->detail == 9) return 0;
    //       break;
    //     }
    //     }

    //     printf("dsa\n");

    //     free (e);
    //   }

    
    display_free(dsp);
}