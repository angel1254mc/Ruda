#pragma once
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>
#include "../Util/color.h"
#include "../Ruda/ruda.h"
#include "../Ruda/structs.h"
#include "di.h"


struct DI_Structure;
struct DI_Window;
struct DI_Monitor;
struct DI_Cursor;
struct DI_Display;

struct DI_Structure {

	std::vector<DI_Window*> windows;
	
	DI_Window* currentWindow;
	
	DI_Display* display;
	
	XWindow root;
	
	int screen = -1;
	
	XEvent currentEvent;
	
	bool shouldClose;
	
	~DI_Structure();
	
}; 

struct DI_Window {

	XWindow* xWindow;
	unsigned int width;
	unsigned int height;
	std::string title;
	Ruda_Context* context;

	DI_Window(XWindow xWindow, unsigned int width, unsigned int height, std::string title);
	~DI_Window();
};

struct DI_Display {

	XDisplay* xDisplay;
	DI_Display(XDisplay* xDisplay);
};
