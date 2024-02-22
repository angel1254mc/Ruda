#pragma once
#include "../../include/RudaDI/di.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#define NIL (0)
// Initializes  


bool diInit() {
	
    structure->display = XOpenDisplay(NIL);
    structure->screen = DefaultScreen(structure->display);
    structure->root = RootWindow(structure->display, structure->screen);
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

DI_Window* diCreateWindow(unsigned int width, unsigned int height, string title, DI_Monitor* monitor, DI_Window* window) {
    XVisual* visual = DefaultVisual(structure->display, structure->screen);
    int depth = DefaultDepth(structure->display, structure->screen);

    // Create Simple Window
    XWindow xWindow = XCreateSimpleWindow(structure->display, structure->root, 0, 0, width, height, 0, BlackPixel(structure->display, structure->screen), BlackPixel(structure->display, structure->screen));

    std::cout << "Created Window " << xWindow << std::endl;
    
    // Map Window to make it viewable
    
    XMapWindow(structure->display, xWindow);
    XStoreName(structure->display, xWindow, title.c_str());
    XFlush(structure->display);

    std::cout << "Made it to post-flush" << std::endl;
    
    // Do everything else in window constructor
    // such as:
    // create graphics context
    // set foreground
    // 
    structure->window = new DI_Window(&xWindow, width, height, title);

    return structure->window;
}
