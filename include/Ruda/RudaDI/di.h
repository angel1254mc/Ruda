#pragma once
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include "../Util/color.h"
#include "../Util/util.h"
#include "../Ruda/ruda.h"


typedef struct DI_Window DI_Window;
typedef struct DI_Cursor DI_Cursor;
typedef struct DI_Monitor DI_Monitor;
typedef struct DI_Display DI_Display;
typedef struct DI_WindowConfig DI_WindowConfig;
typedef struct DI_Structure DI_Structure;


// ------------------------------- MACROS ------------------------------- //


#define 	DI_MAINTAIN_ASPECT_RATIO	0x00020000
	// Maintain aspect ratio on resize attribute.

#define 	DI_FOCUSED   				0x00020001
 	// Input focus window hint and attribute.
 
#define 	DI_MINIMIZED				0x00020002
 	// Window minimization window attribute.
 
#define 	DI_RESIZABLE				0x00020003
 	// Window resize-ability window hint and attribute.
 
#define 	DI_VISIBLE					0x00020004
 	// Window visibility window hint and attribute.
 
 #define 	DI_DECORATED				0x00020005
 	// Window decoration window hint and attribute.
 
#define 	DI_AUTO_MINIMIZE			0x00020006
 	// Window auto-iconification window hint and attribute.
 
#define 	DI_FLOATING					0x00020007
 	// Window decoration window hint and attribute.
 
#define 	DI_MAXIMIZED				0x00020008
 	// Window maximization window hint and attribute.
 
#define 	DI_CENTER_CURSOR			0x00020009
 	// Cursor centering window hint.
 
#define 	DI_TRANSPARENT_FRAMEBUFFER	0x0002000A
 	// Window framebuffer transparency hint and attribute.
 
#define 	DI_HOVERED					0x0002000B
 	// Mouse cursor hover window attribute.
 
#define 	DI_FOCUS_ON_SHOW			0x0002000C
 	// Input focus on calling show window hint and attribute.
 
#define 	DI_MOUSE_PASSTHROUGH		0x0002000D
 	// Mouse input transparency window hint and attribute.
 
#define 	DI_POSITION_X				0x0002000E
 	// Initial position x-coordinate window hint.
 
#define 	DI_POSITION_Y				0x0002000F
 	// Initial position y-coordinate window hint.

#define 	DI_REFRESH_RATE				0x00020010
 	// Monitor refresh rate hint.
 
#define 	DI_DOUBLE_FRAMEBUFFER		0x00020011
 	// Framebuffer double buffering hint and attribute.
 
#define 	DI_SCALE_TO_MONITOR   		0x00020012
 	// Window content area scaling window window hint.
 
#define 	DI_X11_INSTANCE_NAME		0x00020013
 	// X11 specific window hint.

#define 	DI_X11_CLASS_NAME			0x00020014
 	// X11 specific window hint.

#define		DI_SCALE_FRAMEBUFFER		0x00020015
	// Scale framebuffer windowh hint.



extern DI_Structure* structure; //global structure referenced everywhere
DI_Structure* diGetStructure();
extern DI_WindowConfig defaultWindowConfig;



// ------------------------------- CALLBACKS.CPP ------------------------------- //
//TO DO:
//void diInitHint(); Sets hints for initialization
//bool diTerminate();
	
/* Callback functions to be implemented at a later date */
typedef void (*windowResizeCallback)(DI_Window*, int, int);
typedef void (*windowCloseCallback)(DI_Window*);
typedef void (*windowFocusCallback)(DI_Window*);
typedef void (*windowIconifyCallback)(DI_Window*, int);
typedef void (*windowMaximizeCallback)(DI_Window*, int);
typedef void (*windowFramebufferSizeCallback)(DI_Window*, float, float);
typedef void (*windowContentScaleCallback)(DI_Window*, float, float);
typedef void (*windowEnterCallback)(DI_Window*, bool);
typedef void (*windowPosCallback)(DI_Window*, int, int);
typedef void (*keyCallback)(DI_Window*, int, bool, int);
typedef void (*mouseButtonCallback)(DI_Window* window, int button, bool pressed, int mods);
typedef void (*cursorPosCallback)(DI_Window* window, double xpos, double ypos);
typedef void (*scrollCallback)(DI_Window* window, double xoffset, double yoffset);
typedef void (*cursorEnterCallback)(DI_Window* window, bool entered);
/**
 * Calls "terminate" if necessary, initializes
 * default DI_ configuration and creates all internal
 * default DI_ configuration and creates all internal
 * memory objects needed to get started w/ creating windows and such
*/

