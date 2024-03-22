
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
#define _NEW_MODELVIEW         (1u << 0)   /**< RUDA_context::ModelView */
#define _NEW_PROJECTION        (1u << 1)   /**< RUDA_context::Projection */
#define _NEW_TEXTURE_MATRIX    (1u << 2)   /**< RUDA_context::TextureMatrix */
#define _NEW_COLOR             (1u << 3)   /**< RUDA_context::Color */
#define _NEW_DEPTH             (1u << 4)   /**< RUDA_context::Depth */
#define _NEW_TNL_SPACES        (1u << 5)  /**< _mesa_update_tnl_spaces */
#define _NEW_FOG               (1u << 6)   /**< RUDA_context::Fog */
#define _NEW_HINT              (1u << 7)   /**< RUDA_context::Hint */
#define _NEW_LIGHT_CONSTANTS   (1u << 8)   /**< RUDA_context::Light */
#define _NEW_LINE              (1u << 9)   /**< RUDA_context::Line */
#define _NEW_PIXEL             (1u << 10)  /**< RUDA_context::Pixel */
#define _NEW_POINT             (1u << 11)  /**< RUDA_context::Point */
#define _NEW_POLYGON           (1u << 12)  /**< RUDA_context::Polygon */
#define _NEW_POLYGONSTIPPLE    (1u << 13)  /**< RUDA_context::PolygonStipple */
#define _NEW_SCISSOR           (1u << 14)  /**< RUDA_context::Scissor */
#define _NEW_STENCIL           (1u << 15)  /**< RUDA_context::Stencil */
#define _NEW_TEXTURE_OBJECT    (1u << 16)  /**< RUDA_context::Texture (bindings only) */
#define _NEW_TRANSFORM         (1u << 17)  /**< RUDA_context::Transform */
#define _NEW_VIEWPORT          (1u << 18)  /**< RUDA_context::Viewport */
#define _NEW_TEXTURE_STATE     (1u << 19)  /**< RUDA_context::Texture (states only) */
#define _NEW_LIGHT_STATE       (1u << 20)  /**< RUDA_context::Light */
#define _NEW_RENDERMODE        (1u << 21)  /**< RUDA_context::RenderMode, etc */
#define _NEW_BUFFERS           (1u << 22)  /**< RUDA_context::Visual, DrawBuffer, */
#define _NEW_CURRENT_ATTRIB    (1u << 23)  /**< RUDA_context::Current */
#define _NEW_MULTISAMPLE       (1u << 24)  /**< RUDA_context::Multisample */
#define _NEW_TRACK_MATRIX      (1u << 25)  /**< RUDA_context::VertexProgram */
#define _NEW_PROGRAM           (1u << 26)  /**< New program/shader state */
#define _NEW_PROGRAM_CONSTANTS (1u << 27)
#define _NEW_FF_VERT_PROGRAM   (1u << 28)
#define _NEW_FRAG_CLAMP        (1u << 29)
#define _NEW_MATERIAL          (1u << 30)  /**< RUDA_context::Light.Material */
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

#ifndef RUDA_NV_fragment_program
#define RUDA_NV_fragment_program 1
#define RUDA_MAX_FRAGMENT_PROGRAM_LOCAL_PARAMETERS_NV 0x8868
#define RUDA_FRAGMENT_PROGRAM_NV            0x8870
#define RUDA_MAX_TEXTURE_COORDS_NV          0x8871
#define RUDA_MAX_TEXTURE_IMAGE_UNITS_NV     0x8872
#define RUDA_FRAGMENT_PROGRAM_BINDING_NV    0x8873
#define RUDA_PROGRAM_ERROR_STRING_NV        0x8874
#endif

#ifndef RUDA_NV_geometry_program4
#define RUDA_NV_geometry_program4 1
#define RUDA_GEOMETRY_PROGRAM_NV            0x8C26
#define RUDA_MAX_PROGRAM_OUTPUT_VERTICES_NV 0x8C27
#define RUDA_MAX_PROGRAM_TOTAL_OUTPUT_COMPONENTS_NV 0x8C28
#endif

