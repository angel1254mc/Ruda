#include "../../include/Ruda/RudaDI/di.h"
#include "../../include/Ruda/RudaDI/di_structs.h"
// Does any extra stuff the low-level team needs done on key presses/releases, then calls
// the high-level dev callback function
void handleKeyPressed (DI_Window* window, int xKeycode, bool pressed, int mods) {
    
    if (xKeycode == NULL)
        print("handleKeyPressed: keycode cannot be NULL!");\
    if (pressed != true && pressed != false)
        print("handleKeyPressed: pressed has to be true or false!");
    if (mods == NULL)
        print("handleKeyPressed: Mods cannot be NULL!");
    // We simply pass in everything sincne there isn't anything us low-level devs need to do
    // with key press/release events as of yet
    if (window->callbacks.keyCallback != nullptr)
       window->callbacks.keyCallback(window, xKeycode, pressed, mods);
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
    // if (window->callbacks.cursorEnter != nullptr)
    //     return window->callbacks.cursorEnterCallback(window, entered)
}

void handleCursorMovement(DI_Window* window, int x, int y) {
    if (window == NULL)
        print("handleCursorMovement: Window cannot be NULL!");
    
    if (window->callbacks.cursorPosCallback != nullptr)
        window->callbacks.cursorPosCallback(window, x, y);
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
    XFlush(structure->display->xDisplay);
}

void diProcessEvent(XEvent* event) {
    // handles the if/else structure to decide what callback to invoke

    // Find the DI_Window object that corresponds to this window
    DI_Window* window = nullptr;
    XFindContext(
            structure->display->xDisplay, 
            event->xany.window, 
            structure->xContext,
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
            break;
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
                if (next.type == KeyPress && next.xkey.window == event->xkey.window && next.xkey.keycode) {
                    // If time is nearly identical, this is veeeeery likely a key repeat and we can ignore it
                    if (next.xkey.time - event->xkey.time < 20)
                        return;
                }
            }
            // Trigger the INTERNAL callback for key events
            handleKeyPressed(window, keycode, pressed, mods);
            break;
        }
        case ButtonPress:
        {
            int mods = translateMods(event->xbutton.state);
            // handle left, right, middle click (1, 2, 3 respectively)
            if (window->callbacks.mouseButtonCallback != nullptr 
                && (event->xbutton.button >= Button1 
                && event->xbutton.button <= Button3))
                window->callbacks.mouseButtonCallback(window, event->xbutton.button, true, mods);
            // handle scroll
            if (window->callbacks.scrollCallback != nullptr) {
                if (event->xbutton.button == Button4) {
                    window->callbacks.scrollCallback(window, 0, 1.0);
                } else if (event->xbutton.button == Button5) {
                    window->callbacks.scrollCallback(window, 0, -1.0);
                }
            }
            break;
        }
        case ButtonRelease:
        {
            int mods = translateMods(event->xbutton.state);
            // For left click, middle click, and right click
            if (window->callbacks.mouseButtonCallback != nullptr && (event->xbutton.button >= Button1 && event->xbutton.button <= Button3))
                window->callbacks.mouseButtonCallback(window, event->xbutton.button, false, mods);
            break;
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
            break;
        }
        case LeaveNotify:
        {
            handleCursorEnterWindow(window, false);
            break;
        }
        case MotionNotify:
        {
            int x = event->xmotion.x;
            int y = event->xmotion.y;
           
            if (window->callbacks.cursorPosCallback) {
                window->callbacks.cursorPosCallback(window, x, y);
            }
            break;
        }
        case ConfigureNotify:
        {
            // Handle resizes and stuff here.
            print("Testing this real quick");
            break;
        }
        default:
        {
            print("Do Nothing because event type was not recognized");
            break;
        }
    }
    return;
}
