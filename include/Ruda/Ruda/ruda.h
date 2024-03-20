
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


#ifndef GL_EXT_buffer_storage
#define GL_EXT_buffer_storage 1
#define GL_MAP_READ_BIT                   0x0001
#define GL_MAP_WRITE_BIT                  0x0002
#define GL_MAP_PERSISTENT_BIT_EXT         0x0040
#define GL_MAP_COHERENT_BIT_EXT           0x0080
#define GL_DYNAMIC_STORAGE_BIT_EXT        0x0100
#define GL_CLIENT_STORAGE_BIT_EXT         0x0200
#define GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT_EXT 0x00004000
#define GL_BUFFER_IMMUTABLE_STORAGE_EXT   0x821F
#define GL_BUFFER_STORAGE_FLAGS_EXT       0x8220
#endif /* GL_EXT_buffer_storage */

#ifndef GL_VERSION_4_4
#define GL_VERSION_4_4 1
#define GL_MAX_VERTEX_ATTRIB_STRIDE       0x82E5
#define GL_PRIMITIVE_RESTART_FOR_PATCHES_SUPPORTED 0x8221
#define GL_TEXTURE_BUFFER_BINDING         0x8C2A
#define GL_MAP_PERSISTENT_BIT             0x0040
#define GL_MAP_COHERENT_BIT               0x0080
#define GL_DYNAMIC_STORAGE_BIT            0x0100
#define GL_CLIENT_STORAGE_BIT             0x0200
#define GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT 0x00004000
#define GL_BUFFER_IMMUTABLE_STORAGE       0x821F
#define GL_BUFFER_STORAGE_FLAGS           0x8220
#define GL_CLEAR_TEXTURE                  0x9365
#define GL_LOCATION_COMPONENT             0x934A
#define GL_TRANSFORM_FEEDBACK_BUFFER_INDEX 0x934B
#define GL_TRANSFORM_FEEDBACK_BUFFER_STRIDE 0x934C
#define GL_QUERY_BUFFER                   0x9192
#define GL_QUERY_BUFFER_BARRIER_BIT       0x00008000
#define GL_QUERY_BUFFER_BINDING           0x9193
#define GL_QUERY_RESULT_NO_WAIT           0x9194
#define GL_MIRROR_CLAMP_TO_EDGE           0x8743
#endif

#define GL_STREAM_READ                    0x88E1
#define GL_STREAM_COPY                    0x88E2
#define GL_STATIC_READ                    0x88E5
#define GL_STATIC_COPY                    0x88E6
#define GL_DYNAMIC_READ                   0x88E9
#define GL_DYNAMIC_COPY                   0x88EA
#define GL_STREAM_DRAW                    0x88E0
#define GL_STATIC_DRAW                    0x88E4
#define GL_DYNAMIC_DRAW                   0x88E8

#ifndef GL_ARB_sparse_buffer
#define GL_ARB_sparse_buffer 1
#define GL_SPARSE_STORAGE_BIT_ARB         0x0400
#define GL_SPARSE_BUFFER_PAGE_SIZE_ARB    0x82F8
#endif


/**
 * Max number of texture image units.  Also determines number of texture
 * samplers in shaders.
 */
#define MAX_TEXTURE_IMAGE_UNITS 32
#define MAX_COMBINED_TEXTURE_IMAGE_UNITS (MAX_TEXTURE_IMAGE_UNITS * 6)

/**
 * Max number of texture image units.  Also determines number of texture
 * samplers in shaders.
 */
#define MAX_TEXTURE_IMAGE_UNITS 32
#define MAX_SAMPLERS                   MAX_TEXTURE_IMAGE_UNITS


// ------------------------------- BUFFER_OBJECT.CPP ------------------------------- //

void rudaGenBuffer(uint *buffers);

void rudaCreateBuffer(struct Ruda_Context *ctx, uint *buffers);

void rudaBindVBO(uint* buffer);

void rudaBufferArrayData(int size, const void *data);
