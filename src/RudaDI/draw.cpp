#include "RudaDI/di.h"
#include "RudaDI/di_structs.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>


void diDrawLine (unsigned int x_origin, unsigned int y_origin, unsigned int x_dest, unsigned int y_dest, Color color) {
	Colormap colormap = DefaultColormap(structure->display->xDisplay, 0);
 	XAllocColor(structure->display->xDisplay, colormap, &color.xColor); 
 	XSetForeground(structure->display->xDisplay, structure->currentWindow->context->xContext, color.xColor.pixel);
	XDrawLine(structure->display->xDisplay, structure->currentWindow->xWindow, structure->currentWindow->context->xContext, x_origin, y_origin, x_dest, y_dest);
	XFreeColors(structure->display->xDisplay, colormap, &color.xColor.pixel, 1, 0);
}

void diDrawLine (unsigned int x_origin, unsigned int y_origin, unsigned int x_dest, unsigned int y_dest, unsigned int r, unsigned int g, unsigned int b) {
    diDrawLine(x_origin, y_origin, x_dest, y_dest, Color(r, g, b));
}
void diDrawRectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, Color color) {
	Colormap colormap = DefaultColormap(structure->display->xDisplay, 0);
 	XAllocColor(structure->display->xDisplay, colormap, &color.xColor); 
	XSetForeground(structure->display->xDisplay, structure->currentWindow->context->xContext, color.xColor.pixel);
	XFillRectangle(structure->display->xDisplay, structure->currentWindow->xWindow, structure->currentWindow->context->xContext, x, y, width, height);
	XFreeColors(structure->display->xDisplay, colormap, &color.xColor.pixel, 1, 0);
}

void diDrawRectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int r, unsigned int g, unsigned int b) {
	diDrawRectangle(x, y, width, height, Color(r, g, b));
}

void diClearWindow() {
    // Clear the window by drawing its background color
    XClearWindow(structure->display->xDisplay, structure->currentWindow->xWindow);
	Colormap colormap = DefaultColormap(structure->display->xDisplay, 0);
	XColor color;
    color.red = 0;
    color.green = 0;
    color.blue = 0;
    XAllocColor(structure->display->xDisplay, colormap, &color);
    XSetWindowBackground(structure->display->xDisplay, structure->currentWindow->xWindow, color.pixel);
    XClearWindow(structure->display->xDisplay, structure->currentWindow->xWindow);
	XFreeColors(structure->display->xDisplay, colormap, &color.pixel, 1, 0);
}
void diFlush() {
	XSync(structure->display->xDisplay, false);
}