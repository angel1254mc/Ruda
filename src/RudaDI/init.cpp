#include "RudaDI/di.h"

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

bool diInit() {
	structure = new DI_Structure(); //global structure referenced everywhere
    structure->display = new DI_Display(XOpenDisplay(NIL));
    structure->screen = DefaultScreen(structure->display);
    structure->xContext = XUniqueContext();
    print("setting root...");
    structure->root = RootWindow(structure->display->xDisplay, structure->screen);
    print("root set");
    
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
    
}

DI_Window* diCreateWindow(const str title, unsigned int width, unsigned int height, DI_Monitor* monitor, DI_Window* parentWindow) {
    XVisual* visual = DefaultVisual(structure->display, structure->screen);
    int depth = DefaultDepth(structure->display, structure->screen);

    // Create Simple Window
    XWindow xWindow = XCreateSimpleWindow(structure->display->xDisplay, structure->root, 0, 0, width, height, 0, BlackPixel(structure->display->xDisplay, structure->screen), BlackPixel(structure->display->xDisplay, structure->screen));

    std::cout << "Created Window " << xWindow << std::endl;
    
    // Map Window to make it viewable
    
    XMapWindow(structure->display->xDisplay, xWindow);
    XStoreName(structure->display->xDisplay, xWindow, title.c_str());
    XFlush(structure->display->xDisplay);

    print("Made it to post-flush");
    
    // Do everything else in window constructor
    // such as:
    // create graphics context
    // set foreground
    
    structure->currentWindow = new DI_Window(xWindow, title, width, height, parentWindow);

    // The reason why XSaveContext matters is because it allows us to FIND our DI_Window object on events
	// For instance, an XEvent will provide the xWindow where the event occured, but not the DI_Window
	// By saving a pointer to our DI_Window alongside the xWindow, we can just call XFindContext(xWindow) to return a pointer
	// to DI_Window
	XSaveContext(structure->display->xDisplay,
				xWindow,
				structure->xContext,
				(XPointer)&structure->currentWindow);
    
    print("set currentWindow");
    structure->windows.push_back(structure->currentWindow);
    return structure->currentWindow;
}
