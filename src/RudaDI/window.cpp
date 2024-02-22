#include "../../include/RudaDI/di.h"

DI_Window::DI_Window (Window* xWindow, unsigned int width, unsigned int height, string title) {
	this->xWindow = *xWindow;
	this->width = width;
	this->height = height;
	this->title = title;
	
	// I guess we caan handle actually generating the graphics context and the pixmap here but this can get switched around as needed
	this->context = new RudaContext(XCreateGC(structure->display, this->xWindow, 0, NULL));
	// set the foreground to white for now.
	XSetForeground(structure->display, gc, WhitePixel(structure->display, structure->screen));
	// Please please please let me know when
	// Keys are pressed and released
	// Buttons are pressed and released
	// Mouse moves
	// window resizing and such (and some more stuff like Mapping, unmapping, etc.)
	XSelectInput(structure->display, this->xWindow, KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask);
}

DI_Window::~DI_Window() {
	XFreeGC(structure->display, context->context);
}
