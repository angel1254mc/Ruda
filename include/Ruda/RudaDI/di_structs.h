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
#include "../Ruda/structs.h"


struct DI_Structure;
struct DI_WindowConfig;
struct DI_Window;
struct DI_Monitor;
struct DI_Cursor;
struct DI_Display;

struct DI_Structure {

	std::vector<DI_Window*> windows;
	
	XContext xContext;
	
	DI_Window* currentWindow;
	
	DI_Display* display;
	
	XWindow root;

	DI_WindowConfig currentConfig;

	int screen = -1;
	
	XEvent currentEvent;
	
	bool shouldClose;
	
	~DI_Structure();
	
};

struct DI_Window {

	XWindow xWindow;

	Ruda_Context* context;

	DI_WindowConfig config;

	DI_Window* parent;

	struct {
		void (*windowResizeCallback)(int, int) = nullptr;
		void (*windowClosCallback)() = nullptr;
		void (*windowFocusCallback)() = nullptr;
		void (*windowIconifyCallback)(DI_Window*, int) = nullptr;
		void (*windowMaximizeCallback)(DI_Window*, int) = nullptr;
		void (*windowFramebufferSizeCallback)(DI_Window*, float, float) = nullptr;
		void (*windowContentScaleCallback)(DI_Window*, float, float) = nullptr;
		void (*keyCallback)(DI_Window*, int, int, int) = nullptr;
		void (*mouseButtonCallback)(DI_Window* window, int button, bool pressed, int mods);
		void (*cursorPosCallback)(DI_Window* window, double xpos, double ypos);
		void (*scrollCallback)(DI_Window* window, double xoffset, double yoffset);
		void (*cursorEnterCallback)(DI_Window* window, bool entered);
	} callbacks;

	DI_Window(XWindow xWindow, str title = structure->currentConfig.title, unsigned int width = structure->currentConfig.width, unsigned int height = structure->currentConfig.height, DI_Window* parentWindow = nullptr);
	~DI_Window();
};

struct DI_Display {

	XDisplay* xDisplay;
	DI_Display(XDisplay* xDisplay) {this->xDisplay = xDisplay;};
};

struct DI_WindowConfig {
	int x_position = 0;
	int y_position = 0;
	unsigned int width = 1920;
	unsigned int height = 1080;
	unsigned int refresh_rate = 60;
	bool maintain_aspect_ratio = false;
	bool focused = true;
	bool minimized = false;
	bool resizable = true;
	bool visible = true;
	bool decorated = true;
	bool auto_minimize = false;
	bool floating = false;
	bool maximized = false;
	bool center_cursor = false;
	bool transparent_framebuffer = false;
	bool hovered = false;
	bool double_framebuffer = true;
	bool focus_on_show = true;
	bool mouse_passthrough = false;
	bool scale_to_monitor = false;
	bool scale_framebuffer = true;
	str title = "Default Window Title";
	DI_Monitor* default_monitor = nullptr;
};