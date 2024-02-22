#pragma once
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>

struct Ruda_Context; 

struct Ruda_Context {

	GC context;

	Ruda_Context(GC context) {this->context = context};

};
