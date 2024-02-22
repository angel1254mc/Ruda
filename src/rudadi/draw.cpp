#pragma once
#include "../../include/RudaDI/rudadi.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>


void diDrawLine (unsigned int x_origin, unsigned int y_origin, unsigned int x_dest, unsigned int y_dest, Color color) {
	//XSetForeground
	XDrawLine(rudaDI->display, window->xWindow, window->gc, 0, 0, 300, 300);
}
