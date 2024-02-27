#pragma once


// Abstracts the Window struct from X11 to XWindow to avoid ambiguity
#define XWindow Window

// Abstracts the Visual struct from X11 to XVisual to avoid ambiguity
#define XVisual Visual

// Abstracts the Display struct from X11 to XDisplay to avoid ambiguity
#define XDisplay Display

// Abstracts the Display struct from X11 to XDisplay to avoid ambiguity
#define XContext GC