#pragma once
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <X11/extensions/Xrandr.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>
#include "../Util/color.h"
#include "../Ruda/structs.h"
#include "../Ruda/ruda.h"
#include "./di.h"
// This is an alternative to typedef Window XWindow, or #define Window XWindow for providing an alias
// Reason I used it is because #define directive messes with the source code, replacing all instances of `Window`
// to compilation and I wouldn't want that to mess with any other libraries we depend on
// See https://stackoverflow.com/a/75367122

struct DI_Structure;
typedef struct DI_Window;
// Wrapper  around the RUDA context
typedef struct Ruda_Context;
typedef struct DI_Context;
typedef struct DI_Monitor;
typedef struct DI_Display;
typedef struct DI_Cursor;
typedef struct DI_VidMode;
typedef struct DI_Image;
typedef struct DI_FramebufferConfig;
typedef struct DI_WindowConfig;



using DI_Bool = bool;


// Effectively a way to store
// configuration details for a window we intend to create
struct DI_WindowConfig {
	int xPos;
	int yPos;
	int width;
	int height;
	std::string title;
	DI_Bool resizable;
	DI_Bool visible;
	DI_Bool decorated;
	DI_Bool focused;
	DI_Bool floating;
	DI_Bool maximized;
	DI_Bool centerCursor;
	DI_Bool focusOnShow;
	DI_Bool mousePassthrough;
	DI_Bool scalelToMonitor;
	DI_Bool scaleFrameBuffer;
	DI_Monitor* defaultMonitor;
};

// Here we store configuration details (hints)
// Associated with our Framebuffer
// The framebuffer is the raw data tht our video card renders onto the monitor screen
// https://openbooks.sourceforge.net/books/wga/graphics.html

struct DI_FramebufferConfig {
	int redBits;
	int greenBits;
	int blueBits;
	int alphaBits;
	int depthBits;
	int stencilBits;
	DI_Bool doubleBuffer;
	DI_Bool transparent;
};

// Analogous to _GLFWLibrary + X11 Platform Functions
struct DI_Structure {

	DI_Bool initialized;
	// No need for allocator. These are used specifically
	// In the case that a user provides a class/object for
	// the user pointer property, which we won't be utilizing
	// for this project as it does not affect the rendering pipeline

	std::vector<DI_Monitor*> monitors;
	int monitorCount;

	std::vector<DI_Cursor*> cursors;
	std::vector<DI_Window*> windows;
	DI_WindowConfig windowConfig;
	DI_Window* currentWindow;

	struct {
		monitorCallback monitor;
	} callbacks;
	
	/** =============================================================
	 * BEGIN X11 SPECIFIC ATTRIBUTES
	 * ============================================================== 
	*/


	XDisplay* display;
	XWindow root;
	int screen = -1;
	// maps window ids to DI_Window pointers
	XContext context;

	float contentScaleX, contentScaleY;
	// Helper Window for IPC. Actually mainly used for selection purposes
	XWindow helperWindowHandle;
	XCursor hiddenCursorHandle;

	DI_FramebufferConfig fbConfig;

	XEvent currentEvent;
	
	bool shouldClose;
	
	DI_Structure(){};
	
	~DI_Structure();
	
};

extern DI_Structure* structure;

struct DI_Window {

	DI_Bool resizable;
	DI_Bool decorated;
	DI_Bool autoIconify;
	DI_Bool floating;
	DI_Bool focusOnShow;
	DI_Bool mousePassthrough;
	DI_Bool shouldClose;
	// optional, used when high-level devs
	// wrap over the DI_Window struct with their own class
	void* userPointer;
	// Yes or no to back buffer + front buffer
	DI_Bool doubleBuffer;
	DI_VidMode* videoMode;
	DI_Monitor* monitor;
	DI_Cursor* cursor;
	std::string title;

	XWindow xWindow;
	unsigned int width;
	unsigned int height;
	Ruda_Context* context;
	int minWidth, minHeight;
	int maxWidth, maxHeight;
	int numer, denom; // what.

	DI_Bool stickyKeys;
	DI_Bool stickyMouseButtons;
	DI_Bool lockKeyMods;
	int cursorMode;
	
	// char keys[DI_KEY_LAST + 1];
	// char mouseButtons[DI_MOUSE_BUTTON_LAST + 1];

	// Below is used to keep track of virtual cursor position
	// for when mouse is disabled
	double  virtualCursorPosX, virtualCursorPosY;
	DI_Bool rawMouseMotion;
	//DI_Context context;

