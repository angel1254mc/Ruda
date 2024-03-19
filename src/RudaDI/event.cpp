#include "RudaDI/di.h"
#include "RudaDI/di_structs.h"

void print(char* text) {
    std::cout << text << std::endl;
}
// Does any extra stuff the low-level team needs done on key presses/releases, then calls
// the high-level dev callback function
void handleKeyPressed (DI_Window* window, int xKeycode, bool pressed, int mods) {
    
    if (xKeycode == NULL)
        print("handleKeyPressed: keycode cannot be NULL!");\
    if (pressed != true && pressed != false)
        print("handleKeyPressed: pressed has to be true or false!");
    // We simply pass in everything sincne there isn't anything us low-level devs need to do
    // with key press/release events as of yet
    if (window->callbacks.keypress != NULL)
        window->callbacks.keypress(window, xKeycode, pressed, mods);
}

// Translates a mod code (stored in xkey struct from an event) to something readable by high level devs
// for example, shift + 1 where 1 is the keycode and shift is the mod!
static int translateMods(int state) {

    // Bitwise operations for masking MULTIPLE mods
    // so Shift + CTRL + L would have mod for Shift and CTRL
    int mods = 0;
    if (state & ShiftMask) 
        mods |= DI_MOD_SHIFT;
    if (state & ControlMask)
        mods |= DI_MOD_CONTROL;
    if (state & Mod1Mask)
        mods |= DI_MOD_SUPER;
    if (state & LockMask)
        mods |= DI_MOD_CAPS_LOCK;
    if (state & Mod2Mask)
        mods |= DI_MOD_NUM_LOCK;

    return mods;
}

void handleCursorEnterWindow(DI_Window* window, bool entered) {
    if (window == nullptr)
        print("handleCursorEnterWindow: Window cannot be null!");
    if (entered != true && entered != false) 
        print("handleCursorEnterWindow: entered has to be true or false!");
    if (window->callbacks.cursor_enter != NULL)
         return window->callbacks.cursor_enter(window, entered);
}

void handleCursorMovement(DI_Window* window, int x, int y) {
    if (window == NULL)
        print("handleCursorMovement: Window cannot be NULL!");
    
    if (window->callbacks.cursor_pos_change!= NULL)
        window->callbacks.cursor_pos_change(window, x, y);
}

void handleFramebufferSizeChange(DI_Window* window, int width, int height) {
    if (window == NULL)
        print("handleFramebufferSizeChange: Window cannot be NULL!");
    if (width < 0 || height < 0)
        print("handleFramebufferSizeChange: Window width/height cannot be less than 0!");
    if (window->callbacks.fb_resize != NULL)
        window->callbacks.fb_resize(window, width, height);
}

void handleWindowSizeChange(DI_Window* window, int width, int height) {
    if (window == NULL)
        print("handleWindowSizeChange: Window cannot be NULL!");
    if (width < 0 || height < 0)
        print("handleWindowSizeChange: Window width/height cannot be less than 0!");
    if (window->callbacks.resize != NULL)
        window->callbacks.resize(window, width, height);
}

void handleWindowPositionChange(DI_Window* window, int x_pos, int y_pos) {
    if (window == NULL)
        print("handleWindowPositionChange: Window cannot be NULL!");

    if (window->callbacks.window_pos_change != NULL)
        window->callbacks.window_pos_change(window, x_pos, y_pos);
}

void handleCloseWindowRequest(DI_Window* window) {
    if (window  == NULL)
        print("handleCloseWindowRequest: Window cannot be NULL!");
    window->shouldClose = true;
    if (window->callbacks.close != NULL)
        window->callbacks.close(window);
}

