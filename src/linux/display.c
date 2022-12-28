#include "display.h"

#include <assert.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <stdio.h>

#include "../constants.h"

display_t* display_new(uint32_t width, uint32_t height)
{

    /*
     * full documentation on xlib here
     * https://www.x.org/releases/X11R7.7/doc/libX11/libX11/libX11.html
     */

    display_t* dsp = malloc(sizeof(display_t));
    dsp->buffer = malloc(sizeof(unsigned char) * width * height * RGB_CHANNELS);
    dsp->width = width;
    dsp->height = height;

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
    config->min_width = (int)width;
    config->max_width = (int)width;
    config->min_height = (int)height;
    config->max_height = (int)height;
    XSetWMNormalHints(dsp->display, dsp->window, config);
    XFree(config);

    /* create image that will hold buffer info */
    dsp->ximage = XCreateImage(dsp->display, XDefaultVisual(dsp->display, dsp->screen), 
                               (uint32_t)XDefaultDepth(dsp->display, dsp->screen), ZPixmap, 0, 
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

#if 0
void display_draw(display_t* dsp, const frame_buffer_t* frame_buffer)
{
    /*  
     * Copy contents from framebuffer to ximage
     * framebuffer - RGBA
     * display buffer - BGRA (alpha is not used)
    */
    for (int i = 0; i < dsp->width * RGB_CHANNELS; i += RGB_CHANNELS)
    {
        for (int j = 0; j < dsp->height; j++)
        {
            dsp->buffer[i + j * dsp->width * RGB_CHANNELS + 0] = 255;
        }
    }

    for (int i = 0; i < 20 * RGB_CHANNELS; i += RGB_CHANNELS)
    {
        for (int j = 0; j < 20; j++)
        {
            dsp->buffer[i + j * dsp->width * RGB_CHANNELS + 2] = 255;
        }
    }

    /* show image on display */
    XPutImage(dsp->display, dsp->window, XDefaultGC(dsp->display, dsp->screen), 
              dsp->ximage, 0, 0, 0, 0, dsp->width, dsp->height);
    XFlush(dsp->display);
}
#endif

void display_free(display_t* dsp)
{
    XUnmapWindow(dsp->display, dsp->window);
    dsp->ximage->data = NULL;
    XDestroyImage(dsp->ximage);
    XCloseDisplay(dsp->display);
    free(dsp->buffer);
    free(dsp);
}