#pragma once
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
using namespace std;

class RudaDIWindow; // See https://github.com/glfw/glfw/blob/d7e7b164bc0df637d120f1f0543553f454ced091/src/internal.h#L521
class RudaDIMonitor; // See https://github.com/glfw/glfw/blob/d7e7b164bc0df637d120f1f0543553f454ced091/src/internal.h#L582a

class RudaDI {
    Display* display;
    RudaDIWindow window;
    int screen;
    XEvent currEvent;
    int screen;
    bool shouldClose;
public:

    /**
     * Calls "terminate" if necessary, initializes
     * default rudadi configuration and creates all internal
     * memory objects needed to get started w/ creating windows and such
    */
    RudaDI();
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
    void rudaDestroyWindow(RudaDIWindow* window);

    /// @brief Tracks state of whether or not window should close, useful for polling on while loops
    /// @param window window we are checking
    /// @return boolean true or false
    bool rudaWindowShouldClose(RudaDIWindow* window);

    /// @brief Set whether window should close for next poll
    /// @param window window we are setting value for
    /// @param value True for yes, False for no
    void rudaSetWindowShouldClose(RudaDIWindow* window, bool value);

    /// @brief returns title of window
    /// @param window window we are getting title of
    /// @returns title string
    string rudaGetWindowTitle(RudaDIWindow* window);

    /// @brief returns title of window
    /// @param window window we are setting title of
    /// @param title string title we set window to
    void rudaSetWindowTitle(RudaDIWindow* window, string title);

    void rudaSetWindowIcon(RudaDIWindow* window,  string imagePath);
    int* rudaGetWindowPos(RudaDIWindow* window);
    void rudaSetWindowPos(RudaDIWindow* window, int x, int y);
    int* rudaGetWindowSize(RudaDIWindow* window);
    void rudaSetWindowSize(RudaDIWindow* window, int x, int y);
    // https://github.com/glfw/glfw/blob/8f2f766f0d2ed476c03a2ae02e48ac41a9602b03/include/GLFW/glfw3.h#L3700
    
    /// @brief Retrieves the size of the frame of the window, including in-screen coords of each edge of the frame
    /// @param window window to retrieve size for
    /// @return array of int values, corresponding to left, top, right, and bottom edge.
    int* rudaGetWindowFrameSize(RudaDIWindow* window);
    // Minify window
    void rudaIconifyWindow(RudaDIWindow* window);
    // Maximize window
    void rudaMaximizeWindow(RudaDIWindow* window);
    // Restore window to non-maxified non-minified state
    void rudaRestoreWindow(RudaDIWindow* window);

    void rudaShowWindow(RudaDIWindow* window);
    
    void rudaHideWindow(RudaDIWindow* window);

    void rudaFocusWindow(RudaDIWindow* window);

    void rudaRequestWindowAttention(RudaDIWindow* window);

    /// @brief Returns the monitor that the window uses for full screen mode.
    /// @param window window whose monitor handle we are trying to get
    /// @return monitor handle that corresponds to fullscreened window
    RudaDIMonitor rudaGetWindowMonitor(RudaDIWindow window);

    /// @brief Sets the mode, monitor, video mode, and placement of a window that uses fullscreen mode
    /// @param window RudaDI window object
    /// @param monitor RudaDI monitor object
    /// @param xpos x position of window
    /// @param ypos y position of window
    /// @param width width of window (screen coords)
    /// @param height height of window (screen cords_)
    /// @param refreshRate desired refresh rate of mode (optional)
    void rudaSetWindowMonitor(RudaDIWindow* window, GLFWmonitor* monitor, int xpos, int ypos, int width, int height, int refreshRate);

