#include "include/RudaDI/rudadi.h"
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
// Initializes  

RudaDI::RudaDI() {
    // Literally do nothing
}
RudaDI::rudaInit() {
    this->display = XOpenDisplay(NULL);
    this->screen = DefaultScreen(*(this->display));
    this->root = RootWindow(this->display, this->screen);
    // initialize variables such as
    // shouldClose
    // isMaximized
    // is Minimized
    // isFullScreen
    // defaultCursor
    // etc.

    // Initialize GLX after all variables have been initialized
    if (!this->display || !this->screen || !this->root) {
        throw std::bad_exception("Display, Screen, and/or RootWindow not allocated correctly")
    }
    
}

Window* RudaDI::rudaCreateWindow(int width, int height, const string title, RudaDIMonitor* monitor, RudaDIWindow* window) {
    Visual* visual = DefaultVisual(this->display, this->screen);
    int depth = DefaultDepth(_glfw.x11.display, _glfw.x11.screen);

    // Create Simple Window
    Window* xWindow = XCreateSimpleWindow(this->display, this->root, 10, 10, width, height, 1, depth, BlackPixel(this->display, this->screen), WhitePixel(this->display, this->screen));

    // Map Window to make it viewable
    XMap(this->display, xWindow);
    
    XChangeProperty(this->display, xWindow, NET_WM_NAME, UTF8.STRING, 8, PropModeReplace);
    XFlush(this->display);

    this->window = new RudaDIWindow(xWindow, width, height, title, this);
}

RudaDI::~RudaDI() {
    delete this->window;
    XFreeDisplay(this->display);
}

RudaDIWindow::RudaDIWindow(Window* window, int width, int height, string title, RudaDI* rudaDILibrary) {
    this->xWindow = window;
    this->width = width;
    this->height = height;
    this->title = title;
    this->rudaDI = rudaDILibrary;

    // I guess we caan handle actually generating the graphics context and the pixmap here but this can get switched around as needed
    this->gc = XCreateGC(this->rudaDI->display, xWindow, 0, NULL);

}


RudaDIWindow::~RudaDIWindow() {
    XFreeGC(this->rudaDI->display, this->gc);

}