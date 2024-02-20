#pragma once
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
using namespace std;

class RudaDIWindow;
class RudaDIMonitor;
class RudaDIError;

class RudaDI {
    Display *display;
    RudaDIWindow window;
    XEvent currEvent;
    int screen;
    bool shouldClose;
public:

    /**
     * Calls "terminate" if necessary, initializes
     * default rudadi configuration and creates all internal
     * memory objects needed to get started w/ creating windows and such
    */
    bool rudaInit();
    bool rudaTerminate();
    void rudaInitHint();

    RudaDIMonitor rudaGetMonitors(int* count);
    RudaDIMonitor rudaGetPrimaryMonitor(int* count);
    
    /// @brief Resets all window hints to default values
    void rudaDefaultWindowHints();
    /// @brief Sets specific window hint to value
    /// @param hint integer/enum representation of hint to set
    /// @param value integer/enum representation of hint value
    void rudaWindowHint(int hint, int value);
    
    /// @brief Sets specific window hint to value
    /// @param width width of window
    /// @param height height of window
    /// @param title string title of window
    /// @param monitor Monitor on which to create window
    /// @param window OPTIONAL additional window to share resources with
    /// @returns RudaDIWindow object corresponding to newly created window
    RudaDIWindow rudaCreateWindow(int width, int height, const string title, RudaDIMonitor* monitor, RudaDIWindow* window);

    /// @brief Destroys window and associated resources
    /// @param window RudaDIWindow object to destroy
    void rudaDestroyWindow(RudaDIWindow window);

    /// @brief Tracks state of whether or not window should close, useful for polling on while loops
    /// @param window window we are checking
    /// @return boolean true or false
    bool rudaWindowShouldClose(RudaDIWindow window);

    /// @brief Set whether window should close for next poll
    /// @param window window we are setting value for
    /// @param value True for yes, False for no
    void rudaSetWindowShouldClose(RudaDIWindow window, bool value);

    /// @brief returns title of window
    /// @param window window we are getting title of
    /// @returns title string
    string rudaGetWindowTitle(RudaDIWindow window);

    /// @brief returns title of window
    /// @param window window we are setting title of
    /// @param title string title we set window to
    void rudaSetWindowTitle(RudaDIWindow window, string title);

    void rudaSetWindowIcon(RudaDIWindow window,  string imagePath);
    int* rudaGetWindowPos(RudaDIWindow window);
    void rudaSetWindowPos(RudaDIWindow window, int x, int y);
    int* rudaGetWindowSize(RudaDIWindow window);
    void rudaSetWindowSize(RudaDIWindow window, int x, int y);
}
