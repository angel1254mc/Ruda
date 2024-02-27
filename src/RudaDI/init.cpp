#pragma once
#include "../../include/Ruda/RudaDI/di_structs.h"
#include "../../include/Ruda/RudaDI/di.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#define NIL (0)

DI_Structure* structure;

static XWindow createHelperWindow() {
    XSetWindowAttributes wa;
    wa.event_mask = PropertyChangeMask;

    return XCreateWindow(
        structure->display,
        structure->root, 
        0, 0, 1, 1, 0, 0,
        InputOnly,
        DefaultVisual(structure->display, structure->screen),
        CWEventMask, &wa);
};

bool diInit() {

    if (structure != nullptr && structure->initialized) {
        return true;
    }

    structure = new DI_Structure();
	
    structure->display = XOpenDisplay(NIL);
    structure->screen = DefaultScreen(structure->display);
    structure->root = RootWindow(structure->display, structure->screen);
    structure->context = XUniqueContext();

    // getSystemContentScale here https://github.com/glfw/glfw/blob/7b6aead9fb88b3623e3b3725ebb42670cbe4c579/src/x11_init.c#L1528

    // Initialize Extensions 

    structure->helperWindowHandle = createHelperWindow();
    
    // Create a hidden cursor
    //structure->hiddenCursorHandle = createHiddenCursor();

    // In a more  complicated application, we would have accounted for additional languages and keys using an InputContext and an XIM.
    // However, since we're almost exclusively using English we can simply use the default Event Masks available for use with 
    // SelectInput :-)

    // Initialize GLX after all variables have been initialized
    if (!structure->display || (structure->screen == -1) || !structure->root) {
        std::cout << structure->screen << std::endl;
        std::cout << structure->display << std::endl;
        std::cout << structure->root << std::endl;
        throw std::runtime_error("Display, Screen, and/or RootWindow not allocated correctly");
        return false;
    }

    // handle multi-monitor initialization here. This would typically involve using
    // RandR to setup a big virtual "monitor" representation across all connected monitors
    // See _glfwPollMonitors in GLFW for a good idea of how this works https://github.com/glfw/glfw/blob/master/src/x11_monitor.c#L101

    // Finally, initialize Hints. Hints afaik are just ways of telling the library prior to initialization of a window what we want the
    // Window to look and act like (should it be resizable, decorated, transparent, etc)
    // These hints should be stored in the windowConfig, and framebufferConfig

    

    structure->initialized = true;
    return true;
    
};

bool diTerminate() {
    // DI_Structure destructor handles deletion of windows,
    // monitors, Freeing of connections
    delete structure;
}
