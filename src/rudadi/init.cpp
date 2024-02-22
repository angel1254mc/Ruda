#pragma once
#include "../../include/RudaDI/rudadi.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
using namespace std;
#define NIL (0)
// Initializes  

RudaDI::RudaDI() {
    // Literally do nothing
}
bool RudaDI::rudaInit() {
    this->display = XOpenDisplay(NIL);
    this->screen = DefaultScreen(this->display);
    this->root = RootWindow(this->display, this->screen);
    // initialize variables such as
    // shouldClose
    // isMaximized
    // is Minimized
    // isFullScreen
    // defaultCursor
    // etc.

    // Initialize GLX after all variables have been initialized
    if (!this->display || (this->screen == -1) || !this->root) {
        cout << this->screen << endl;
        cout << this->display << endl;
        cout << this->root << endl;
        throw runtime_error("Display, Screen, and/or RootWindow not allocated correctly");
        return false;
    }

    return true;
    
}

RudaDIWindow* RudaDI::rudaCreateWindow(int width, int height, string title, RudaDIMonitor* monitor, RudaDIWindow* window) {
    Visual* visual = DefaultVisual(this->display, this->screen);
    int depth = DefaultDepth(this->display, this->screen);

    // Create Simple Window
    Window xWindow = XCreateSimpleWindow(this->display, this->root, 0, 0, width, height, 0, BlackPixel(this->display, this->screen), WhitePixel(this->display, this->screen));

    cout << "Created Window " << xWindow << endl;
    // Map Window to make it viewable
    XMapWindow(this->display, xWindow);
    
    XStoreName(this->display, xWindow, title.c_str());
    XFlush(this->display);
    cout << "Made it to post-flush" << endl;
    this->window = new RudaDIWindow(&xWindow, width, height, title, this);

    return this->window;
}

RudaDI::~RudaDI() {
    delete &(this->window);
    XFree(this->display);
}

RudaDIWindow::RudaDIWindow(Window* window, int width, int height, string title, RudaDI* rudaDILibrary) {
    this->xWindow = *window;
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