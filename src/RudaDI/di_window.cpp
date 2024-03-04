#include "RudaDI/di.h"

DI_Window::DI_Window (Window xWindow, unsigned int width, unsigned int height, std::string title) {
	this->xWindow = xWindow;
	this->width = width;
	this->height = height;
	this->title = title;
	std::cout << "creating context" << std::endl;
	// I guess we caan handle actually generating the graphics context and the pixmap here but this can get switched around as needed
	this->context = new Ruda_Context(XCreateGC(structure->display->xDisplay, this->xWindow, 0, NULL));
	// set the foreground to white for now.

	std::cout << "created context" << std::endl;

	XSetForeground(structure->display->xDisplay, this->context->xContext, WhitePixel(structure->display->xDisplay, structure->screen));

	std::cout << "set foreground" << std::endl;
	// Please please please let me know when
	// Keys are pressed and released
	// Buttons are pressed and released
	// Mouse moves
	// window resizing and such (and some more stuff like Mapping, unmapping, etc.)
	XSelectInput(structure->display->xDisplay, this->xWindow, KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask);
}

DI_Window::~DI_Window() {
	XFreeGC(structure->display->xDisplay, this->context->xContext);
}
