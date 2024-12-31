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

XEvent event        = { 0 };
bool initialized    = false;
int32_t prev_x      = 0;
int32_t prev_y      = 0;
uint64_t keys       = 0;


/********************/
/* static functions */
/********************/

static void handle_mouse_motion(input_t* input)
{
    int32_t x = event.xmotion.x;
    int32_t y = event.xmotion.y;
    
    if (!initialized)
    {
        initialized = true;
        prev_x = x;
        prev_y = y;
    }
    
    input->curr_x   = x;
    input->curr_y   = y;
    input->prev_x   = prev_x;
    input->prev_y   = prev_y;

    prev_x          = x;
    prev_y          = y;
}

static void handle_mouse_buttons()
{
    int32_t type = event.type;
    uint32_t button = event.xbutton.button;

    if (type == ButtonPress)
    {
        if      (button == Button1) { keys |= LEFT_CLICK; }
        else if (button == Button3) { keys |= RIGHT_CLICK; }
        else if (button == Button4) { keys |= SCROLL_UP; }
        else if (button == Button5) { keys |= SCROLL_DOWN; }
    }
    else if (type == ButtonRelease)
    {
        if      (button == Button1) { keys ^= LEFT_CLICK; }
        else if (button == Button3) { keys ^= RIGHT_CLICK; }
    }
}

static void handle_keyboard()
{
    int32_t type = event.type;
    uint32_t button = event.xkey.keycode;

    if (type == KeyPress)
    {
        if      (button == X_ESCAPE)    { keys |= QUIT; }
        else if (button == X_1)         { keys |= KEY_1; }
    }
    else if (type == KeyRelease)
    {
        if      (button == X_ESCAPE)    { keys ^= QUIT; }
        else if (button == X_1)         { keys ^= KEY_1; }
    }
}

/********************/
/* public functions */
/********************/

input_t handle_input(display_t* dsp)
{
    keys            = keys & ~((uint64_t)(SCROLL_UP | SCROLL_DOWN));
    input_t input   = { 0 };

    XPeekEvent(dsp->display, &event);

    while (event.type != Expose)
    {
        if      (event.type == KeyPress || event.type == KeyRelease)        { handle_keyboard(); }
        else if (event.type == MotionNotify)                                { handle_mouse_motion(&input); }
        else if (event.type == ButtonPress || event.type == ButtonRelease)  { handle_mouse_buttons(); }

        XNextEvent(dsp->display, &event);
        XPeekEvent(dsp->display, &event);
    }

    // pop Expose event
    XNextEvent(dsp->display, &event);

    input.keys = keys;

    return input;
}