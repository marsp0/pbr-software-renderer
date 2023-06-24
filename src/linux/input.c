#include "input.h"

#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <X11/Xlib.h>

/********************
 *  Notes
 *
 ********************/

/********************/
/*      defines     */
/********************/

#define X_ESCAPE 0x09
#define X_1      0x0a
#define X_2      0x0b
#define X_3      0x0c
#define X_4      0x0d
#define X_W      0x19
#define X_A      0x26
#define X_S      0x27
#define X_D      0x28       

/********************/
/* static variables */
/********************/

bool initialized    = false;
int32_t prev_x      = 0;
int32_t prev_y      = 0;

/********************/
/* static functions */
/********************/

static void handle_mouse(display_t* dsp, XEvent* event, input_t* input)
{
    int32_t x       = event->xmotion.x;
    int32_t y       = event->xmotion.y;
    int32_t width   = (int32_t)dsp->width;
    int32_t height  = (int32_t)dsp->height;
    
    if (!initialized)
    {
        prev_x = x;
        prev_y = y;
        initialized = true;
    }

    int32_t diff_x = (width >> 1) - x;
    int32_t diff_y = (height >> 1) - y;
    if (fabs((float)diff_x) > 100 || fabs((float)diff_y) > 100)
    {
        XWarpPointer(dsp->display, 
                     0,
                     dsp->window,
                     0,
                     0,
                     0,
                     0,
                     (int32_t)dsp->width >> 1,
                     (int32_t)dsp->height >> 1);
        initialized = false;
        return;
    }
    
    input->dx   = x - prev_x;
    input->dy   = y - prev_y;
    prev_x      = x;
    prev_y      = y;
}

static void handle_keyboard(XEvent* event, input_t* input)
{
    switch (event->xkey.keycode)
    {
        case X_ESCAPE:
            input->keys |= QUIT;
            break;
        case X_1:
            input->keys |= KEY_1;
            break;
        case X_2:
            input->keys |= KEY_2;
            break;
        case X_3:
            input->keys |= KEY_3;
            break;
        case X_4:
            input->keys |= KEY_4;
            break;
        case X_W:
            input->keys |= KEY_W;
            break;
        case X_A:
            input->keys |= KEY_A;
            break;
        case X_S:
            input->keys |= KEY_S;
            break;
        case X_D:
            input->keys |= KEY_D;
            break;
    }
}

/********************/
/* public functions */
/********************/

input_t handle_input(display_t* dsp)
{
    input_t input = { .keys = 0, .dx= 0, .dy= 0 };
    XEvent event;
    XPeekEvent(dsp->display, &event);

    while (event.type != Expose)
    {
        if (event.type == KeyRelease)
        {
            handle_keyboard(&event, &input);
        }
        else if (event.type == MotionNotify)
        {
            handle_mouse(dsp, &event, &input);
        }

        XNextEvent(dsp->display, &event);
        XPeekEvent(dsp->display, &event);
    }

    // pop Expose event
    XNextEvent(dsp->display, &event);

    return input;
}