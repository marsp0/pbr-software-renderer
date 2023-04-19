#include "input.h"

#include <X11/Xlib.h>
#include <stdio.h>

/********************
 *  Notes
 *
 ********************/

/********************/
/*      defines     */
/********************/

#define ESCAPE 0x09

/********************/
/* static variables */
/********************/

/********************/
/* static functions */
/********************/

/********************/
/* public functions */
/********************/

void handle_input(display_t* dsp, input_t* input)
{
	XEvent event;
	XPeekEvent(dsp->display, &event);

	while (event.type != Expose)
	{
		if (event.type == KeyRelease && event.xkey.keycode == ESCAPE)
		{
			*input |= QUIT;
		}
		XNextEvent(dsp->display, &event);
		XPeekEvent(dsp->display, &event);
	}

	// pop Expose event
	XNextEvent(dsp->display, &event);

	printf("finished processing input\n");
}