#define RUDA_TRANSFORM_FEEDBACK_VARYING     0x92F4
#define RUDA_UNIFORM                        0x92E1
#define GET_PROGRAM_RESOURCE_TYPE_FROM_GLENUM(x) ((x) - RUDA_UNIFORM)
#define NUM_PROGRAM_RESOURCE_TYPES (RUDA_TRANSFORM_FEEDBACK_VARYING - RUDA_UNIFORM + 1)

#ifndef RUDA_ARB_fragment_program
#define RUDA_ARB_fragment_program 1
#define RUDA_FRAGMENT_PROGRAM_ARB           0x8804
#define RUDA_PROGRAM_FORMAT_ASCII_ARB       0x8875
#define RUDA_PROGRAM_LENGTH_ARB             0x8627
#define RUDA_PROGRAM_FORMAT_ARB             0x8876
#define RUDA_PROGRAM_BINDING_ARB            0x8677
#define RUDA_PROGRAM_INSTRUCTIONS_ARB       0x88A0
#define RUDA_MAX_PROGRAM_INSTRUCTIONS_ARB   0x88A1
#define RUDA_PROGRAM_NATIVE_INSTRUCTIONS_ARB 0x88A2
#define RUDA_MAX_PROGRAM_NATIVE_INSTRUCTIONS_ARB 0x88A3
#define RUDA_PROGRAM_TEMPORARIES_ARB        0x88A4
#define RUDA_MAX_PROGRAM_TEMPORARIES_ARB    0x88A5
#define RUDA_PROGRAM_NATIVE_TEMPORARIES_ARB 0x88A6
#define RUDA_MAX_PROGRAM_NATIVE_TEMPORARIES_ARB 0x88A7
#define RUDA_PROGRAM_PARAMETERS_ARB         0x88A8
#define RUDA_MAX_PROGRAM_PARAMETERS_ARB     0x88A9
#define RUDA_PROGRAM_NATIVE_PARAMETERS_ARB  0x88AA
#define RUDA_MAX_PROGRAM_NATIVE_PARAMETERS_ARB 0x88AB
#define RUDA_PROGRAM_ATTRIBS_ARB            0x88AC
#define RUDA_MAX_PROGRAM_ATTRIBS_ARB        0x88AD
#define RUDA_PROGRAM_NATIVE_ATTRIBS_ARB     0x88AE
#define RUDA_MAX_PROGRAM_NATIVE_ATTRIBS_ARB 0x88AF
#define RUDA_MAX_PROGRAM_LOCAL_PARAMETERS_ARB 0x88B4
#define RUDA_MAX_PROGRAM_ENV_PARAMETERS_ARB 0x88B5
#define RUDA_PROGRAM_UNDER_NATIVE_LIMITS_ARB 0x88B6
#define RUDA_PROGRAM_ALU_INSTRUCTIONS_ARB   0x8805
#define RUDA_PROGRAM_TEX_INSTRUCTIONS_ARB   0x8806
#define RUDA_PROGRAM_TEX_INDIRECTIONS_ARB   0x8807
#define RUDA_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB 0x8808
#define RUDA_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB 0x8809
#define RUDA_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB 0x880A
#define RUDA_MAX_PROGRAM_ALU_INSTRUCTIONS_ARB 0x880B
#define RUDA_MAX_PROGRAM_TEX_INSTRUCTIONS_ARB 0x880C
#define RUDA_MAX_PROGRAM_TEX_INDIRECTIONS_ARB 0x880D
#define RUDA_MAX_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB 0x880E
#define RUDA_MAX_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB 0x880F
#define RUDA_MAX_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB 0x8810
#define RUDA_PROGRAM_STRING_ARB             0x8628
#define RUDA_PROGRAM_ERROR_POSITION_ARB     0x864B
#define RUDA_CURRENT_MATRIX_ARB             0x8641
#define RUDA_TRANSPOSE_CURRENT_MATRIX_ARB   0x88B7
#define RUDA_CURRENT_MATRIX_STACK_DEPTH_ARB 0x8640
#define RUDA_MAX_PROGRAM_MATRICES_ARB       0x862F
#define RUDA_MAX_PROGRAM_MATRIX_STACK_DEPTH_ARB 0x862E
#define RUDA_MAX_TEXTURE_COORDS_ARB         0x8871
#define RUDA_MAX_TEXTURE_IMAGE_UNITS_ARB    0x8872
#define RUDA_PROGRAM_ERROR_STRING_ARB       0x8874
#define RUDA_MATRIX0_ARB                    0x88C0
#define RUDA_MATRIX1_ARB                    0x88C1
#define RUDA_MATRIX2_ARB                    0x88C2
#define RUDA_MATRIX3_ARB                    0x88C3
#define RUDA_MATRIX4_ARB                    0x88C4
#define RUDA_MATRIX5_ARB                    0x88C5
#define RUDA_MATRIX6_ARB                    0x88C6
#define RUDA_MATRIX7_ARB                    0x88C7
#define RUDA_MATRIX8_ARB                    0x88C8
#define RUDA_MATRIX9_ARB                    0x88C9
#define RUDA_MATRIX10_ARB                   0x88CA
#define RUDA_MATRIX11_ARB                   0x88CB
#define RUDA_MATRIX12_ARB                   0x88CC
#define RUDA_MATRIX13_ARB                   0x88CD
#define RUDA_MATRIX14_ARB                   0x88CE
#define RUDA_MATRIX15_ARB                   0x88CF
#define RUDA_MATRIX16_ARB                   0x88D0
#define RUDA_MATRIX17_ARB                   0x88D1
#define RUDA_MATRIX18_ARB                   0x88D2
#define RUDA_MATRIX19_ARB                   0x88D3
#define RUDA_MATRIX20_ARB                   0x88D4
#define RUDA_MATRIX21_ARB                   0x88D5
#define RUDA_MATRIX22_ARB                   0x88D6
#define RUDA_MATRIX23_ARB                   0x88D7
#define RUDA_MATRIX24_ARB                   0x88D8
#define RUDA_MATRIX25_ARB                   0x88D9
#define RUDA_MATRIX26_ARB                   0x88DA
#define RUDA_MATRIX27_ARB                   0x88DB
#define RUDA_MATRIX28_ARB                   0x88DC
#define RUDA_MATRIX29_ARB                   0x88DD
#define RUDA_MATRIX30_ARB                   0x88DE
#define RUDA_MATRIX31_ARB                   0x88DF
#endif

