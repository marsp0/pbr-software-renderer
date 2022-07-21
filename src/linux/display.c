#include "display.h"

#include <assert.h>
#include <X11/Xutil.h>
#include <stdlib.h>

#include "../constants.h"

display* display_new(int width, int height)
{

    /*
     * full documentation on xlib here
     * https://www.x.org/releases/X11R7.7/doc/libX11/libX11/libX11.html
     */

    display* dsp = malloc(sizeof(display));
    dsp->buffer = malloc(sizeof(unsigned char) * width * height * RGB_CHANNELS);

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
    dsp->window = XCreateSimpleWindow(dsp->display, XRootWindow(dsp->display, dsp->screen), 
                                      0, 0, width, height, 0, XWhitePixel(dsp->display, dsp->screen), 
                                      XBlackPixel(dsp->display, dsp->screen));

    /* configure the window */
    XSizeHints* config = XAllocSizeHints();
    config->flags = PMinSize | PMaxSize;
    config->min_width = width;
    config->max_width = width;
    config->min_height = height;
    config->max_height = height;
    XSetWMNormalHints(dsp->display, dsp->window, config);
    XFree(config);

    /* create image that will hold buffer info */
    dsp->ximage = XCreateImage(dsp->display, XDefaultVisual(dsp->display, dsp->screen), 
                               XDefaultDepth(dsp->display, dsp->screen), ZPixmap, 0, 
                               (char*)dsp->buffer, width, height, 8 * RGB_CHANNELS, 0);

    /* event subscription */
    long key_mask = KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask;
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

void display_draw(display* dsp, framebuffer* frame_buffer)
{

}

void display_free(display* dsp)
{
    XUnmapWindow(dsp->display, dsp->window);
    dsp->ximage->data = NULL;
    XDestroyImage(dsp->ximage);
    XCloseDisplay(dsp->display);
    free(dsp->buffer);
    free(dsp);
}