void processEvent(XEvent* event) {
    // handles the if/else structure to decide what callback to invoke

    // Find the DI_Window object that corresponds to this window
    DI_Window* window = nullptr;
    XFindContext(
            structure->display->xDisplay, 
            event->xany.window, 
            structure->ctx->xStore,
            (XPointer*) &window);

    switch(event->type) {
        case KeyPress:
        {
            int keycode = event->xkey.keycode;
            bool pressed = true;
            int mods = translateMods(event->xkey.state);
            // [TO DO @angel1254mc] investigate 'mods' param
            // Mods apparently describe key combinations, such as ctrl / shift / caps
            // So for instance, you could theoretically pass in mods like 
            // ctrl + shift along with the keycode of v
            // don't think it needs to be implemented for the basic demo but will look into this

            // Trigger the INTERNAL callback for key events
            handleKeyPressed(window, keycode, pressed, mods);
            return;
        }
        case KeyRelease:
        {
            int keycode = event->xkey.keycode;
            bool pressed = false;
            int mods = translateMods(event->xkey.state);

            // Key release events are a little quirky due to the fact that when
            // a key is pressed for a long time, the server sends pairs of KeyPress/KeyRelease
            // events even if the user has not released the key

            // We have to discard these extra events and just return something that denotes
            // the key has been repeated/left pressed
            // See GLFW Source Code for additional details https://github.com/glfw/glfw/blob/dc557ecf38a42b0b93898a7aef69f6dc48bf0e57/src/x11_window.c#L1320-L1349

            if (XEventsQueued(structure->display->xDisplay, QueuedAfterReading)) {
                // Check the next event, see if it's a keypress event on the same window
                XEvent next;
                XPeekEvent(structure->display->xDisplay, &next);
                if (next.type == KeyPress && next.xkey.window == event->xkey.window && next.xkey.keycode == event->xkey.keycode) {
                    // If time is nearly identical, this is veeeeery likely a key repeat and we can ignore it
                    if ((next.xkey.time - event->xkey.time) < 20)
                        return;
                }
            }
            // Trigger the INTERNAL callback for key events
            handleKeyPressed(window, keycode, pressed, mods);
            return;
        }
        case ButtonPress:
        {
            int mods = translateMods(event->xbutton.state);
            // handle left, right, middle click (1, 2, 3 respectively)
            if (window->callbacks.mouse_click != NULL 
                && (event->xbutton.button >= Button1 
                && event->xbutton.button <= Button3))
                window->callbacks.mouse_click(window, event->xbutton.button, true, mods);
            // handle scroll
            if (window->callbacks.scroll != NULL) {
                if (event->xbutton.button == Button4) {
                    window->callbacks.scroll(window, 0, 1.0);
                } else if (event->xbutton.button == Button5) {
                    window->callbacks.scroll(window, 0, -1.0);
                }
            }
            return;
        }
        case ButtonRelease:
        {
            int mods = translateMods(event->xbutton.state);
            // For left click, middle click, and right click
            if (window->callbacks.mouse_click != NULL && (event->xbutton.button >= Button1 && event->xbutton.button <= Button3))
                window->callbacks.mouse_click(window, event->xbutton.button, false, mods);
            return;
        }
        case EnterNotify:
        {
            // This is for when the mouse pointer enters the current window
            // https://tronche.com/gui/x/xlib/events/window-entry-exit/
            // Window entry also constites a mouse motion event! (XCrossingEvent)
            const int x = event->xcrossing.x;
            const int y = event->xcrossing.y;
            
            handleCursorEnterWindow(window, true);
            handleCursorMovement(window, x, y);
            return;
        }
        case LeaveNotify:
        {
            handleCursorEnterWindow(window, false);
            return;
        }
        case MotionNotify:
        {
            int x = event->xmotion.x;
            int y = event->xmotion.y;

           
            handleCursorMovement(window, x, y);
            return;
        }
        case ConfigureNotify:
        {
            if (event->xconfigure.width != window->config.width || event->xconfigure.height != window->config.height) {
                window->config.width = event->xconfigure.width;
                window->config.height = event->xconfigure.height;

                handleFramebufferSizeChange(window, event->xconfigure.width, event->xconfigure.height);
                handleWindowSizeChange(window, event->xconfigure.width, event->xconfigure.height);
            }

            int x_pos = event->xconfigure.x;
            int y_pos = event->xconfigure.y;

            if (!event->xany.send_event && window->parent && window->parent->xWindow != structure->root) {
                XWindow dummy;
                XTranslateCoordinates(structure->display->xDisplay,
                                    window->xWindow,
                                    structure->root,
                                    x_pos, y_pos,
                                    &x_pos, &y_pos,
                                    &dummy);
            }

            if (x_pos != window->config.get_x_position() || y_pos != window->config.get_y_position()) {
                window->config.set_x_position(x_pos);
                window->config.set_y_position(y_pos);

                handleWindowPositionChange(window, x_pos, y_pos);
            }

            return;
        }
        case ClientMessage:
        {
            // Client messages from the window manager
            // Useful for detecting dnd events, window close events
            // and selection events too
            if (event->xclient.message_type == XInternAtom(structure->display->xDisplay, "WM_PROTOCOLS", false)) {
                // Get the specific protocol method that the window manager is 
                // telling us to perform
                const Atom protocol = event->xclient.data.l[0];

                if (protocol == XInternAtom(structure->display->xDisplay, "WM_DELETE_WINDOW", false)) {
                    handleCloseWindowRequest(window);
                }
            }
        }
        default:
        {
            print("Do Nothing because event type was not recognized");
            break;
        }
    }
    return;
}


void diPollEvents() {
    // The point of polling events is to hit the X Window server 
    // asking it if any events have been queued

    // While events exist that haven't been handled (QLength gives us this)
    while (QLength(structure->display->xDisplay))
    {
        XEvent event;
        XNextEvent(structure->display->xDisplay, &event);
        // process individual event
        processEvent(&event);
    }
    // Update client changes to the x11 server :D
    XSync(structure->display->xDisplay, false);
}