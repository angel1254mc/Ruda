
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

// Abstracts the Window Context struct from X11 to XC to avoid ambiguity
#define XC XContext

#define RUDA_TRUE 1

#define RUDA_FALSE 0

#define DYNAMIC_STORAGE_BIT            0x0100
#define MAP_READ_BIT                   0x0001
#define MAP_WRITE_BIT                  0x0002


// ------------------------------- BUFFER_OBJECT.CPP ------------------------------- //

void rudaGenBuffer(uint *buffers);

void rudaCreateBuffer(struct Ruda_Context *ctx, uint *buffers);
