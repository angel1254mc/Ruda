
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


#ifndef RUDA_EXT_buffer_storage
#define RUDA_EXT_buffer_storage 1
#define RUDA_MAP_READ_BIT                   0x0001
#define RUDA_MAP_WRITE_BIT                  0x0002
#define RUDA_MAP_PERSISTENT_BIT_EXT         0x0040
#define RUDA_MAP_COHERENT_BIT_EXT           0x0080
#define RUDA_DYNAMIC_STORAGE_BIT_EXT        0x0100
#define RUDA_CLIENT_STORAGE_BIT_EXT         0x0200
#define RUDA_CLIENT_MAPPED_BUFFER_BARRIER_BIT_EXT 0x00004000
#define RUDA_BUFFER_IMMUTABLE_STORAGE_EXT   0x821F
#define RUDA_BUFFER_STORAGE_FLAGS_EXT       0x8220
#endif /* RUDA_EXT_buffer_storage */

#ifndef RUDA_VERSION_4_4
#define RUDA_VERSION_4_4 1
#define RUDA_MAX_VERTEX_ATTRIB_STRIDE       0x82E5
#define RUDA_PRIMITIVE_RESTART_FOR_PATCHES_SUPPORTED 0x8221
#define RUDA_TEXTURE_BUFFER_BINDING         0x8C2A
#define RUDA_MAP_PERSISTENT_BIT             0x0040
#define RUDA_MAP_COHERENT_BIT               0x0080
#define RUDA_DYNAMIC_STORAGE_BIT            0x0100
#define RUDA_CLIENT_STORAGE_BIT             0x0200
#define RUDA_CLIENT_MAPPED_BUFFER_BARRIER_BIT 0x00004000
#define RUDA_BUFFER_IMMUTABLE_STORAGE       0x821F
#define RUDA_BUFFER_STORAGE_FLAGS           0x8220
#define RUDA_CLEAR_TEXTURE                  0x9365
#define RUDA_LOCATION_COMPONENT             0x934A
#define RUDA_TRANSFORM_FEEDBACK_BUFFER_INDEX 0x934B
#define RUDA_TRANSFORM_FEEDBACK_BUFFER_STRIDE 0x934C
#define RUDA_QUERY_BUFFER                   0x9192
#define RUDA_QUERY_BUFFER_BARRIER_BIT       0x00008000
#define RUDA_QUERY_BUFFER_BINDING           0x9193
#define RUDA_QUERY_RESULT_NO_WAIT           0x9194
#define RUDA_MIRROR_CLAMP_TO_EDGE           0x8743
#endif

/* Primitives */
#define RUDA_POINTS				0x0000
#define RUDA_LINES				0x0001
#define RUDA_LINE_LOOP				0x0002
#define RUDA_LINE_STRIP				0x0003
#define RUDA_TRIS   				0x0004
#define RUDA_TRI_STRIP  			0x0005
#define RUDA_TRI_FAN   				0x0006
#define RUDA_QUADS				0x0007
#define RUDA_QUAD_STRIP				0x0008
#define RUDA_POLYGON				0x0009
#define RUDA_LINES_ADJACENCY                0x000A
#define RUDA_LINE_STRIP_ADJACENCY           0x000B
#define RUDA_TRIANGLES_ADJACENCY            0x000C
#define RUDA_TRIANGLE_STRIP_ADJACENCY       0x000D




#define RUDA_STREAM_READ                    0x88E1
#define RUDA_STREAM_COPY                    0x88E2
#define RUDA_STATIC_READ                    0x88E5
#define RUDA_STATIC_COPY                    0x88E6
#define RUDA_DYNAMIC_READ                   0x88E9
#define RUDA_DYNAMIC_COPY                   0x88EA
#define RUDA_STREAM_DRAW                    0x88E0
#define RUDA_STATIC_DRAW                    0x88E4
#define RUDA_DYNAMIC_DRAW                   0x88E8