// ------------------------------- INIT.CPP ------------------------------- //

bool diInit();


// ------------------------------- MONITOR.CPP ------------------------------- //

DI_Monitor diGetMonitors(int* count);
DI_Monitor diGetPrimaryMonitor(int* count);

// ------------------------------- DI_WINDOW.CPP ------------------------------- //

/// @brief Resets all window hints to default values
void diDefaultWindowHints();
/// @brief Sets specific window hint to value
/// @param hint integer/enum representation of hint to set
/// @param value integer/enum representation of hint value
#define HINT_CHANGED		1
#define HINT_UNCHANGED		2
#define HINT_INVALID_VALUE	3
#define HINT_INVALID_HINT 	4
int diWindowHint(unsigned short hint, int value);

/// @brief Sets specific window hint to value
/// @param width width of window
/// @param height height of window
/// @param title std::string title of window
/// @param monitor Monitor on which to create window
/// @param window OPTIONAL additional window to share resources with
/// @returns DI_Window object corresponding to newly created window
DI_Window* diCreateWindow(const str title, unsigned int width, unsigned int height, DI_Monitor* monitor, DI_Window* parentWindow);
DI_Window* diCreateWindow(const str title);
/// @brief Destroys window and associated resources
/// @param window DI_Window object to destroy
/// @param window DI_Window object to destroy
void diDestroyWindow(DI_Window* window);

/// @brief Tracks state of whether or not window should close, useful for polling on while loops
/// @param window window we are checking
/// @return boolean true or false
bool diWindowShouldClose(DI_Window* window);

/// @brief Set whether window should close for next poll
/// @param window window we are setting value for
/// @param value True for yes, False for no
void diSetWindowShouldClose(DI_Window* window, bool value);

/// @brief returns title of window
/// @param window window we are getting title of
/// @returns title std::string
std::string diGetWindowTitle(DI_Window* window);

/// @brief returns title of window
/// @param window window we are setting title of
/// @param title std::string title we set window to
void diSetWindowTitle(DI_Window* window, std::string title);

void diSetWindowIcon(DI_Window* window,  std::string imagePath);

int* diGetWindowPos(DI_Window* window);

void diSetWindowPos(DI_Window* window, int x, int y);

int* diGetWindowSize(DI_Window* window);
void diGetWindowSize(DI_Window* window, int* width, int* height); 

void diSetWindowSize(DI_Window* window, int width, int height);


bool isVisualTransparent(Visual* visual);


// https://github.com/glfw/glfw/blob/8f2f766f0d2ed476c03a2ae02e48ac41a9602b03/include/GLFW/glfw3.h#L3700

/// @brief Retrieves the size of the frame of the window, including in-screen coords of each edge of the frame
/// @param window window to retrieve size for
/// @return array of int values, corresponding to left, top, right, and bottom edge.
int* diGetWindowFrameSize(DI_Window* window);
// Minify window
void diIconifyWindow(DI_Window* window);
// Maximize window
void diMaximizeWindow(DI_Window* window);
// Restore window to non-maxified non-minified state
void diRestoreWindow(DI_Window* window);

void diShowWindow(DI_Window* window);

void diHideWindow(DI_Window* window);

void diFocusWindow(DI_Window* window);

void diRequestWindowAttention(DI_Window* window);

/// @brief Returns the monitor that the window uses for full screen mode.
/// @param window window whose monitor handle we are trying to get
/// @return monitor handle that corresponds to fullscreened window
DI_Monitor diGetWindowMonitor(DI_Window window);

/// @brief Sets the mode, monitor, video mode, and placement of a window that uses fullscreen mode
/// @param window DI_ window object
/// @param monitor DI_ monitor object
/// @param xpos x position of window
/// @param ypos y position of window
/// @param width width of window (screen coords)
/// @param height height of window (screen cords_)
/// @param refreshRate desired refresh rate of mode (optional)
void diSetWindowMonitor(DI_Window* window, DI_Monitor* monitor, int xpos, int ypos, int width, int height, int refreshRate);

/// @brief returns the value of an attribute of the specified window or its di context.
/// @param window window object
/// @param attribute int/enum representation of Ruda context attribute
/// @return attribute value ( as an int/enum)
int diGetWindowAttrib(DI_Window* window, int attribute);

void diSetWindowAttrib(DI_Window* window, int attribute, int value);

