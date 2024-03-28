#pragma once
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <string.h>
#include "../Util/color.h"
#include "../Ruda/structs.h"
#include "../Util/util.h"
#include "di.h"
#include "../Ruda/rudax.h"

struct DI_Structure;
struct DI_Window;
struct DI_Monitor;
struct DI_Cursor;
struct DI_Display;
struct DI_WindowConfig;

extern DI_Structure* structure; //global structure referenced everywhere

struct DI_WindowConfig {
	private:
	int x_position = 0;
	int y_position = 0;
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
	DI_Monitor* default_monitor = nullptr;
	
	public:
	unsigned int width = 1920;
	unsigned int height = 1080;
	str title = "Default Window Title";
	int diWindowHint(unsigned int hint, int value);
	int  get_x_position() { return x_position; };
	int  get_y_position() { return y_position; };

	void set_x_position(int x) { this->x_position = x; };
	void set_y_position(int y) { this->y_position = y; };

	unsigned int get_refresh_rate() { return refresh_rate; };
	bool get_maintain_aspect_ratio() { return maintain_aspect_ratio; };
	bool get_focused() { return focused; };
	bool get_minimized() { return minimized; };
	bool get_resizable() { return resizable; };
	bool get_visible() { return visible; };
	bool get_decorated() { return decorated; };
	bool get_auto_minimize() { return auto_minimize; };
	bool get_floating() { return floating; };
	bool get_maximized() { return maximized; };
	bool get_center_cursor() { return center_cursor; };
	bool get_transparent_framebuffer() { return transparent_framebuffer; };
	bool get_hovered() { return hovered; };
	bool get_double_framebuffer() { return double_framebuffer; };
	bool get_focus_on_show() { return focus_on_show; };
	bool get_mouse_passthrough() { return mouse_passthrough; };
	bool get_scale_to_monitor() { return scale_to_monitor; };
	bool get_scale_framebuffer() { return scale_framebuffer; };
};

struct DI_ContextConfig {
	int           client;
    int           source;
    int           major;
    int           minor;
    bool      forward;
    bool      debug;
    bool      noerror;
    int           profile;
    int           robustness;
    int           release;
    DI_Window*  share;
    struct {
        bool  offline;
    } nsgl;
};

struct DI_Structure {

	std::vector<DI_Window*> windows;
	
	Ruda_Context* ctx;
	
	DI_Window* currentWindow;
	
	DI_Display* display;
	
	XWindow root;

	DI_WindowConfig* currentConfig;
	DI_FBConfig* fbConfig;
	DI_ContextConfig* ctxConfig;
	int screen = -1;
	
	XEvent currentEvent;
	
	
	~DI_Structure();
	
};

struct DI_FBConfig
{
    int         redBits;
    int         greenBits;
    int         blueBits;
    int         alphaBits;
    int         depthBits;
    int         stencilBits;
    int         accumRedBits;
    int         accumGreenBits;
    int         accumBlueBits;
    int         accumAlphaBits;
    int         auxBuffers;
    bool    stereo;
    int         samples;
    bool    sRGB;
    bool    doublebuffer;
    bool    transparent;
    uintptr_t   handle;
};

struct DI_Window {

	XWindow xWindow;
	GLXWindow* rudaXWindow;

	Ruda_Context* context;
	GLXContext* rudaXContext;

	DI_WindowConfig config;

	DI_Window* parent;

	struct {
		windowResizeCallback resize = NULL;
		windowCloseCallback close = NULL;
		windowFocusCallback focus = NULL;
		windowIconifyCallback iconify = NULL;
		windowMaximizeCallback maximize = NULL;
		windowFramebufferSizeCallback fb_resize = NULL;
		windowPosCallback window_pos_change = NULL;
		windowContentScaleCallback content_scale_resize = NULL;
		keyCallback keypress = NULL;
		mouseButtonCallback mouse_click = NULL;
		cursorPosCallback cursor_pos_change = NULL;
		scrollCallback scroll = NULL;
		cursorEnterCallback cursor_enter = NULL;
	} callbacks;

	bool shouldClose = false;

	DI_Window(XWindow xWindow, str title = structure->currentConfig->title, unsigned int width = structure->currentConfig->width, unsigned int height = structure->currentConfig->height, DI_Window* parentWindow = NULL);
	~DI_Window();
};

struct DI_Display {

	XDisplay* xDisplay;
	DI_Display(XDisplay* xDisplay) {this->xDisplay = xDisplay;};
};