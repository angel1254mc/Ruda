#include "RudaDI/di.h"
#include "RudaDI/di_structs.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#define NIL (0)
// Initializes  


DI_Structure* structure = nullptr;
DI_WindowConfig defaultWindowConfig = DI_WindowConfig();

bool diInit() {
	structure = new DI_Structure(); //global structure referenced everywhere
    structure->display = new DI_Display(XOpenDisplay(NULL));
    structure->screen = DefaultScreen(structure->display);
    structure->xContext = XUniqueContext();
    structure->root = RootWindow(structure->display->xDisplay, structure->screen);
    structure->currentConfig = &defaultWindowConfig;
    
    // initialize variables such as
    // shouldClose
    // isMaximized
    // is Minimized
    // isFullScreen
    // defaultCursor
    // etc.

    // Initialize GLX after all variables have been initialized
    if (!structure->display || (structure->screen == -1) || !structure->root) {
        std::cout << structure->screen << std::endl;
        std::cout << structure->display << std::endl;
        std::cout << structure->root << std::endl;
        throw std::runtime_error("Display, Screen, and/or RootWindow not allocated correctly");
        return false;
    }
    return true;
};

DI_Structure* diGetStructure() {
    return structure;
}

int diWindowHint(unsigned short hint, int value) {
    return structure->currentConfig->diWindowHint(hint, value);
};

DI_Window* diCreateWindow(const str title) {
    return diCreateWindow(title, structure->currentConfig->width, structure->currentConfig->height, nullptr, nullptr);
};

DI_Window* diCreateWindow(const str title, unsigned int width, unsigned int height, DI_Monitor* monitor, DI_Window* parentWindow) {
    XVisual* visual = DefaultVisual(structure->display, structure->screen);
    int depth = DefaultDepth(structure->display, structure->screen);

    // Create Simple Window
    XWindow xWindow = XCreateSimpleWindow(structure->display->xDisplay, structure->root, structure->currentConfig->get_x_position(), structure->currentConfig->get_y_position(), width, height, 0, BlackPixel(structure->display->xDisplay, structure->screen), BlackPixel(structure->display->xDisplay, structure->screen));
    
    // Map Window to make it viewable

    
    XSelectInput(structure->display->xDisplay, xWindow, KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask);

    XMapWindow(structure->display->xDisplay, xWindow);
    XStoreName(structure->display->xDisplay, xWindow, title.c_str());
    XFlush(structure->display->xDisplay);

    
    // Do everything else in window constructor
    // such as:
    // create graphics context
    // set foreground
    
    structure->currentWindow = new DI_Window(xWindow, title, width, height, parentWindow);

    // The reason why XSaveContext matters is because it allows us to FIND our DI_Window object on events
	// For instance, an XEvent will provide the xWindow where the event occured, but not the DI_Window
	// By saving a pointer to our DI_Window alongside the xWindow, we can just call XFindContext(xWindow) to return a pointer
	// to DI_Window
	if (XSaveContext(structure->display->xDisplay,
				xWindow,
				structure->ctx->xStore,
				(XPointer)structure->currentWindow) != 0) {
                    std::cout << "Error saving context" << std::endl;
                } 
    
    structure->windows.push_back(structure->currentWindow);
    return structure->currentWindow;
}