#ifndef RUDA_ARB_sparse_buffer
#define RUDA_ARB_sparse_buffer 1
#define RUDA_SPARSE_STORAGE_BIT_ARB         0x0400
#define RUDA_SPARSE_BUFFER_PAGE_SIZE_ARB    0x82F8
#endif
/* Polygons */
#define RUDA_POINT				0x1B00
#define RUDA_LINE					0x1B01
#define RUDA_FILL					0x1B02
#define RUDA_CW					0x0900
#define RUDA_CCW					0x0901
#define RUDA_FRONT				0x0404
#define RUDA_BACK					0x0405
#define RUDA_POLYGON_MODE				0x0B40
#define RUDA_POLYGON_SMOOTH			0x0B41
#define RUDA_POLYGON_STIPPLE			0x0B42
#define RUDA_EDGE_FLAG				0x0B43
#define RUDA_CULL_FACE				0x0B44
#define RUDA_CULL_FACE_MODE			0x0B45
#define RUDA_FRONT_FACE				0x0B46
#define RUDA_POLYGON_OFFSET_FACTOR		0x8038
#define RUDA_POLYGON_OFFSET_UNITS			0x2A00
#define RUDA_POLYGON_OFFSET_POINT			0x2A01
#define RUDA_POLYGON_OFFSET_LINE			0x2A02
#define RUDA_POLYGON_OFFSET_FILL			0x8037


#define RUDA_BYTE                           0x1400
#define RUDA_UNSIGNED_BYTE                  0x1401
#define RUDA_SHORT                          0x1402
#define RUDA_UNSIGNED_SHORT                 0x1403
#define RUDA_INT                            0x1404
#define RUDA_UNSIGNED_INT                   0x1405
#define RUDA_FLOAT                          0x1406
#define RUDA_2_BYTES				        0x1407
#define RUDA_3_BYTES				        0x1408
#define RUDA_4_BYTES				        0x1409
#define RUDA_DOUBLE				            0x140A
#define RUDA_FIXED                          0x140C
#define RUDA_UNSIGNED_INT64_ARB             0x140F


#ifndef RUDA_OES_texture_half_float
#define RUDA_OES_texture_half_float 1
#define RUDA_HALF_FLOAT_OES                 0x8D61
#endif /* RUDA_OES_texture_half_float */

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

/** Used to do error checking for RUDA_EXT_vertex_array_bgra */
#define BGRA_OR_4  5

#define RUDA_RGBA                           0x1908
#define RUDA_BGRA                           0x80E1


#define RUDA_UNSIGNED_INT_10F_11F_11F_REV   0x8C3B
#define RUDA_UNSIGNED_INT_2_10_10_10_REV    0x8368
#define RUDA_INT_2_10_10_10_REV             0x8D9F

#define RUDA_COLOR_BUFFER_BIT               0x00004000

/** Extra draw modes beyond RUDA_POINTS, RUDA_TRIANGLE_FAN, etc */
#define RUDA_PATCHES                        0x000E
#define PRIM_MAX                            RUDA_PATCHES
#define PRIM_OUTSIDE_BEGIN_END              (PRIM_MAX + 1)
#define PRIM_UNKNOWN                        (PRIM_MAX + 2)

/**
 * Max number of primitives (number of glBegin/End pairs) per VBO.
 */
#define VBO_MAX_PRIM 64
#define VBO_MAX_COPIED_VERTS 31


/* Render Mode */
#define RUDA_FEEDBACK			0x1C01
#define RUDA_RENDER				0x1C00
#define RUDA_SELECT				0x1C02

#define VBO_ATTRIB_LAST_MATERIAL VBO_ATTRIB_MAT_BACK_INDEXES
/**
 * Move material attribs to the last generic attribs, moving LAST_MATERIAL
 * to GENERIC15, etc.
*/
#define VBO_MATERIAL_SHIFT (VBO_ATTRIB_LAST_MATERIAL - VBO_ATTRIB_GENERIC15)

/*
 * For RUDA_ARB_vertex_buffer_object we need to treat vertex array pointers
 * as offsets into buffer stores.  Since the vertex array pointer and
 * buffer store pointer are both pointers and we need to add them, we use
 * this macro.
 * Both pointers/offsets are expressed in bytes.
 */
#define ADD_POINTERS(A, B)  ( (GLubyte *) (A) + (uintptr_t) (B) )


/**
 * \name Bits to indicate what state has changed.
 */
