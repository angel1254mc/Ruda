#include "RudaDI/di.h"

DI_Window::DI_Window (Window xWindow, str title, unsigned int width, unsigned int height, DI_Window* parentWindow) {
	this->xWindow = xWindow;
	this->config = structure->currentConfig;
	this->config.width = width;
	this->config.height = height;
	this->config.title = title;
	parent = parentWindow;
	print("creating context");
	// I guess we caan handle actually generating the graphics context and the pixmap here but this can get switched around as needed
	this->context = new Ruda_Context(XCreateGC(structure->display->xDisplay, this->xWindow, 0, NULL));
	// set the foreground to white for now.

	print("created context");

	XSetForeground(structure->display->xDisplay, this->context->xContext, WhitePixel(structure->display->xDisplay, structure->screen));

	print("set foreground");
	// Please please please let me know when
	// Keys are pressed and released
	// Buttons are pressed and released
	// Mouse moves
	// window resizing and such (and some more stuff like Mapping, unmapping, etc.)
	XSelectInput(structure->display->xDisplay, this->xWindow, KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask);
}

DI_Window::~DI_Window() {
	XFreeGC(structure->display->xDisplay, this->context->xContext);
}


int DI_WindowConfig::diWindowHint(unsigned int hint, int value) {
	switch (hint)
	{
		case DI_RESIZABLE:
			if (value == RUDA_FALSE) {
				if (resizable) {
					resizable = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!resizable) {
					resizable = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_MAINTAIN_ASPECT_RATIO:
			if (value == RUDA_FALSE) {
				if (maintain_aspect_ratio) {
					maintain_aspect_ratio = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!maintain_aspect_ratio) {
					maintain_aspect_ratio = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_FOCUSED:
			if (value == RUDA_FALSE) {
				if (focused) {
					focused = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!focused) {
					focused = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_MINIMIZED:
			if (value == RUDA_FALSE) {
				if (minimized) {
					minimized = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!minimized) {
					minimized = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_VISIBLE:
			if (value == RUDA_FALSE) {
				if (visible) {
					visible = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!visible) {
					visible = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_DECORATED:
			if (value == RUDA_FALSE) {
				if (decorated) {
					decorated = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!decorated) {
					decorated = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_AUTO_MINIMIZE:
			if (value == RUDA_FALSE) {
				if (auto_minimize) {
					auto_minimize = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!auto_minimize) {
					auto_minimize = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_FLOATING:
			if (value == RUDA_FALSE) {
				if (floating) {
					floating = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!floating) {
					floating = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_MAXIMIZED:
			if (value == RUDA_FALSE) {
				if (maximized) {
					maximized = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!maximized) {
					maximized = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_CENTER_CURSOR:
			if (value == RUDA_FALSE) {
				if (center_cursor) {
					center_cursor = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!center_cursor) {
					center_cursor = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_TRANSPARENT_FRAMEBUFFER:
			if (value == RUDA_FALSE) {
				if (transparent_framebuffer) {
					transparent_framebuffer = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!transparent_framebuffer) {
					transparent_framebuffer = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_HOVERED:
			if (value == RUDA_FALSE) {
				if (hovered) {
					hovered = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!hovered) {
					hovered = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_FOCUS_ON_SHOW:
			if (value == RUDA_FALSE) {
				if (focus_on_show) {
					focus_on_show = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!focus_on_show) {
					focus_on_show = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_MOUSE_PASSTHROUGH:
			if (value == RUDA_FALSE) {
				if (mouse_passthrough) {
					mouse_passthrough = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!mouse_passthrough) {
					mouse_passthrough = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_SCALE_TO_MONITOR:
			if (value == RUDA_FALSE) {
				if (scale_to_monitor) {
					scale_to_monitor = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!scale_to_monitor) {
					scale_to_monitor = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_DOUBLE_FRAMEBUFFER:
			if (value == RUDA_FALSE) {
				if (double_framebuffer) {
					double_framebuffer = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!double_framebuffer) {
					double_framebuffer = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_SCALE_FRAMEBUFFER:
			if (value == RUDA_FALSE) {
				if (scale_framebuffer) {
					scale_framebuffer = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!scale_framebuffer) {
					scale_framebuffer = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_POSITION_X:
			if (value != x_position) {
				x_position = value;
				return HINT_CHANGED;
			} else return HINT_UNCHANGED;
		case DI_POSITION_Y:
			if (value != y_position) {
				y_position = value;
				return HINT_CHANGED;
			} else return HINT_UNCHANGED;
		case DI_REFRESH_RATE:
			if (value != refresh_rate) {
				refresh_rate = value;
				return HINT_CHANGED;
			} else return HINT_UNCHANGED;
	}
	return HINT_INVALID_HINT;
}
