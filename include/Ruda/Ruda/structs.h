#pragma once
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>

// Abstracts the Graphics Context struct from X11 to XGC to avoid ambiguity
#define XGC GC

struct Ruda_Context; 

struct Ruda_Context {

	XGC xContext;

	Ruda_Context(XGC xContext) {this->xContext = xContext;};

};
