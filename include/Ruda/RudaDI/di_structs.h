#pragma once
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include "../Util/color.h"
#include "../Ruda/ruda.h"
#include "di.h"


struct DI_Structure;
struct DI_Window;
struct DI_Monitor;
struct DI_Cursor;

struct DI_Structure {

	Vector<DI_Window*> windows;
	
	DI_Window* currentWindow;
	
	DI_Display* display;
	
	Window root;
	
	int screen = -1;
	
	XEvent currentEvent;
	
	bool shouldClose;
	
	DI_Structure(){};
	
	~DI_Structure();
	
}; 

struct DI_Window {

	Window xWindow;
	unsigned int width;
	unsigned int height;
	string title;
	Ruda_Context* context;

	DI_Window();
	DI_Window(XWindow* xWindow, unsigned int width, unsigned int height, string title);
	~DI_Window();
};
