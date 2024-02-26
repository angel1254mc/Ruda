#pragma once
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include "./structs.h"

// Abstracts the Window struct from X11 to XWindow to avoid ambiguity
#define XWindow Window
// Abstracts the Visual struct from X11 to XVisual to avoid ambiguity
#define XVisual Visual
// Abstracts the Cursor struct from X11 to XCursor
#define XCursor Cursor

#define XDisplay Display

// Creates an X11 Window, binds to it and uses the drawable from that window to draw whatever we render w/ Ruda
// Mirror of GLX's glxCreateContext as seen here https://registry.khronos.org/OpenGL-Refpages/gl2.1/xhtml/glXCreateContext.xml
Ruda_Context Ruda_CreateContext(XWindow window);