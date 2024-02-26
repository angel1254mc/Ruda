#include "../../include/Ruda/RudaDI/di.h"

void diPollEvents() {
    // The point of polling events is to hit the X Window server 
    // asking it if any events have been queued
    // Then we loop over each event and call the necessary handler
    // The handlers are stored in the RudaDI object, and can be set with
    // These include
    /**
     *      void rudaSetKeyCallback(RudaDIWindow* window,  void(*callbackFunc)(RudaDIWindow, int, int, int, int));
     *      void rudaSetMouseButtonCallback(RudaDIWindow* window, void(*callbackFunc)(RudaDIWindow* window, int button, int action, int mods));
     *      void rudaSetCursorPosCallback(RudaDIWindow* window, void(*callbackFunc)(RudaDIWindow* window, double xpos, double ypos));
    */
}

/* void diProcessEvent(XEvent* event) {
    // handles the if/else structure to decide what callback to invoke
    switch(event->type) {
        case KeyPress:
            int keycode = event->xkey.keycode;
            bool pressed = true;
            int mods = 0; // Idk even know what mods means in this context
            // [TO DO @angel1254mc] investigate 'mods' param
            // Mods apparently describe key combinations, such as ctrl / shift / caps
            // So for instance, you could theoretically pass in mods like 
            // ctrl + shift along with the keycode of v
            // don't think it needs to be implemented for the basic demo but will look into this

            // Trigger the keyCallback with all necessary info
            if (structure->currentWindow->keyCallback != nullptr)
                structure->currentWindow->keyCallback(structure->currentWindow, keycode, pressed, mods);
            break;
        case KeyRelease:
            int keycode = event->xkey.keycode;
            bool pressed = false;
            int mods = 0;
            if (structure->currentWindow->keyCallback != nullptr)
                structure->currentWindow->keyCallback(structure->currentWindow, keycode, pressed, mods);
            break;
        case ButtonPress:
            // handle left, right, middle click (1, 2, 3 respectively)
            if (structure->currentWindow->mouseButtonCallback != nullptr && (event->xbutton.button >= Button1 && event->xbutton.button <= Button3))
                structure->currentWindow->mouseButtonCallback(structure->currentWindow, event->xbutton.button, true, 0);
            // handle scroll
            if (structure->currentWindow->scrollCallback != nullptr) {
                if (event->xbutton.button == Button4) {
                    structure->currentWindow->scrollCallback(structure->currentWindow, 0, 1.0);
                } else if (event->xbutton.button == Button5) {
                    structure->currentWindow->scrollCallback(structure->currentWindow, 0, -1.0);
                }
            }
            break;
        case ButtonRelease:
            if (structure->currentWindow->mouseButtonCallback != nullptr && (event->xbutton.button >= Button1 && event->xbutton.button <= Button3))
                structure->currentWindow->mouseButtonCallback(structure->currentWindow, event->xbutton.button, false, 0);
            break;
         case MotionNotify:
            int x = event->xmotion.x;
            int y = event->xmotion.y;

            if (structure->currentWindow->cursorPosCallback) {
                structure->currentWindow->cursorPosCallback(structure->currentWindow, x, y);
            }
            break;
        case ConfigureNotify:
            // Handle resizes and stuff here.
            break;
    }

}
 */