	struct {
		windowResizeCallback size;
		windowPositionCallback pos;
		windowCloseCallback close;
		// windowRefreshCallback refresh;
		windowFocusCallback focus;
		windowIconifyCallback iconify;
		windowMaximizeCallback maximize;
		windowFramebufferSizeCallback fbsize;
		windowContentScaleCallback scale;
		mouseButtonCallback mouseButton;
		cursorPosCallback cursorPos;
		cursorEnterCallback cursorEnter;
		scrollCallback scroll;
		keyCallback key;
		charCallback character;
		charModsCallback charMods;
		pathDropCallback  pathDrop;
	} callbacks;

	// Everything after this is platform specific
	// IMPORTANT = in GLFW the following properties gets stored in an 'x11' struct property. Seeing as how we're working
	// on one platform, we just keep it on the base object.
	Colormap colormap;
	XWindow handle;
	XWindow parent;
	XIC ic;

	DI_Bool overrideRedirect;
	DI_Bool iconified;
	DI_Bool maximized;

	// Framebuffer transparency for transparent windows and stuff
	DI_Bool transparent;

	// Position and size in screen units
	int xPos, yPos;
	
	// last received cursor positions
	int lastCursorPosX, lastCursorPosY;
	// Last position  WE (Ruda DI) moved the cursor
	int warpCursorPosX, warpCursorPosY;

	Time keyPressTimes[256];

	DI_Window(DI_WindowConfig windowConfig, DI_FramebufferConfig fbConfig);
	~DI_Window();

};



struct DI_Context {
	//
	int client;
	int source;
	// int major, minor, revision;
	// DI_Bool forward, debug, noerror;
	// int profile;
	// int robustness;
	// int release;
	
	void (*makeCurrent)(DI_Window* window);
	void (*swapBuffers)(DI_Window* window);
	void (*swapInterval)(int interval);
	
	void (*destroy)(DI_Window* window);
	
	// RUDA Context
	Ruda_Context handle;
	// RUDA Window [TODO @angel1254mc]
};


// DI_Window();
// DI_Window(XWindow* XWindow, unsigned int width, unsigned int height, string title);
// ~DI_Window();

struct DI_Monitor {
	std::string name;
	// void userPointer;

	int widthMM, heightMM;
	DI_Window* window;
	DI_VidMode* modes;
	int modeCount;
	DI_VidMode currentMode;

	//DI_GammaRamp originalRamp;
	//DI_GammaRamp currentRamp;

	// The following are X11-specific properties
	// These correspond to Rotate & Resize Modes,  which are part of
	// the XRandR extension for xlib
	// This allows us to set the size, orientation, and/or reflection of the outputs for a screen
	RRMode oldMode;
	RROutput output;
	RRCrtc crtc; // Display Controller -> controls size and timing of signal sent to video card

	// Index of xinerama screen (multiple monitors as one big virtual screen)
	int index;
};



// X11 Specific Things begin here
	// struct {
	// 	DI_Bool init();
	// 	void terminate();
	// 	void getCursorPos(DI_Window* window, double xPos, double yPos);
	// 	void setCursorPos(DI_Window* window, double xPos, double yPos);
	// 	void setCursorMode(DI_Window* window, int mode);
	// 	// Raw motion corresponds to unscaled, unaccelerated motion. Its good
	// 	// for 3d cameras and stuff because it gives mouse motion in dx dy rather
	// 	// than coordinates
	// 	void setRawMouseMotion(DI_Window* window, DI_Bool enabled);
	// 	DI_Bool createCursor(DI_Cursor* cursor, const DI_Image* image, int xHot, int yHot);
	// 	DI_Bool createStandardCursor(DI_Cursor* cursor, int shape);
	// 	void destroyCursor(DI_Cursor* cursor);
	// 	void setCursor(DI_Window* window, DI_Cursor* cursor);
	// 	std::string getScanCodeName(int scancode);
	// 	int getKeyScancode(std::string keyname);
	// 	// Monitor-specific functions
	// 	void freeMonitor(DI_Monitor* monitor);
	// 	int* getMonitorPos(DI_Monitor* monitor);
	// 	float* getMonitorContentScale(DI_Monitor* monitor);
	// 	int*  getMonitorWorkarea(DI_Monitor* monitor);
	// 	DI_VidMode* getVideoModes(DI_Monitor* monitor, int count);
	// 	DI_VidMode getVideoMode(DI_Monitor* monitor);
	// 	// DI_GammaRamp getGammaRamp(DI_Monitor* monitor);
	// 	// void setGammaRamp(DI_Monitor* monitor, DI_GammaRamp* gammaRamp);

	// 	// Window-specific functions
	// 	DI_Bool createWindow( );
		
	// } PlatformX11;