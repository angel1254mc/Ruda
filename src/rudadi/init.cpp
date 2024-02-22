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
    Window xWindow = XCreateSimpleWindow(this->display, this->root, 0, 0, width, height, 0, BlackPixel(this->display, this->screen), BlackPixel(this->display, this->screen));

    cout << "Created Window " << xWindow << endl;
    // Map Window to make it viewable
    XMapWindow(this->display, xWindow);
    XStoreName(this->display, xWindow, title.c_str());
    XFlush(this->display);

    cout << "Made it to post-flush" << endl;
    // Do everything else in window constructor
    // such as:
    // create graphics context
    // set foreground
    // 
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
    // set the foreground to white for now.
    XSetForeground(this->rudaDI->display, gc, WhitePixel(this->rudaDI->display, this->rudaDI->screen));
    // Please please please let me know when
    // Keys are pressed and released
    // Buttons are pressed and released
    // Mouse moves
    // window resizing and such (and some more stuff like Mapping, unmapping, etc.)
    XSelectInput(this->rudaDI->display, this->xWindow, KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask);


}


RudaDIWindow::~RudaDIWindow() {
    XFreeGC(this->rudaDI->display, this->gc);

}

void RudaDI::rudaPollEvents() {
    // The point of polling events is to hit the X Window server 
    // asking it if any events have been queued
    // Then we loop over each event and call the necessary handler
    // The handlers are stored in the RudaDI object, and can be set with
    // These include
    /**
     *      void rudaSetKeyCallback(RudaDIWindow* window,  void(*callbackFunc)(RudaDIWindow, int, int, int, int));
     *      void rudaSetMouseButtonCallback(RudaDIWindow* window, void(*callbackFunc)(RudaDIWindow* window, int button, int action, int mods));
     *      void rudaSetCursorPosCallback(RudaDIWindow* window, void(*callbackFunc)(RudaDIWindow* window, double xpos, double ypos));
    */
}

void RudaDI::processEvent(XEvent* event) {
    // handles the if/else structure to decide what callback to invoke
    switch(event->type) {
        case KeyPress:
            int keycode = event->xkey.keycode;
            bool pressed = true;
            int mods = 0; // Idk even know what mods means in this context
            // [TODO @angel1254mc] investigate 'mods' param
            // Mods apparently describe key combinations, such as ctrl / shift / caps
            // So for instance, you could theoretically pass in mods like 
            // ctrl + shift along with the keycode of v
            // don't think it needs to be implemented for the basic demo but will look into this

            // Trigger the keyCallback with all necessary info
            if (this->keyCallback != nullptr)
                this->keyCallback(this->window, keycode, pressed, mods);
            break;
        case KeyRelease:
            int keycode = event->xkey.keycode;
            bool pressed = false;
            int mods = 0;
            if (this->keyCallback != nullptr)
                this->keyCallback(this->window, keycode, pressed, mods);
            break;
        case ButtonPress:
            // handle left, right, middle click (1, 2, 3 respectively)
            if (this->mouseButtonCallback != nullptr && (event->xbutton.button >= Button1 && event->xbutton.button <= Button3))
                this->mouseButtonCallback(this->window, event->xbutton.button, true, 0);
            // handle scroll
            if (this->scrollCallback != nullptr) {
                if (event->xbutton.button == Button4) {
                    this->scrollCallback(this->window, 0, 1.0);
                } else if (event->xbutton.button == Button5) {
                    this->scrollCallback(this->window, 0, -1.0);
                }
            }
            break;
        case ButtonRelease:
            if (this->mouseButtonCallback != nullptr && (event->xbutton.button >= Button1 && event->xbutton.button <= Button3))
                this->mouseButtonCallback(this->window, event->xbutton.button, false, 0);
            break;
         case MotionNotify:
            int x = event->xmotion.x;
            int y = event->xmotion.y;

            if (this->cursorPosCallback) {
                this->cursorPosCallback(this->window, x, y);
            }
            break;
        case ConfigureNotify:
            // Handle resizes and stuff here.
            break;
    }

}