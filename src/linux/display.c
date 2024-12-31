#include "display.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <X11/Xutil.h>

#include "../rasterizer_constants.h"
#include "../constants.h"

/********************
 *  Notes
 *
 ********************/

/********************/
/*      defines     */
/********************/

/********************/
/* static variables */
/********************/

/********************/
/* static functions */
/********************/

/********************/
/* public functions */
/********************/

display_t* display_new()
{

    /*
     * full documentation on xlib here
     * https://www.x.org/releases/X11R7.7/doc/libX11/libX11/libX11.html
     */

    display_t* dsp = malloc(sizeof(display_t));
    dsp->buffer = malloc(sizeof(unsigned char) * WINDOW_WIDTH * WINDOW_HEIGHT * RGB_CHANNELS);

    /*
     * The XOpenDisplay function returns a Display structure that serves as the connection to the X server and 
     * that contains all the information about that X server. XOpenDisplay connects your application to the X server 
     * through TCP or DECnet communications protocols, or through some local inter-process communication protocol. 
     */
    dsp->display = XOpenDisplay(NULL);
    assert(dsp->display != NULL);

    /* get screen number */
    dsp->screen = XDefaultScreen(dsp->display);

    /*
     * The XCreateSimpleWindow function creates an unmapped InputOutput subwindow for a specified parent window, 
     * returns the window ID of the created window
     */
    dsp->window = XCreateSimpleWindow(dsp->display,
                                      XRootWindow(dsp->display, dsp->screen),
                                      0,
                                      0,
                                      WINDOW_WIDTH,
                                      WINDOW_HEIGHT,
                                      0,
                                      XWhitePixel(dsp->display, dsp->screen),
                                      XBlackPixel(dsp->display, dsp->screen));

    /* configure the window */
    XSizeHints* config = XAllocSizeHints();
    config->flags = PMinSize | PMaxSize;
    config->min_width = (int)WINDOW_WIDTH;
    config->max_width = (int)WINDOW_WIDTH;
    config->min_height = (int)WINDOW_HEIGHT;
    config->max_height = (int)WINDOW_HEIGHT;
    XSetWMNormalHints(dsp->display, dsp->window, config);
    XFree(config);

    /* create image that will hold buffer info */
    dsp->ximage = XCreateImage(dsp->display,
                               XDefaultVisual(dsp->display, dsp->screen),
                               (uint32_t)XDefaultDepth(dsp->display, dsp->screen),
                               ZPixmap,
                               0,
                               (char*)dsp->buffer,
                               WINDOW_WIDTH,
                               WINDOW_HEIGHT,
                               8 * RGB_CHANNELS,
                               0);

    /* event subscription */
    long key_mask = ExposureMask | \
                    KeyReleaseMask | \
                    KeyPressMask | \
                    ButtonReleaseMask | \
                    ButtonPressMask | \
                    PointerMotionMask;
    XSelectInput(dsp->display, dsp->window, key_mask);

    /*
     * The XMapWindow function maps the window and all of its subwindows that have had map requests.
     * If the window is an InputOutput window, XMapWindow generates Expose events on each InputOutput 
     * window that it causes to be displayed
     */
    XMapWindow(dsp->display, dsp->window);
    XFlush(dsp->display);
    
    return dsp;
}

void display_draw(display_t* dsp, const framebuffer_t* framebuffer)
{   
    // NOTE: not sure if directly memcpy-ing here is the best thing to do
    //       What if the framebuffer changes the way it stores the color data?
    //       Perhaps a method in framebuffer.c would be better.
    //       Something like void framebuffer_copy(dst, scheme) ?
    memcpy(dsp->buffer,
           framebuffer->data,
           WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(unsigned char) * RGB_CHANNELS);

    /* show image on display */
    XPutImage(dsp->display,
              dsp->window,
              XDefaultGC(dsp->display, dsp->screen),
              dsp->ximage,
              0,
              0,
              0,
              0,
              WINDOW_WIDTH,
              WINDOW_HEIGHT);

    XFlush(dsp->display);
}

void display_clear(display_t* dsp)
{
    XClearArea(dsp->display, dsp->window, 0, 0, 1, 1, true);
    XFlush(dsp->display);
}

void display_free(display_t* dsp)
{
    XUnmapWindow(dsp->display, dsp->window);

    dsp->ximage->data = NULL;
    XDestroyImage(dsp->ximage);
    XCloseDisplay(dsp->display);

    free(dsp->buffer);
    free(dsp);
}