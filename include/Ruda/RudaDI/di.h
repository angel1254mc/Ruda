#pragma once
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include "../Util/color.h"
#include "../Ruda/ruda.h"
#include "../Ruda/ruda.h"
#include "../Ruda/ruda.h"
#include "di_structs.h"
	
	
	//TO DO:
	
	//void diInitHint(); Sets hints for initialization
	//void diInitHint(); Sets hints for initialization
	//bool diTerminate();
	
	/* Callback functions to be implemented at a later date
	void (*windowResizeCallback)(int, int) = nullptr;
    void (*windowClosCallback)() = nullptr;
    void (*windowFocusCallback)() = nullptr;
    void (*windowIconifyCallback)(DI_Window*, int) = nullptr;
    void (*windowIconifyCallback)(DI_Window*, int) = nullptr;
    void (*windowMaximizeCallback)(DI_Window*, int) = nullptr;
    void (*windowMaximizeCallback)(DI_Window*, int) = nullptr;
    void (*windowFramebufferSizeCallback)(DI_Window*, float, float) = nullptr;
    void (*windowFramebufferSizeCallback)(DI_Window*, float, float) = nullptr;
    void (*windowContentScaleCallback)(DI_Window*, float, float) = nullptr;
    void (*windowContentScaleCallback)(DI_Window*, float, float) = nullptr;

    
    void (*keyCallback)(DI_Window*, int, int, int) = nullptr;
    void (*keyCallback)(DI_Window*, int, int, int) = nullptr;

    void (*mouseButtonCallback)(DI_Window* window, int button, bool pressed, int mods);
    void (*mouseButtonCallback)(DI_Window* window, int button, bool pressed, int mods);
    void (*cursorPosCallback)(DI_Window* window, double xpos, double ypos);
    void (*cursorPosCallback)(DI_Window* window, double xpos, double ypos);
    void (*scrollCallback)(DI_Window* window, double xoffset, double yoffset);
    void (*scrollCallback)(DI_Window* window, double xoffset, double yoffset);
	*/
    /**
     * Calls "terminate" if necessary, initializes
     * default DI_ configuration and creates all internal
     * default DI_ configuration and creates all internal
     * memory objects needed to get started w/ creating windows and such
    */

	static DI_Structure structure = DI_Structure(); //global structure referenced everywhere

	bool diInit();
    

    DI_Monitor diGetMonitors(int* count);
    DI_Monitor diGetPrimaryMonitor(int* count);
    
    /// @brief Resets all window hints to default values
    void diDefaultWindowHints();
    /// @brief Sets specific window hint to value
    /// @param hint integer/enum representation of hint to set
    /// @param value integer/enum representation of hint value
    void diWindowHint(int hint, int value);
    
    /// @brief Sets specific window hint to value
    /// @param width width of window
    /// @param height height of window
    /// @param title string title of window
    /// @param monitor Monitor on which to create window
    /// @param window OPTIONAL additional window to share resources with
    /// @returns DI_Window object corresponding to newly created window
    /// @returns DI_Window object corresponding to newly created window
    DI_Window* diCreateWindow(unsigned int width, unsigned int height, const string title, DI_Monitor* monitor, DI_Window* window);

    /// @brief Destroys window and associated resources
    /// @param window DI_Window object to destroy
    /// @param window DI_Window object to destroy
    void diDestroyWindow(DI_Window* window);
    void diDestroyWindow(DI_Window* window);

    /// @brief Tracks state of whether or not window should close, useful for polling on while loops
    /// @param window window we are checking
    /// @return boolean true or false
    bool diWindowShouldClose(DI_Window* window);
    bool diWindowShouldClose(DI_Window* window);

    /// @brief Set whether window should close for next poll
    /// @param window window we are setting value for
    /// @param value True for yes, False for no
    void diSetWindowShouldClose(DI_Window* window, bool value);
    void diSetWindowShouldClose(DI_Window* window, bool value);

    /// @brief returns title of window
    /// @param window window we are getting title of
    /// @returns title string
    string diGetWindowTitle(DI_Window* window);
    string diGetWindowTitle(DI_Window* window);

    /// @brief returns title of window
    /// @param window window we are setting title of
    /// @param title string title we set window to
    void diSetWindowTitle(DI_Window* window, string title);
    void diSetWindowTitle(DI_Window* window, string title);

    void diSetWindowIcon(DI_Window* window,  string imagePath);
    void diSetWindowIcon(DI_Window* window,  string imagePath);
    int* diGetWindowPos(DI_Window* window);
    int* diGetWindowPos(DI_Window* window);
    void diSetWindowPos(DI_Window* window, int x, int y);
    void diSetWindowPos(DI_Window* window, int x, int y);
    int* diGetWindowSize(DI_Window* window);
    int* diGetWindowSize(DI_Window* window);
    void diSetWindowSize(DI_Window* window, int x, int y);
    void diSetWindowSize(DI_Window* window, int x, int y);
    // https://github.com/glfw/glfw/blob/8f2f766f0d2ed476c03a2ae02e48ac41a9602b03/include/GLFW/glfw3.h#L3700
    
    /// @brief Retrieves the size of the frame of the window, including in-screen coords of each edge of the frame
    /// @param window window to retrieve size for
    /// @return array of int values, corresponding to left, top, right, and bottom edge.
    int* diGetWindowFrameSize(DI_Window* window);
    int* diGetWindowFrameSize(DI_Window* window);
    // Minify window
    void diIconifyWindow(DI_Window* window);
    void diIconifyWindow(DI_Window* window);
    // Maximize window
    void diMaximizeWindow(DI_Window* window);
    void diMaximizeWindow(DI_Window* window);
    // Restore window to non-maxified non-minified state
    void diRestoreWindow(DI_Window* window);
    void diRestoreWindow(DI_Window* window);

    void diShowWindow(DI_Window* window);
    void diShowWindow(DI_Window* window);

    void diHideWindow(DI_Window* window);
    void diHideWindow(DI_Window* window);

    void diFocusWindow(DI_Window* window);
    void diFocusWindow(DI_Window* window);

    void diRequestWindowAttention(DI_Window* window);
    void diRequestWindowAttention(DI_Window* window);

    /// @brief Returns the monitor that the window uses for full screen mode.
    /// @param window window whose monitor handle we are trying to get
    /// @return monitor handle that corresponds to fullscreened window
    DI_Monitor diGetWindowMonitor(DI_Window window);
    DI_Monitor diGetWindowMonitor(DI_Window window);
    DI_Monitor diGetWindowMonitor(DI_Window window);

    /// @brief Sets the mode, monitor, video mode, and placement of a window that uses fullscreen mode
    /// @param window DI_ window object
    /// @param window DI_ window object
    /// @param monitor DI_ monitor object
    /// @param monitor DI_ monitor object
    /// @param xpos x position of window
    /// @param ypos y position of window
    /// @param width width of window (screen coords)
    /// @param height height of window (screen cords_)
    /// @param refreshRate desired refresh rate of mode (optional)
    void diSetWindowMonitor(DI_Window* window, DI_Monitor* monitor, int xpos, int ypos, int width, int height, int refreshRate);
    void diSetWindowMonitor(DI_Window* window, DI_Monitor* monitor, int xpos, int ypos, int width, int height, int refreshRate);
    void diSetWindowMonitor(DI_Window* window, DI_Monitor* monitor, int xpos, int ypos, int width, int height, int refreshRate);

    /// @brief returns the value of an attribute of the specified window or its di context.
    /// @brief returns the value of an attribute of the specified window or its di context.
    /// @param window window object
    /// @param attribute int/enum representation of Ruda context attribute
    /// @param attribute int/enum representation of Ruda context attribute
    /// @return attribute value ( as an int/enum)
    int diGetWindowAttrib(DI_Window* window, int attribute);
    int diGetWindowAttrib(DI_Window* window, int attribute);
    void diSetWindowAttrib(DI_Window* window, int attribute, int value);
    void diSetWindowAttrib(DI_Window* window, int attribute, int value);
    /// @brief Sets function as callback on window resizes
    /// @param window window which we are setting callback for
    /// @param callbackFunc the callback func, which is given the width and height of content window post-resize
    void diSetWindowSizeCallback(DI_Window* window, void (*callbackFunc)(int, int));
    void diSetWindowSizeCallback(DI_Window* window, void (*callbackFunc)(int, int));
    void diSetWindowSizeCallback(DI_Window* window, void (*callbackFunc)(int, int));
    void diSetWindowCloseCallback(DI_Window* window, void (*callbackFunc)());
    void diSetWindowCloseCallback(DI_Window* window, void (*callbackFunc)());
    void diSetWindowCloseCallback(DI_Window* window, void (*callbackFunc)());
    void diSetWindowFocusCallback(DI_Window* window, void(*callbackFunc)());
    void diSetWindowFocusCallback(DI_Window* window, void(*callbackFunc)());
    void diSetWindowFocusCallback(DI_Window* window, void(*callbackFunc)());
    void diSetWindowIconifyCallback(DI_Window* window, void(*callbackFunc)(DI_Window*, int));
    void diSetWindowIconifyCallback(DI_Window* window, void(*callbackFunc)(DI_Window*, int));
    void diSetWindowIconifyCallback(DI_Window* window, void(*callbackFunc)(DI_Window*, int));
    void diSetWindowIconifyCallback(DI_Window* window, void(*callbackFunc)(DI_Window*, int));
    void diSetWindowMaximizeCallback(DI_Window* window, void(*callbackFunc)(DI_Window*, int));
    void diSetWindowMaximizeCallback(DI_Window* window, void(*callbackFunc)(DI_Window*, int));
    void diSetWindowMaximizeCallback(DI_Window* window, void(*callbackFunc)(DI_Window*, int));
    void diSetWindowMaximizeCallback(DI_Window* window, void(*callbackFunc)(DI_Window*, int));
    // Pretty sure this is relevant for resizing textures/images attached to a framebuffer since it doesn't get done automatically
    void diSetFramebufferSizeCallback(DI_Window* window, void(*callbackFunc)(DI_Window*, float, float));
    void diSetFramebufferSizeCallback(DI_Window* window, void(*callbackFunc)(DI_Window*, float, float));
    void diSetFramebufferSizeCallback(DI_Window* window, void(*callbackFunc)(DI_Window*, float, float));
    void diSetFramebufferSizeCallback(DI_Window* window, void(*callbackFunc)(DI_Window*, float, float));
    void diSetWindowContentScaleCallback(DI_Window* window, void(*callbackFunc)(DI_Window*, float, float));
    void diSetWindowContentScaleCallback(DI_Window* window, void(*callbackFunc)(DI_Window*, float, float));
    void diSetWindowContentScaleCallback(DI_Window* window, void(*callbackFunc)(DI_Window*, float, float));
    void diSetWindowContentScaleCallback(DI_Window* window, void(*callbackFunc)(DI_Window*, float, float));

    /// @brief processes all pending events in queue (event listening on another thread, implemented by x11, see _glfwPollEventsX11)
    void diPollEvents();
    void diPollEvents();
    /// @brief Waits for events (with timeout of 1000 by default) and blocks until an event is encountered (then handles them)
    void diWaitForEvents();
    void diWaitForEvents();
    /// @brief Waits for events (with customizable timeout and blocks until an event is encountered (then handles them)
    void diWaitForEvents(double timeoutMs);
    void diWaitForEvents(double timeoutMs);

    //void diPostEmptyEvent();
    //void diPostEmptyEvent();
    //int diGetInputMode(DI_Window* window, int mode);
    //int diGetInputMode(DI_Window* window, int mode);
    //int diGetInputMode(DI_Window* window, int mode);
    //void diSetInputMode(DI_Window* window, int mode, int value)
    //void diSetInputMode(DI_Window* window, int mode, int value)
    //void diSetInputMode(DI_Window* window, int mode, int value)
    //int diRawMouseMotionSupported();
    //int diRawMouseMotionSupported();
    //string diGetKeyName(int key, int scancode);
    //string diGetKeyName(int key, int scancode);
    //int diGetKeyScancode(int key);
    //int diGetKeyScancode(int key);
    //int diGetKey(DI_Window* window, int key);
    //int diGetKey(DI_Window* window, int key);
    //int diGetKey(DI_Window* window, int key);
    //int diGgtMouseButton(DI_Window* window, int button);
    //int diGgtMouseButton(DI_Window* window, int button);
    //int diGgtMouseButton(DI_Window* window, int button);

    /// @brief Retrieves mouse position on window content area (starting from top left)
    /// @param window DI_Window object
    /// @param window DI_Window object
    /// @return array of double values, corresponding to xpos and ypos of where the cursor is relative to left edge of content area
    double* diGetMouseButton(DI_Window* window);
    double* diGetMouseButton(DI_Window* window);
    double* diGetMouseButton(DI_Window* window);

    DI_Cursor* diCreateStandardCursor();
    DI_Cursor* diCreateStandardCursor();
    DI_Cursor* diCreateStandardCursor();
    void diDestroyCursor(DI_Cursor* cursor);
    void diDestroyCursor(DI_Cursor* cursor);
    void diDestroyCursor(DI_Cursor* cursor);
    void diSetCursor(DI_Window* window, DI_Cursor* cursor);
    void diSetCursor(DI_Window* window, DI_Cursor* cursor);
    void diSetCursor(DI_Window* window, DI_Cursor* cursor);
    void diSetCursor(DI_Window* window, DI_Cursor* cursor);
    /// @brief
    /// @param window 
    /// @param callbackFunc 
    /// @callback_Signature
    /// func(DI_Window window, int key, int action, int mods????)
    /// func(DI_Window window, int key, int action, int mods????)
    void diSetKeyCallback(DI_Window* window,  void(*callbackFunc)(DI_Window*, int, int, int));
    void diSetKeyCallback(DI_Window* window,  void(*callbackFunc)(DI_Window*, int, int, int));
    void diSetKeyCallback(DI_Window* window,  void(*callbackFunc)(DI_Window*, int, int, int));
    void diSetKeyCallback(DI_Window* window,  void(*callbackFunc)(DI_Window*, int, int, int));
    void diSetMouseButtonCallback(DI_Window* window, void(*callbackFunc)(DI_Window* window, int button, bool pressed, int mods));
    void diSetMouseButtonCallback(DI_Window* window, void(*callbackFunc)(DI_Window* window, int button, bool pressed, int mods));
    void diSetMouseButtonCallback(DI_Window* window, void(*callbackFunc)(DI_Window* window, int button, bool pressed, int mods));
    void diSetMouseButtonCallback(DI_Window* window, void(*callbackFunc)(DI_Window* window, int button, bool pressed, int mods));
    void diSetCursorPosCallback(DI_Window* window, void(*callbackFunc)(DI_Window* window, double xpos, double ypos));
    void diSetCursorPosCallback(DI_Window* window, void(*callbackFunc)(DI_Window* window, double xpos, double ypos));
    void diSetCursorPosCallback(DI_Window* window, void(*callbackFunc)(DI_Window* window, double xpos, double ypos));
    void diSetCursorPosCallback(DI_Window* window, void(*callbackFunc)(DI_Window* window, double xpos, double ypos));
    // void diSetCursorEnterCallback(DI_Window* window, void(*callbackFunc)(DI_Window* window, bool entered)));
    // void diSetCursorEnterCallback(DI_Window* window, void(*callbackFunc)(DI_Window* window, bool entered)));
    // void diSetCursorEnterCallback(DI_Window* window, void(*callbackFunc)(DI_Window* window, bool entered)));
    // void diSetCursorEnterCallback(DI_Window* window, void(*callbackFunc)(DI_Window* window, bool entered)));

    // see GLFWscrollfun  callback for more details on the callback params
    void diSetScrollCallback(DI_Window* window, void(*callbackFunc)(DI_Window* window, double xoffset, double yoffset));
    void diSetScrollCallback(DI_Window* window, void(*callbackFunc)(DI_Window* window, double xoffset, double yoffset));
    void diSetScrollCallback(DI_Window* window, void(*callbackFunc)(DI_Window* window, double xoffset, double yoffset));
    void diSetScrollCallback(DI_Window* window, void(*callbackFunc)(DI_Window* window, double xoffset, double yoffset));



    // Finally context related functions 🥲🥲🥲
    void diMakeContextCurrent(DI_Window* window);
    void diMakeContextCurrent(DI_Window* window);
    void diMakeContextCurrent(DI_Window* window);
    DI_Window* diGetCurrentContext();
    DI_Window* diGetCurrentContext();
    DI_Window* diGetCurrentContext();
    void diSwapBuffers(DI_Window* window);
    void diSwapBuffers(DI_Window* window);
    void diSwapBuffers(DI_Window* window);
    // This is mainly for vsync. See https://github.com/glfw/glfw/blob/8f2f766f0d2ed476c03a2ae02e48ac41a9602b03/include/GLFW/glfw3.h#L6168
    void diSwapInterval(int interval);
    void diSwapInterval(int interval);

    void processEvent(XEvent* event);


void diDrawLine (unsigned int x_origin, unsigned int y_origin, unsigned int x_dest, unsigned int y_dest, Color color);

void diDrawLine (unsigned int x_origin, unsigned int y_origin, unsigned int x_dest, unsigned int y_dest, unsigned int r, unsigned int g, unsigned int b) : diDrawLine(x_origin, y_origin, x_dest, y_dest, Color(r, g, b));