    /// @brief returns the value of an attribute of the specified window or its Ruda context.
    /// @param window window object
    /// @param attribute int/enum representation of Ruda context attribute
    /// @return attribute value ( as an int/enum)
    int rudaGetWindowAttrib(RudaDIWindow* window, int attribute);
    void rudaSetWindowAttrib(RudaDIWindow* window, int attribute, int value);
    /// @brief Sets function as callback on window resizes
    /// @param window window which we are setting callback for
    /// @param callbackFunc the callback func, which is given the width and height of content window post-resize
    void rudaSetWindowSizeCallback(RudaDIWindow* window, void (*callbackFunc)(int, int));
    void rudaSetWindowCloseCallback(RudaDIWindow* window, void (*callbackFunc)());
    void rudaSetWindowFocusCallback(RudaDIWindow* window, void(*callbackFunc)());
    void rudaSetWindowIconifyCallback(RudaDIWindow* window, void(*callbackFunc)(RudaDIWindow, int));
    void rudaSetWindowMaximizeCallback(RudaDIWindow* window, void(*callbackFunc)(RudaDIWindow, int));
    // Pretty sure this is relevant for resizing textures/images attached to a framebuffer since it doesn't get done automatically
    void rudaSetFramebufferSizeCallback(RudaDIWindow* window, void(*callbackFunc)(RudaDIWindow, float, float));
    void rudaSetWindowContenntScaleCallback(RudaDIWindow* window, void(*callbackFunc)(RudaDIWindow, float, float));

    /// @brief processes all pending events in queue (event listening on another thread, implemented by x11, see _glfwPollEventsX11)
    void rudaPollEvents();
    /// @brief Waits for events (with timeout of 1000 by default) and blocks until an event is encountered (then handles them)
    void rudaWaitForEvents();
    /// @brief Waits for events (with customizable timeout and blocks until an event is encountered (then handles them)
    void rudaWaitForEvents(double timeoutMs);
    
    //void rudaPostEmptyEvent();
    //int rudaGetInputMode(RudaDIWindow* window, int mode);
    //void rudaSetInputMode(RudaDIWindow* window, int mode, int value)
    //int rudaRawMouseMotionSupported();
    //string rudaGetKeyName(int key, int scancode);
    //int rudaGetKeyScancode(int key);
    //int rudaGetKey(RudaDIWindow* window, int key);
    //int rudaGgtMouseButton(RudaDIWindow* window, int button);

    /// @brief Retrieves mouse position on window content area (starting from top left)
    /// @param window RudaDIWindow object
    /// @return array of double values, corresponding to xpos and ypos of where the cursor is relative to left edge of content area
    double* rudaGetMouseButton(RudaDIWindow* window);

    RudaDICursor* rudaCreateStandardCursor();
    void rudaDestroyCursor(RudaDICursor* cursor);
    void rudaSetCursor(RudaDIWindow* window, RudaDICursor* cursor);
    /// @brief 
    /// @param window 
    /// @param callbackFunc 
    /// @callback_Signature
    /// func(RudaDIWindow window, int key, int scancode, int action, int mods????)
    void rudaSetKeyCallback(RudaDIWindow* window,  void(*callbackFunc)(RudaDIWindow, int, int, int, int));
    void rudaSetMouseButtonCallback(RudaDIWindow* window, void(*callbackFunc)(RudaDiWindow* window, int button, int action, int mods));
    void rudaSetCursorPosCallback(RudaDIWindow* window, void(*callbackFunc)(RudaDiWindow* window, double xpos, double ypos));
    // void rudaSetCursorEnterCallback(RudaDIWindow* window, void(*callbackFunc)(RudaDiWindow* window, bool entered)));

    // see GLFWscrollfun  callback for more details on the callback params
    void rudaSetScrollCallback(RudaDIWindow* window, void(*callbackFunc)(RudaDiWindow* window, double xoffset, double yoffset));



    // Finally context related functions 🥲🥲🥲
    void rudaMakeContextCurrent(RudaDIWindow* window);
    RudaDIWindow* rudaGetCurrentContext();
    void rudaSwapBuffers(RudaDIWindow* window);
    // This is mainly for vsync. See https://github.com/glfw/glfw/blob/8f2f766f0d2ed476c03a2ae02e48ac41a9602b03/include/GLFW/glfw3.h#L6168
    void rudaSwapInterval(int interval);

}


class RudaDIWindow {
    RudaDIWindow()
}