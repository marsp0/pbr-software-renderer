#include "input.h"

#include <math.h>
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
uint64_t keys       = 0;
uint32_t dx         = 0;
uint32_t dy         = 0;

/********************/
/* static functions */
/********************/

static void handle_mouse_motion(display_t* dsp, XEvent* event)
{
    int32_t x = event->xmotion.x;
    int32_t y = event->xmotion.y;
    
    if (!initialized)
    {
        prev_x = x;
        prev_y = y;
        initialized = true;
    }
    
    dx      = x - prev_x;
    dy      = y - prev_y;
    prev_x  = x;
    prev_y  = y;
}

static void handle_mouse_buttons(display_t* dsp, XEvent* event)
{
    int32_t type = event->type;
    uint32_t button = event->xbutton.button;

    if (type == ButtonPress)
    {
        if      (button == Button1) { keys |= LEFT_CLICK; printf("pressed the button -------\n"); }
        else if (button == Button3) { keys |= RIGHT_CLICK; printf("pressed the button -------\n"); }
        else if (button == Button4) { keys |= SCROLL_UP; }
        else if (button == Button5) { keys |= SCROLL_DOWN; }
    }
    else if (type == ButtonRelease)
    {
        if      (button == Button1) { keys ^= LEFT_CLICK; }
        else if (button == Button3) { keys ^= RIGHT_CLICK; }
    }
}

static void handle_keyboard(XEvent* event)
{
    int32_t type = event->type;
    uint32_t button = event->xkey.keycode;

    if (type == KeyPress)
    {
        if (button == X_ESCAPE) { keys |= QUIT; }
    }
    else if (type == KeyRelease)
    {
        if (button == X_ESCAPE) { keys ^= QUIT; }
    }
}

/********************/
/* public functions */
/********************/

input_t handle_input(display_t* dsp)
{
    keys    = keys & ~(SCROLL_UP | SCROLL_DOWN);
    dx      = 0;
    dy      = 0;

    XEvent event;
    XPeekEvent(dsp->display, &event);

    while (event.type != Expose)
    {
        if (event.type == KeyRelease)
        {
            handle_keyboard(&event);
        }
        else if (event.type == MotionNotify)
        {
            handle_mouse_motion(dsp, &event);
        }
        else if (event.type == ButtonPress || event.type == ButtonRelease)
        {
            handle_mouse_buttons(dsp, &event);
        }

        XNextEvent(dsp->display, &event);
        XPeekEvent(dsp->display, &event);
    }

    // pop Expose event
    XNextEvent(dsp->display, &event);

    input_t input = { .keys = keys, .dx = dx, .dy = dy };

    return input;
}