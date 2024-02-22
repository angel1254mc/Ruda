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

struct DI_Structure;
struct DI_Window;

struct DI_Structure {

	Vector<DI_Window*> windows;
	
	DI_Window* currentWindow;
	
	DI_Display* display;
	
	Window root;
	
	int screen = -1;
	
	XEvent currentEvent;
	
	bool shouldClose;
}; 

stuct DI_Window {


}