/// @brief Sets function as callback on window resizes
/// @param window window which we are setting callback for
/// @param callbackFunc the callback func, which is given the width and height of content window post-resize
windowResizeCallback diSetWindowSizeCallback(DI_Window* window, windowResizeCallback callbackFunc);

windowCloseCallback diSetWindowCloseCallback(DI_Window* window, windowCloseCallback callbackFunc);

windowFocusCallback diSetWindowFocusCallback(DI_Window* window, windowFocusCallback callbackFunc);

windowIconifyCallback diSetWindowIconifyCallback(DI_Window* window, windowIconifyCallback callbackFunc);

windowMaximizeCallback diSetWindowMaximizeCallback(DI_Window* window, windowMaximizeCallback callbackFunc);

// Pretty sure this is relevant for resizing textures/images attached to a framebuffer since it doesn't get done automatically
windowFramebufferSizeCallback diSetFramebufferSizeCallback(DI_Window* window, windowFramebufferSizeCallback callbackFunc);

windowContentScaleCallback diSetWindowContentScaleCallback(DI_Window* window, windowContentScaleCallback callbackFunc);

windowEnterCallback diSetCursorEnterCallback(DI_Window* window, windowEnterCallback callbackFunc);

// ------------------------------- EVENT.CPP ------------------------------- // 


/// @brief processes all pending events in queue (event listening on another thread, implemented by x11, see _glfwPollEventsX11)
void diPollEvents();

/// @brief Waits for events (with timeout of 1000 by default) and blocks until an event is encountered (then handles them)
void diWaitForEvents();

/// @brief Waits for events (with customizable timeout and blocks until an event is encountered (then handles them)
void diWaitForEvents(double timeoutMs);


//void diPostEmptyEvent();

//int diGetInputMode(DI_Window* window, int mode);

//void diSetInputMode(DI_Window* window, int mode, int value)

//int diRawMouseMotionSupported();

//std::string diGetKeyName(int key, int scancode);

//int diGetKeyScancode(int key);

//int diGetKey(DI_Window* window, int key);

//int diGetMouseButton(DI_Window* window, int button);


/// @brief Retrieves mouse position on window content area (starting from top left)
/// @param window DI_Window object
/// @return array of double values, corresponding to xpos and ypos of where the cursor is relative to left edge of content area
double* diGetMouseButton(DI_Window* window);


DI_Cursor* diCreateStandardCursor();

void diDestroyCursor(DI_Cursor* cursor);

void diSetCursor(DI_Window* window, DI_Cursor* cursor);

/// @brief
/// @param window 
/// @param callbackFunc 
/// @callback_Signature
/// func(DI_Window window, int key, int action, int mods????)

keyCallback diSetKeyCallback(DI_Window* window,  keyCallback callbackFunc);

mouseButtonCallback diSetMouseButtonCallback(DI_Window* window, mouseButtonCallback callbackFunc);

cursorPosCallback diSetCursorPosCallback(DI_Window* window, cursorPosCallback callbackFunc);

scrollCallback diSetScrollCallback(DI_Window* window, scrollCallback callbackFunc);



// Finally context related functions 🥲🥲🥲
void diMakeContextCurrent(DI_Window* window);

DI_Window* diGetCurrentContext();

void diSwapBuffers(DI_Window* window);

// This is mainly for vsync. See https://github.com/glfw/glfw/blob/8f2f766f0d2ed476c03a2ae02e48ac41a9602b03/include/GLFW/glfw3.h#L6168
void diSwapInterval(int interval);


void processEvent(XEvent* event);

// ------------------------------- DRAW.CPP ------------------------------- //

void diDrawLine (unsigned int x_origin, unsigned int y_origin, unsigned int x_dest, unsigned int y_dest, Color color);

void diDrawLine (unsigned int x_origin, unsigned int y_origin, unsigned int x_dest, unsigned int y_dest, unsigned int r, unsigned int g, unsigned int b);

void diClearWindow();

void diDrawRectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, Color color);

void diDrawRectangle(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int r, unsigned int g, unsigned int b);

void diFlush();

// ------------------------------- KEYCODES ------------------------------- //

// Mouse Buttons
#define DI_LMB_CLICK Button1
#define DI_MIDDLE_CLICK Button2
#define DI_RIGHT_CLICK Button3

