#pragma once
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>

struct Ruda_Context; 
struct DI_DesiredVisibility;

// This is a utility struct to represent booleans in C, we may not need
// this since thre are bools in C++!

struct Ruda_Context {

	GC graphicsContext;

	Ruda_Context(GC context) {this->graphicsContext = context;};
};

// Video Mode is used to represent the properties
// associated with a monitor's video mode.
struct DI_VidMode {
	// Width and Height in screen coords
	int width;
	int height;
	// Bit depth of each channenl
	int redBits;
	int greenBits;
	int blueBits;
	// refresh rate
	int refreshRate;
};