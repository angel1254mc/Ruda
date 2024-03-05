
#pragma once
#include "structs.h"
#include "../Util/util.h"

// ------------------------------- MACROS ------------------------------- //

// Shorthand for std::string
#define str std::string

// Abstracts the Window struct from X11 to XWindow to avoid ambiguity
#define XWindow Window

// Abstracts the Visual struct from X11 to XVisual to avoid ambiguity
#define XVisual Visual

// Abstracts the Display struct from X11 to XDisplay to avoid ambiguity
#define XDisplay Display

// Abstracts the Graphics Context struct from X11 to XGC to avoid ambiguity
#define XGC GC

#define RUDA_TRUE 1

#define RUDA_FALSE 0