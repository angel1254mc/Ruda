#include "../../include/Ruda/RudaDI/di_structs.h"
#include "../../include/Ruda/Ruda/ruda.h"

DI_Bool createX11Window(DI_Window* window, DI_WindowConfig windowConfig, DI_FramebufferConfig fbConfig);

DI_Window::~DI_Window() {
	// Delete all dynamically allocated attributes
	delete videoMode;
	delete monitor;
	// Context should handle all graphics contexts if any were created.
	delete context;
	
	//XLib specific state management
	XDeleteContext(structure->display, this->xWindow, structure->context);
	XUnmapWindow(structure->display, this->xWindow);
	XDestroyWindow(structure->display, this->xWindow);
	this->xWindow = NULL;

	if (this->colormap) {
		XFreeColormap(structure->display, this->colormap);
		this->colormap = NULL;
	}

	XFlush(structure->display);

};

DI_Window::DI_Window(DI_WindowConfig windowConfig, DI_FramebufferConfig fbConfig) {
	this->videoMode = new DI_VidMode();
	this->title = windowConfig.title;
	this->videoMode->width = windowConfig.width;
	this->videoMode->height = windowConfig.height;
	this->videoMode->redBits = fbConfig.redBits;
	this->videoMode->blueBits = fbConfig.blueBits;
	this->videoMode->greenBits = fbConfig.greenBits;
	this->videoMode->refreshRate = 60; /**[TODO: @angel1254mc] replaec with defaultHints refreshRate*/ 
	this->monitor = nullptr;
	this->resizable = windowConfig.resizable;
	this->decorated = windowConfig.decorated;
	this->autoIconify = false;
	this->floating = false;
	this->focusOnShow = false;
	this->mousePassthrough = false;
	this->cursorMode = 1;
	this->doubleBuffer = fbConfig.doubleBuffer;
};


DI_Window* diCreateWindow(unsigned int width, unsigned int height, std::string title, DI_Monitor* monitor, DI_Window* parent) {

    DI_Window* window;
    // Lives on the stack btw
    DI_WindowConfig windowConfig = structure->windowConfig;
    DI_FramebufferConfig fbConfig = structure->fbConfig;
    // At the most basic level, window creation  requires

    // Checking Window size is correct
    // Checking window title is not null

    // Checking library has been initialized

    // Creating the DI_Window object

    // pushing the pointer to the DI_Window object to the list of Windows
    
    // Initializing Window with all necessary attributes

    // Creating the underlying X11 Window, binding it to our Window struct

    // Return the DI_Window pointer!

    if (title.empty()) {
        throw std::runtime_error("No value provided for title.");
        return NULL;
    }
    if (width < 0 || height < 0) {
        throw std::runtime_error("Invalid value provided for width/height.");
        return NULL;
    }
    if (!structure->initialized) {
        throw std::runtime_error("DI was not initialized!");
        return NULL;
    }
	windowConfig.title = title;
	windowConfig.width = width;
	windowConfig.height = height;

    window = new DI_Window(windowConfig, fbConfig);
    
    // Now create actual window in x11
    if (!createX11Window(window, windowConfig, fbConfig)) {
		delete window;
	}

	return window;
}

/// @brief Handles creation of native window, Initialization of Ruda interface and context, Flushes changes to window
/// @param window DI_Window, to which we wish to bind resulting XWindow
/// @param windowConfig Window configuration struct for creating the window
/// @param fbConfig Framebuffer config for altering Ruda initialization and window setup
/// @return True if successful 
DI_Bool createX11Window(DI_Window* window, DI_WindowConfig windowConfig, DI_FramebufferConfig fbConfig) {

	// Visual is concerned with low-level control. In certain GL-based interfaces it gets passed around
	// for example visual EGL initialization requires the visual as a parameter. The reason why it's above everything else rn
	// is because we should remember it for when we choose to implement our Ruda interface + context which we'll end up using in tandem
	// with this window system.

	XVisual* visual = DefaultVisual(structure->display,  structure->screen);
	int depth = DefaultDepth(structure->display, structure->screen);


	// This is the part that handles a couple of things

	// 1) Activate the interface between Ruda and the window system (our own GLX essentially)

	// [TODO @angel1254mc, @james] Create a ruda_Init() function or smth like that

	// 2) Actually create the window, map it to screen and bind it to our monitor (notice how earlier it was null)
	// **NOTE** Seeing as how contentScale initialization is not done on diInit(), I'll leave this for this upcoming sprint
	int xPos = windowConfig.xPos >= 0 ? windowConfig.xPos : 0;
	int yPos = windowConfig.yPos >= 0 ? windowConfig.yPos : 0;

	window->colormap = XCreateColormap(structure->display, structure->root, visual, AllocNone);
	XSetWindowAttributes windowAttributes;

	windowAttributes.colormap = window->colormap;
	// Same thing as SelectInput
	windowAttributes.event_mask = StructureNotifyMask | KeyPressMask | KeyReleaseMask |
                    PointerMotionMask | ButtonPressMask | ButtonReleaseMask |
                    ExposureMask | FocusChangeMask | VisibilityChangeMask |
                    EnterWindowMask | LeaveWindowMask | PropertyChangeMask;

	// This helped me get a good idea about what exactly InputOutput refers to in this context https://tronche.com/gui/x/xlib/window/attributes/
	// Also, CWBorderPixel, CWColormap, etc are essentially masks that dictate the properties/attributes of the window we seek to change w/ the
	// windowAttributes object we've provided
	window->xWindow = XCreateWindow(structure->display,
									structure->root,
									xPos, yPos, windowConfig.width, windowConfig.height,
									0, depth, InputOutput, visual,
									CWBorderPixel | CWColormap | CWEventMask,
									&windowAttributes);
	// The reason why XSaveContext matters is because it allows us to FIND our DI_Window object on events
	// For instance, an XEvent will provide the xWindow where the event occured, but not the DI_Window
	// By saving a pointer to our DI_Window alongside the xWindow, we can just call XFindContext(xWindow) to return a pointer
	// to DI_Window
	XSaveContext(structure->display,
				window->xWindow,
				structure->context,
				(XPointer)window);

	// Additional attributes we could take care of here
	// Decoration
	// floating
	// maximized
	// [TODO: @angel1254mc] after Tuesday 2/27/2024 verify whether these are necessary

	// set window title
	XStoreName(structure->display, window->xWindow, windowConfig.title.c_str());
	// get window position
	// Translate the top left of our window into coordinates in terms of the root window (monitor view area)
	Window temp;
	XTranslateCoordinates(structure->display, window->xWindow, structure->root, 0, 0, &window->xPos, &window->yPos, &temp);
	if (!window->xPos)
		window->xPos = 0;
	if (!window->yPos)
		window->yPos = 0;

	// Get window size
	XWindowAttributes tempAttribs;
	XGetWindowAttributes(structure->display, window->xWindow, &tempAttribs);
	window->width = tempAttribs.width;
	window->height = tempAttribs.height;


	// 3) Create a Ruda context, bind it to our Window/XWindow

	//window->context = rudaCreateContext(window, fbConfig);

	// 4) Finally, map the window and flush all events

	XMapWindow(structure->display, window->xWindow);

	// We have to wait for the window to finish mapping before drawing on it.
	// Credits to Cristopher Tronche & his awesome XLib mini-tutorial
	// https://tronche.com/gui/x/xlib-tutorial/
	while (true) {
		XEvent e;
		XNextEvent(structure->display, &e);
		if (e.type == MapNotify)
			break;
	}
    XFlush(structure->display);

	return true;

}