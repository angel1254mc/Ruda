#include "../../include/Ruda/RudaDI/di.h"
#include "../../include/Ruda/RudaDI/di_structs.h"


windowResizeCallback diSetWindowResizeCallback(DI_Window* window, windowResizeCallback callbackFunc) {
    windowResizeCallback temp = window->callbacks.resize;
    window->callbacks.resize = callbackFunc;

    return temp;
};

windowCloseCallback diSetWindowCloseCallback(DI_Window* window, windowCloseCallback callbackFunc) {
    windowCloseCallback temp = window->callbacks.close;
    window->callbacks.close = callbackFunc;
    return temp;
}

windowFocusCallback diSetWindowFocusCallback(DI_Window* window, windowFocusCallback callbackFunc) {
    windowFocusCallback temp = window->callbacks.focus;
    window->callbacks.focus = callbackFunc;
    return temp;
}

windowIconifyCallback diSetWindowIconifyCallback(DI_Window* window, windowIconifyCallback callbackFunc) {
    windowIconifyCallback temp = window->callbacks.iconify;
    window->callbacks.iconify = callbackFunc;
    return temp;
}

windowMaximizeCallback diSetWindowMaximizeCallback(DI_Window* window, windowMaximizeCallback callbackFunc)  {
    windowMaximizeCallback temp = window->callbacks.maximize;
    window->callbacks.maximize = callbackFunc;
    return temp;
}

windowFramebufferSizeCallback diSetFramebufferSizeCallback(DI_Window* window, windowFramebufferSizeCallback callbackFunc) {
    windowFramebufferSizeCallback temp = window->callbacks.fb_resize;
    window->callbacks.fb_resize = callbackFunc;
    return temp;
}

windowContentScaleCallback diSetWindowContentScaleCallback(DI_Window* window, windowContentScaleCallback callbackFunc) {
    windowContentScaleCallback temp = window->callbacks.content_scale_resize;
    window->callbacks.content_scale_resize = callbackFunc;
    return temp;
}

windowEnterCallback diSetCursorEnterCallback(DI_Window* window, windowEnterCallback callbackFunc) {
    windowEnterCallback temp = window->callbacks.cursor_enter;
    window->callbacks.cursor_enter = callbackFunc;
    return temp;
}

keyCallback diSetKeyCallback(DI_Window* window, keyCallback callbackFunc) {
    keyCallback temp = window->callbacks.keypress;
    window->callbacks.keypress = callbackFunc;
    return temp;
}

mouseButtonCallback diSetMouseButtonCallback(DI_Window* window, mouseButtonCallback callbackFunc) {
    mouseButtonCallback temp = window->callbacks.mouse_click;
    window->callbacks.mouse_click = callbackFunc;
    return temp;
}

cursorPosCallback diSetCursorPosCallback(DI_Window* window, cursorPosCallback callbackFunc) {
    cursorPosCallback temp = window->callbacks.cursor_pos_change;
    window->callbacks.cursor_pos_change = callbackFunc;
    return temp;
}

windowPosCallback diSetWindowPosCallback(DI_Window* window, windowPosCallback callbackFunc) {
    windowPosCallback temp = window->callbacks.window_pos_change;
    window->callbacks.window_pos_change = callbackFunc;
    return temp;
}

scrollCallback diSetScrollCallback(DI_Window* window, scrollCallback callbackFunc) {
    scrollCallback temp = window->callbacks.scroll;
    window->callbacks.scroll = callbackFunc;
    return temp;
}

