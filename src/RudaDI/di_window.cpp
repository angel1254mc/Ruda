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

int diWindowHint(unsigned int hint, int value) {
	switch (hint)
	{
		case DI_RESIZABLE:
			if (value == RUDA_FALSE) {
				if (structure->currentConfig.resizable) {
					structure->currentConfig.resizable = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!structure->currentConfig.resizable) {
					structure->currentConfig.resizable = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_MAINTAIN_ASPECT_RATIO:
			if (value == RUDA_FALSE) {
				if (structure->currentConfig.maintain_aspect_ratio) {
					structure->currentConfig.maintain_aspect_ratio = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!structure->currentConfig.maintain_aspect_ratio) {
					structure->currentConfig.maintain_aspect_ratio = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_FOCUSED:
			if (value == RUDA_FALSE) {
				if (structure->currentConfig.focused) {
					structure->currentConfig.focused = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!structure->currentConfig.focused) {
					structure->currentConfig.focused = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_MINIMIZED:
			if (value == RUDA_FALSE) {
				if (structure->currentConfig.minimized) {
					structure->currentConfig.minimized = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!structure->currentConfig.minimized) {
					structure->currentConfig.minimized = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_VISIBLE:
			if (value == RUDA_FALSE) {
				if (structure->currentConfig.visible) {
					structure->currentConfig.visible = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!structure->currentConfig.visible) {
					structure->currentConfig.visible = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_DECORATED:
			if (value == RUDA_FALSE) {
				if (structure->currentConfig.decorated) {
					structure->currentConfig.decorated = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!structure->currentConfig.decorated) {
					structure->currentConfig.decorated = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_AUTO_MINIMIZE:
			if (value == RUDA_FALSE) {
				if (structure->currentConfig.auto_minimize) {
					structure->currentConfig.auto_minimize = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!structure->currentConfig.auto_minimize) {
					structure->currentConfig.auto_minimize = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_FLOATING:
			if (value == RUDA_FALSE) {
				if (structure->currentConfig.floating) {
					structure->currentConfig.floating = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!structure->currentConfig.floating) {
					structure->currentConfig.floating = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_MAXIMIZED:
			if (value == RUDA_FALSE) {
				if (structure->currentConfig.maximized) {
					structure->currentConfig.maximized = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!structure->currentConfig.maximized) {
					structure->currentConfig.maximized = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_CENTER_CURSOR:
			if (value == RUDA_FALSE) {
				if (structure->currentConfig.center_cursor) {
					structure->currentConfig.center_cursor = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!structure->currentConfig.center_cursor) {
					structure->currentConfig.center_cursor = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_TRANSPARENT_FRAMEBUFFER:
			if (value == RUDA_FALSE) {
				if (structure->currentConfig.transparent_framebuffer) {
					structure->currentConfig.transparent_framebuffer = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!structure->currentConfig.transparent_framebuffer) {
					structure->currentConfig.transparent_framebuffer = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_HOVERED:
			if (value == RUDA_FALSE) {
				if (structure->currentConfig.hovered) {
					structure->currentConfig.hovered = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!structure->currentConfig.hovered) {
					structure->currentConfig.hovered = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_FOCUS_ON_SHOW:
			if (value == RUDA_FALSE) {
				if (structure->currentConfig.focus_on_show) {
					structure->currentConfig.focus_on_show = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!structure->currentConfig.focus_on_show) {
					structure->currentConfig.focus_on_show = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_MOUSE_PASSTHROUGH:
			if (value == RUDA_FALSE) {
				if (structure->currentConfig.mouse_passthrough) {
					structure->currentConfig.mouse_passthrough = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!structure->currentConfig.mouse_passthrough) {
					structure->currentConfig.mouse_passthrough = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_SCALE_TO_MONITOR:
			if (value == RUDA_FALSE) {
				if (structure->currentConfig.scale_to_monitor) {
					structure->currentConfig.scale_to_monitor = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!structure->currentConfig.scale_to_monitor) {
					structure->currentConfig.scale_to_monitor = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_DOUBLE_FRAMEBUFFER:
			if (value == RUDA_FALSE) {
				if (structure->currentConfig.double_framebuffer) {
					structure->currentConfig.double_framebuffer = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!structure->currentConfig.double_framebuffer) {
					structure->currentConfig.double_framebuffer = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_SCALE_FRAMEBUFFER:
			if (value == RUDA_FALSE) {
				if (structure->currentConfig.scale_framebuffer) {
					structure->currentConfig.scale_framebuffer = false;
					return HINT_CHANGED;
				}
			} else if (value == RUDA_TRUE) {
				if (!structure->currentConfig.scale_framebuffer) {
					structure->currentConfig.scale_framebuffer = true;
					return HINT_CHANGED;
				}
			} else return HINT_INVALID_VALUE;
			return HINT_UNCHANGED;
		case DI_POSITION_X:
			if (value != structure->currentConfig.x_position) {
				structure->currentConfig.x_position = value;
				return HINT_CHANGED;
			} else return HINT_UNCHANGED;
		case DI_POSITION_Y:
			if (value != structure->currentConfig.y_position) {
				structure->currentConfig.y_position = value;
				return HINT_CHANGED;
			} else return HINT_UNCHANGED;
	}
	return HINT_INVALID_HINT;

}