#ifndef RUDA_ARB_vertex_program
#define RUDA_ARB_vertex_program 1
#define RUDA_COLOR_SUM_ARB                  0x8458
#define RUDA_VERTEX_PROGRAM_ARB             0x8620
#define RUDA_VERTEX_ATTRIB_ARRAY_ENABLED_ARB 0x8622
#define RUDA_VERTEX_ATTRIB_ARRAY_SIZE_ARB   0x8623
#define RUDA_VERTEX_ATTRIB_ARRAY_STRIDE_ARB 0x8624
#define RUDA_VERTEX_ATTRIB_ARRAY_TYPE_ARB   0x8625
#define RUDA_CURRENT_VERTEX_ATTRIB_ARB      0x8626
#define RUDA_VERTEX_PROGRAM_POINT_SIZE_ARB  0x8642
#define RUDA_VERTEX_PROGRAM_TWO_SIDE_ARB    0x8643
#define RUDA_VERTEX_ATTRIB_ARRAY_POINTER_ARB 0x8645
#define RUDA_MAX_VERTEX_ATTRIBS_ARB         0x8869
#define RUDA_VERTEX_ATTRIB_ARRAY_NORMALIZED_ARB 0x886A
#define RUDA_PROGRAM_ADDRESS_REGISTERS_ARB  0x88B0
#define RUDA_MAX_PROGRAM_ADDRESS_REGISTERS_ARB 0x88B1
#define RUDA_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB 0x88B2
#define RUDA_MAX_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB 0x88B3
#endif
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

void _mesa_update_state( struct Ruda_Context *ctx );

void _mesa_update_framebuffer(struct Ruda_Context *ctx, struct Ruda_Framebuffer *readFb, struct Ruda_Framebuffer *drawFb);

void _mesa_reference_shader_program_data(struct Ruda_Shader_Program_Data **ptr, struct Ruda_Shader_Program_Data *data);