// Mods = modifiers such as shift and ctrl
#define DI_MOD_SHIFT 0x0001
#define DI_MOD_CONTROL 0x0002
#define DI_MOD_ALT 0x0004
#define DI_MOD_SUPER 0x0008
#define DI_MOD_CAPS_LOCK 0x0010
#define DI_MOD_NUM_LOCK 0x0020	

// This is for the high-level devs to know what key each keycode correponds to
#define DI_ESCAPE 9
#define DI_1 10
#define DI_2 11
#define DI_3 12
#define DI_4 13
#define DI_5 14
#define DI_6 15
#define DI_7 16
#define DI_8 17
#define DI_9 18
#define DI_0 19
#define DI_MINUS 20
#define DI_EQUAL 21
#define DI_BACKSPACE 22
#define DI_TAB 23
#define DI_Q 24
#define DI_W 25
#define DI_E 26
#define DI_R 27
#define DI_T 28
#define DI_Y 29
#define DI_U 30
#define DI_I 31
#define DI_O 32
#define DI_P 33
#define DI_BRACKETLEFT 34
#define DI_BRACKETRIGHT 35
#define DI_RETURN 36
#define DI_CONTROL_L 37
#define DI_A 38
#define DI_S 39
#define DI_D 40
#define DI_F 41
#define DI_G 42
#define DI_H 43
#define DI_J 44
#define DI_K 45
#define DI_L 46
#define DI_SEMICOLON 47
#define DI_APOSTROPHE 48
#define DI_GRAVE 49
#define DI_SHIFT_L 50
#define DI_BACKSLASH 51
#define DI_Z 52
#define DI_X 53
#define DI_C 54
#define DI_V 55
#define DI_B 56
#define DI_N 57
#define DI_M 58
#define DI_COMMA 59
#define DI_PERIOD 60
#define DI_SLASH 61
#define DI_SHIFT_R 62
#define DI_KP_MULTIPLY 63
#define DI_ALT_L 64
#define DI_SPACE 65
#define DI_CAPS_LOCK 66
#define DI_F1 67
#define DI_F2 68
#define DI_F3 69
#define DI_F4 70
#define DI_F5 71
#define DI_F6 72
#define DI_F7 73
#define DI_F8 74
#define DI_F9 75
#define DI_F10 76
#define DI_NUM_LOCK 77
#define DI_SCROLL_LOCK 78
#define DI_KP_HOME 79
#define DI_KP_UP 80
#define DI_KP_PRIOR 81
#define DI_KP_SUBTRACT 82
#define DI_KP_LEFT 83
#define DI_KP_BEGIN 84
#define DI_KP_RIGHT 85
#define DI_KP_ADD 86
#define DI_KP_END 87
#define DI_KP_DOWN 88
#define DI_KP_NEXT 89
#define DI_KP_INSERT 90
#define DI_KP_DELETE 91
#define DI_ISO_LEVEL3_SHIFT 92
#define DI_ 93
#define DI_LESS 94
#define DI_F11 95
#define DI_F12 96
#define DI_ 97
#define DI_KATAKANA 98
#define DI_HIRAGANA 99
#define DI_HENKAN_MODE 100
#define DI_HIRAGANA_KATAKANA 101
#define DI_MUHENKAN 102
#define DI_ 103
#define DI_KP_ENTER 104
#define DI_CONTROL_R 105
#define DI_KP_DIVIDE 106
#define DI_PRINT 107
#define DI_ALT_R 108
#define DI_LINEFEED 109
#define DI_HOME 110
#define DI_UP 111
#define DI_PRIOR 112
#define DI_LEFT 113
#define DI_RIGHT 114
#define DI_END 115
#define DI_DOWN 116
#define DI_NEXT 117
#define DI_INSERT 118
#define DI_DELETE 119
#define DI_ 120
#define DI_XF86AUDIOMUTE 121
#define DI_XF86AUDIOLOWERVOLUME 122
#define DI_XF86AUDIORAISEVOLUME 123
#define DI_XF86POWEROFF 124
#define DI_KP_EQUAL 125
#define DI_PLUSMINUS 126
#define DI_PAUSE 127
#define DI_XF86LAUNCHA 128
#define DI_KP_DECIMAL 129
#define DI_HANGUL 130
#define DI_HANGUL_HANJA 131
#define DI_SUPER_L 133
#define DI_SUPER_R 134
#define DI_MENU 135
#define DI_CANCEL 136
#define DI_REDO 137
#define DI_SUNPROPS 138
#define DI_UNDO 139
#define DI_SUNFRONT 140
#define DI_XF86COPY 141
#define DI_XF86OPEN 142
#define DI_XF86PASTE 143
#define DI_FIND 144
#define DI_XF86CUT 145
#define DI_HELP 146
#define DI_XF86MENUKB 147
#define DI_XF86CALCULATOR 148
#define DI_XF86SLEEP 150
#define DI_XF86WAKEUP 151
#define DI_XF86EXPLORER 152
#define DI_XF86SEND 153
#define DI_XF86XFER 155
#define DI_XF86LAUNCH1 156
#define DI_XF86LAUNCH2 157
#define DI_XF86WWW 158
#define DI_XF86DOS 159
#define DI_XF86SCREENSAVER 160
#define DI_XF86ROTATEWINDOWS 161
#define DI_XF86TASKPANE 162
#define DI_XF86MAIL 163
#define DI_XF86FAVORITES 164
#define DI_XF86MYCOMPUTER 165
#define DI_XF86BACK 166
#define DI_XF86FORWARD 167
#define DI_XF86EJECT 169
#define DI_XF86EJECT 170
#define DI_XF86AUDIONEXT 171
#define DI_XF86AUDIOPLAY 172
#define DI_XF86AUDIOPREV 173
#define DI_XF86AUDIOSTOP 174
#define DI_XF86AUDIORECORD 175
#define DI_XF86AUDIOREWIND 176
#define DI_XF86PHONE 177
#define DI_XF86TOOLS 179
#define DI_XF86HOMEPAGE 180
#define DI_XF86RELOAD 181
#define DI_XF86CLOSE 182
#define DI_XF86SCROLLUP 185
#define DI_XF86SCROLLDOWN 186
#define DI_PARENLEFT 187
#define DI_PARENRIGHT 188
#define DI_XF86NEW 189
#define DI_REDO 190
#define DI_XF86TOOLS 191
#define DI_XF86LAUNCH5 192
#define DI_XF86LAUNCH6 193
#define DI_XF86LAUNCH7 194
#define DI_XF86LAUNCH8 195
#define DI_XF86LAUNCH9 196
#define DI_XF86AUDIOMICMUTE 198
#define DI_XF86TOUCHPADTOGGLE 199
#define DI_XF86TOUCHPADON 200
#define DI_XF86TOUCHPADOFF 201
#define DI_MODE_SWITCH 203
#define DI_NOSYMBOL 204
#define DI_NOSYMBOL 205
#define DI_NOSYMBOL 206
#define DI_NOSYMBOL 207
#define DI_XF86AUDIOPLAY 208
#define DI_XF86AUDIOPAUSE 209
#define DI_XF86LAUNCH3 210
#define DI_XF86LAUNCH4 211
#define DI_XF86LAUNCHB 212
#define DI_XF86SUSPEND 213
#define DI_XF86CLOSE 214
#define DI_XF86AUDIOPLAY 215
#define DI_XF86AUDIOFORWARD 216
#define DI_PRINT 218
#define DI_XF86WEBCAM 220
#define DI_XF86AUDIOPRESET 221
#define DI_XF86MAIL 223
#define DI_XF86MESSENGER 224
#define DI_XF86SEARCH 225
#define DI_XF86GO 226
#define DI_XF86FINANCE 227
#define DI_XF86GAME 228
#define DI_XF86SHOP 229
#define DI_CANCEL 231
#define DI_XF86MONBRIGHTNESSDOWN 232
#define DI_XF86MONBRIGHTNESSUP 233
#define DI_XF86AUDIOMEDIA 234
#define DI_XF86DISPLAY 235
#define DI_XF86KBDLIGHTONOFF 236
#define DI_XF86KBDBRIGHTNESSDOWN 237
#define DI_XF86KBDBRIGHTNESSUP 238
#define DI_XF86SEND 239
#define DI_XF86REPLY 240
#define DI_XF86MAILFORWARD 241
#define DI_XF86SAVE 242
#define DI_XF86DOCUMENTS 243
#define DI_XF86BATTERY 244
#define DI_XF86BLUETOOTH 245
#define DI_XF86WLAN 246
#define DI_XF86UWB 247
#define DI_XF86NEXT_VMODE 249
#define DI_XF86PREV_VMODE 250
#define DI_XF86MONBRIGHTNESSCYCLE 251
#define DI_XF86BRIGHTNESSAUTO 252
#define DI_XF86DISPLAYOFF 253
#define DI_XF86WWAN 254
#define DI_XF86RFKILL 255