/*@{*/
#define _NEW_MODELVIEW         (1u << 0)   /**< gl_context::ModelView */
#define _NEW_PROJECTION        (1u << 1)   /**< gl_context::Projection */
#define _NEW_TEXTURE_MATRIX    (1u << 2)   /**< gl_context::TextureMatrix */
#define _NEW_COLOR             (1u << 3)   /**< gl_context::Color */
#define _NEW_DEPTH             (1u << 4)   /**< gl_context::Depth */
#define _NEW_TNL_SPACES        (1u << 5)  /**< _mesa_update_tnl_spaces */
#define _NEW_FOG               (1u << 6)   /**< gl_context::Fog */
#define _NEW_HINT              (1u << 7)   /**< gl_context::Hint */
#define _NEW_LIGHT_CONSTANTS   (1u << 8)   /**< gl_context::Light */
#define _NEW_LINE              (1u << 9)   /**< gl_context::Line */
#define _NEW_PIXEL             (1u << 10)  /**< gl_context::Pixel */
#define _NEW_POINT             (1u << 11)  /**< gl_context::Point */
#define _NEW_POLYGON           (1u << 12)  /**< gl_context::Polygon */
#define _NEW_POLYGONSTIPPLE    (1u << 13)  /**< gl_context::PolygonStipple */
#define _NEW_SCISSOR           (1u << 14)  /**< gl_context::Scissor */
#define _NEW_STENCIL           (1u << 15)  /**< gl_context::Stencil */
#define _NEW_TEXTURE_OBJECT    (1u << 16)  /**< gl_context::Texture (bindings only) */
#define _NEW_TRANSFORM         (1u << 17)  /**< gl_context::Transform */
#define _NEW_VIEWPORT          (1u << 18)  /**< gl_context::Viewport */
#define _NEW_TEXTURE_STATE     (1u << 19)  /**< gl_context::Texture (states only) */
#define _NEW_LIGHT_STATE       (1u << 20)  /**< gl_context::Light */
#define _NEW_RENDERMODE        (1u << 21)  /**< gl_context::RenderMode, etc */
#define _NEW_BUFFERS           (1u << 22)  /**< gl_context::Visual, DrawBuffer, */
#define _NEW_CURRENT_ATTRIB    (1u << 23)  /**< gl_context::Current */
#define _NEW_MULTISAMPLE       (1u << 24)  /**< gl_context::Multisample */
#define _NEW_TRACK_MATRIX      (1u << 25)  /**< gl_context::VertexProgram */
#define _NEW_PROGRAM           (1u << 26)  /**< New program/shader state */
#define _NEW_PROGRAM_CONSTANTS (1u << 27)
#define _NEW_FF_VERT_PROGRAM   (1u << 28)
#define _NEW_FRAG_CLAMP        (1u << 29)
#define _NEW_MATERIAL          (1u << 30)  /**< gl_context::Light.Material */
#define _NEW_FF_FRAG_PROGRAM   (1u << 31)
#define _NEW_ALL ~0
/*@}*/


/* glPush/PopAttrib bits */
#define RUDA_CURRENT_BIT				0x00000001
#define RUDA_POINT_BIT				0x00000002
#define RUDA_LINE_BIT				0x00000004
#define RUDA_POLYGON_BIT				0x00000008
#define RUDA_POLYGON_STIPPLE_BIT			0x00000010
#define RUDA_PIXEL_MODE_BIT			0x00000020
#define RUDA_LIGHTING_BIT				0x00000040
#define RUDA_FOG_BIT				0x00000080
#define RUDA_DEPTH_BUFFER_BIT			0x00000100
#define RUDA_ACCUM_BUFFER_BIT			0x00000200
#define RUDA_STENCIL_BUFFER_BIT			0x00000400
#define RUDA_VIEWPORT_BIT				0x00000800
#define RUDA_TRANSFORM_BIT			0x00001000
#define RUDA_ENABLE_BIT				0x00002000
#define RUDA_COLOR_BUFFER_BIT			0x00004000
#define RUDA_HINT_BIT				0x00008000
#define RUDA_EVAL_BIT				0x00010000
#define RUDA_LIST_BIT				0x00020000
#define RUDA_TEXTURE_BIT				0x00040000
#define RUDA_SCISSOR_BIT				0x00080000
#define RUDA_ALL_ATTRIB_BITS			0xFFFFFFFF

// ------------------------------- BUFFER_OBJECT.CPP ------------------------------- //

void rudaGenBuffer(uint *buffers);

void rudaCreateBuffer(struct Ruda_Context *ctx, uint *buffers);

void rudaBindVBO(uint* buffer);

void rudaBufferArrayData(int size, const void *data);

void rudaEnableVertexAttribArray(unsigned int index);

void rudaVertexAttribPointer(uint index, int size, int type, int stride, const void* ptr);

void rudaDrawArrays(uint mode, int start, int size);
void rudaClear(uint mask);

static inline bool _mesa_inside_begin_end(Ruda_Context* ctx);

static inline bool _mesa_hw_select_enabled(const struct Ruda_Context *ctx);

static inline void update_attribute_map_mode(const struct Ruda_Context *ctx, struct Ruda_Vertex_Array_Object *vao);

