#pragma once
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <threads.h>
#include "../src/util/u_idalloc.h"
#include "../src/compiler/shader_enums.h"
//#include "util/glheader.h"
// Abstracts the Graphics Context struct from X11 to XGC to avoid ambiguity
#define XGC GC
#define XC XContext
#define MAX_TEXTURE_IMAGE_UNITS 32
#define MAX_COMBINED_TEXTURE_IMAGE_UNITS (MAX_TEXTURE_IMAGE_UNITS * 6)
#define MAX_IMAGE_UNIFORMS             32

#define MAX_PROGRAM_ENV_PARAMS         256
#define MAX_TEXTURE_IMAGE_UNITS 32
#define MAX_SAMPLERS                   MAX_TEXTURE_IMAGE_UNITS
#define VBO_MAX_PRIM 64
#define VBO_MAX_COPIED_VERTS 31

typedef enum
{
   TEXTURE_2D_MULTISAMPLE_INDEX,
   TEXTURE_2D_MULTISAMPLE_ARRAY_INDEX,
   TEXTURE_CUBE_ARRAY_INDEX,
   TEXTURE_BUFFER_INDEX,
   TEXTURE_2D_ARRAY_INDEX,
   TEXTURE_1D_ARRAY_INDEX,
   TEXTURE_EXTERNAL_INDEX,
   TEXTURE_CUBE_INDEX,
   TEXTURE_3D_INDEX,
   TEXTURE_RECT_INDEX,
   TEXTURE_2D_INDEX,
   TEXTURE_1D_INDEX,
   NUM_TEXTURE_TARGETS
} Ruda_Texture_Index;

/**
 * Enum for defining the mapping for the position/generic0 attribute.
 *
 * Do not change the order of the values as these are used as
 * array indices.
 */
typedef enum
{
   ATTRIBUTE_MAP_MODE_IDENTITY, /**< 1:1 mapping */
   ATTRIBUTE_MAP_MODE_POSITION, /**< get position and generic0 from position */
   ATTRIBUTE_MAP_MODE_GENERIC0, /**< get position and generic0 from generic0 */
   ATTRIBUTE_MAP_MODE_MAX       /**< for sizing arrays */
} Ruda_Attribute_Map_Mode;



/**
 * Enum for the OpenGL APIs we know about and may support.
 *
 * NOTE: This must match the api_enum table in
 * src/mesa/main/get_hash_generator.py
 */
typedef enum
{
   API_OPENGL_COMPAT,      /* legacy / compatibility contexts */
   API_OPENGLES,
   API_OPENGLES2,
   API_OPENGL_CORE,
   API_OPENGL_LAST = API_OPENGL_CORE
} Ruda_API;



typedef enum
{
   MAP_USER,
   MAP_INTERNAL,
   MAP_GLTHREAD,
   MAP_COUNT
} Ruda_Map_Buffer_Index;

struct gl_extensions
{
   bool EXT_vertex_array_bgra;
   bool ARB_buffer_storage;
};

/* vertex_formats[(gltype & 0x3f) | (double << 5)][integer*2 + normalized][size - 1] */
static const uint8_t vertex_formats[][4][4] = {
   { /* GL_BYTE */
      {
         PIPE_FORMAT_R8_SSCALED,
         PIPE_FORMAT_R8G8_SSCALED,
         PIPE_FORMAT_R8G8B8_SSCALED,
         PIPE_FORMAT_R8G8B8A8_SSCALED
      },
      {
         PIPE_FORMAT_R8_SNORM,
         PIPE_FORMAT_R8G8_SNORM,
         PIPE_FORMAT_R8G8B8_SNORM,
         PIPE_FORMAT_R8G8B8A8_SNORM
      },
      {
         PIPE_FORMAT_R8_SINT,
         PIPE_FORMAT_R8G8_SINT,
         PIPE_FORMAT_R8G8B8_SINT,
         PIPE_FORMAT_R8G8B8A8_SINT
      },
   },
   { /* GL_UNSIGNED_BYTE */
      {
         PIPE_FORMAT_R8_USCALED,
         PIPE_FORMAT_R8G8_USCALED,
         PIPE_FORMAT_R8G8B8_USCALED,
         PIPE_FORMAT_R8G8B8A8_USCALED
      },
      {
         PIPE_FORMAT_R8_UNORM,
         PIPE_FORMAT_R8G8_UNORM,
         PIPE_FORMAT_R8G8B8_UNORM,
         PIPE_FORMAT_R8G8B8A8_UNORM
      },
      {
         PIPE_FORMAT_R8_UINT,
         PIPE_FORMAT_R8G8_UINT,
         PIPE_FORMAT_R8G8B8_UINT,
         PIPE_FORMAT_R8G8B8A8_UINT
      },
   },
   { /* GL_SHORT */
      {
         PIPE_FORMAT_R16_SSCALED,
         PIPE_FORMAT_R16G16_SSCALED,
         PIPE_FORMAT_R16G16B16_SSCALED,
         PIPE_FORMAT_R16G16B16A16_SSCALED
      },
      {
         PIPE_FORMAT_R16_SNORM,
         PIPE_FORMAT_R16G16_SNORM,
         PIPE_FORMAT_R16G16B16_SNORM,
         PIPE_FORMAT_R16G16B16A16_SNORM
      },
      {
         PIPE_FORMAT_R16_SINT,
         PIPE_FORMAT_R16G16_SINT,
         PIPE_FORMAT_R16G16B16_SINT,
         PIPE_FORMAT_R16G16B16A16_SINT
      },
   },
   { /* GL_UNSIGNED_SHORT */
      {
         PIPE_FORMAT_R16_USCALED,
         PIPE_FORMAT_R16G16_USCALED,
         PIPE_FORMAT_R16G16B16_USCALED,
         PIPE_FORMAT_R16G16B16A16_USCALED
      },
      {
         PIPE_FORMAT_R16_UNORM,
         PIPE_FORMAT_R16G16_UNORM,
         PIPE_FORMAT_R16G16B16_UNORM,
         PIPE_FORMAT_R16G16B16A16_UNORM
      },
      {
         PIPE_FORMAT_R16_UINT,
         PIPE_FORMAT_R16G16_UINT,
         PIPE_FORMAT_R16G16B16_UINT,
         PIPE_FORMAT_R16G16B16A16_UINT
      },
   },
   { /* GL_INT */
      {
         PIPE_FORMAT_R32_SSCALED,
         PIPE_FORMAT_R32G32_SSCALED,
         PIPE_FORMAT_R32G32B32_SSCALED,
         PIPE_FORMAT_R32G32B32A32_SSCALED
      },
      {
         PIPE_FORMAT_R32_SNORM,
         PIPE_FORMAT_R32G32_SNORM,
         PIPE_FORMAT_R32G32B32_SNORM,
         PIPE_FORMAT_R32G32B32A32_SNORM
      },
      {
         PIPE_FORMAT_R32_SINT,
         PIPE_FORMAT_R32G32_SINT,
         PIPE_FORMAT_R32G32B32_SINT,
         PIPE_FORMAT_R32G32B32A32_SINT
      },
   },
   { /* GL_UNSIGNED_INT */
      {
         PIPE_FORMAT_R32_USCALED,
         PIPE_FORMAT_R32G32_USCALED,
         PIPE_FORMAT_R32G32B32_USCALED,
         PIPE_FORMAT_R32G32B32A32_USCALED
      },
      {
         PIPE_FORMAT_R32_UNORM,
         PIPE_FORMAT_R32G32_UNORM,
         PIPE_FORMAT_R32G32B32_UNORM,
         PIPE_FORMAT_R32G32B32A32_UNORM
      },
      {
         PIPE_FORMAT_R32_UINT,
         PIPE_FORMAT_R32G32_UINT,
         PIPE_FORMAT_R32G32B32_UINT,
         PIPE_FORMAT_R32G32B32A32_UINT
      },
   },
   { /* GL_FLOAT */
      {
         PIPE_FORMAT_R32_FLOAT,
         PIPE_FORMAT_R32G32_FLOAT,
         PIPE_FORMAT_R32G32B32_FLOAT,
         PIPE_FORMAT_R32G32B32A32_FLOAT
      },
      {
         PIPE_FORMAT_R32_FLOAT,
         PIPE_FORMAT_R32G32_FLOAT,
         PIPE_FORMAT_R32G32B32_FLOAT,
         PIPE_FORMAT_R32G32B32A32_FLOAT
      },
   },
   {{0}}, /* GL_2_BYTES */
   {{0}}, /* GL_3_BYTES */
   {{0}}, /* GL_4_BYTES */
   { /* GL_DOUBLE */
      {
         PIPE_FORMAT_R64_FLOAT,
         PIPE_FORMAT_R64G64_FLOAT,
         PIPE_FORMAT_R64G64B64_FLOAT,
         PIPE_FORMAT_R64G64B64A64_FLOAT
      },
      {
         PIPE_FORMAT_R64_FLOAT,
         PIPE_FORMAT_R64G64_FLOAT,
         PIPE_FORMAT_R64G64B64_FLOAT,
         PIPE_FORMAT_R64G64B64A64_FLOAT
      },
   },
   { /* GL_HALF_FLOAT */
      {
         PIPE_FORMAT_R16_FLOAT,
         PIPE_FORMAT_R16G16_FLOAT,
         PIPE_FORMAT_R16G16B16_FLOAT,
         PIPE_FORMAT_R16G16B16A16_FLOAT
      },
      {
         PIPE_FORMAT_R16_FLOAT,
         PIPE_FORMAT_R16G16_FLOAT,
         PIPE_FORMAT_R16G16B16_FLOAT,
         PIPE_FORMAT_R16G16B16A16_FLOAT
      },
   },
   { /* GL_FIXED */
      {
         PIPE_FORMAT_R32_FIXED,
         PIPE_FORMAT_R32G32_FIXED,
         PIPE_FORMAT_R32G32B32_FIXED,
         PIPE_FORMAT_R32G32B32A32_FIXED
      },
      {
         PIPE_FORMAT_R32_FIXED,
         PIPE_FORMAT_R32G32_FIXED,
         PIPE_FORMAT_R32G32B32_FIXED,
         PIPE_FORMAT_R32G32B32A32_FIXED
      },
   },
   {{0}}, /* unused (13) */
   {{0}}, /* unused (14) */
   {{0}}, /* unused (15) */
   {{0}}, /* unused (16) */
   {{0}}, /* unused (17) */
   {{0}}, /* unused (18) */
   {{0}}, /* unused (19) */
   {{0}}, /* unused (20) */
   {{0}}, /* unused (21) */
   {{0}}, /* unused (22) */
   {{0}}, /* unused (23) */
   {{0}}, /* unused (24) */
   {{0}}, /* unused (25) */
   {{0}}, /* unused (26) */
   {{0}}, /* unused (27) */
   {{0}}, /* unused (28) */
   {{0}}, /* unused (29) */
   {{0}}, /* unused (30) */
   { /* GL_INT_2_10_10_10_REV */
      {
         0,
         0,
         0,
         PIPE_FORMAT_R10G10B10A2_SSCALED
      },
      {
         0,
         0,
         0,
         PIPE_FORMAT_R10G10B10A2_SNORM
      },
   },
   {{0}}, /* unused (32) */
   { /* GL_HALF_FLOAT_OES */
      {
         PIPE_FORMAT_R16_FLOAT,
         PIPE_FORMAT_R16G16_FLOAT,
         PIPE_FORMAT_R16G16B16_FLOAT,
         PIPE_FORMAT_R16G16B16A16_FLOAT
      },
      {
         PIPE_FORMAT_R16_FLOAT,
         PIPE_FORMAT_R16G16_FLOAT,
         PIPE_FORMAT_R16G16B16_FLOAT,
         PIPE_FORMAT_R16G16B16A16_FLOAT
      },
   },
   {{0}}, /* unused (34) */
   {{0}}, /* unused (35) */
   {{0}}, /* unused (36) */
   {{0}}, /* unused (37) */
   {{0}}, /* unused (38) */
   {{0}}, /* unused (39) */
   { /* GL_UNSIGNED_INT_2_10_10_10_REV */
      {
         0,
         0,
         0,
         PIPE_FORMAT_R10G10B10A2_USCALED
      },
      {
         0,
         0,
         0,
         PIPE_FORMAT_R10G10B10A2_UNORM
      },
   },
   {{0}}, /* unused (41) */
   { /* GL_DOUBLE | (doubles << 5) (real double) */
     {
        PIPE_FORMAT_R64_UINT,
        PIPE_FORMAT_R64G64_UINT,
        PIPE_FORMAT_R64G64B64_UINT,
        PIPE_FORMAT_R64G64B64A64_UINT,
     },
   },
   {{0}}, /* unused (43) */
   {{0}}, /* unused (44) */
   {{0}}, /* unused (45) */
   {{0}}, /* unused (46) */
   { /* GL_UNSIGNED_INT64_ARB | (doubles << 5) (doubles is always true) */
     {0},
     {0},
     {
        PIPE_FORMAT_R64_UINT,
        PIPE_FORMAT_R64G64_UINT,
        PIPE_FORMAT_R64G64B64_UINT,
        PIPE_FORMAT_R64G64B64A64_UINT,
     },
   },
   {{0}}, /* unused (48) */
   {{0}}, /* unused (49) */
   {{0}}, /* unused (50) */
   {{0}}, /* unused (51) */
   {{0}}, /* unused (52) */
   {{0}}, /* unused (53) */
   {{0}}, /* unused (54) */
   {{0}}, /* unused (55) */
   {{0}}, /* unused (56) */
   {{0}}, /* unused (57) */
   {{0}}, /* unused (58) */
   { /* GL_UNSIGNED_INT_10F_11F_11F_REV */
      {
         0,
         0,
         PIPE_FORMAT_R11G11B10_FLOAT,
         0
      },
      {
         0,
         0,
         PIPE_FORMAT_R11G11B10_FLOAT,
         0
      },
   },
};


union Ruda_Vertex_Format_User {
   struct {
      unsigned short Type;        /**< datatype: GL_FLOAT, GL_INT, etc */
      bool Bgra;            /**< true if GL_BGRA, else GL_RGBA */
      uint8_t Size:5;       /**< components per element (1,2,3,4) */
      bool Normalized:1;    /**< GL_ARB_vertex_program */
      bool Integer:1;       /**< Integer-valued? */
      bool Doubles:1;       /**< double values are not converted to floats */
   };
   uint32_t All;
};

/* bgra_vertex_formats[type & 0x3][normalized] */
static const uint8_t bgra_vertex_formats[4][2] = {
   { /* GL_UNSIGNED_INT_2_10_10_10_REV */
      PIPE_FORMAT_B10G10R10A2_USCALED,
      PIPE_FORMAT_B10G10R10A2_UNORM
   },
   { /* GL_UNSIGNED_BYTE */
      0,
      PIPE_FORMAT_B8G8R8A8_UNORM
   },
   {0}, /* unused (2) */
   { /* GL_INT_2_10_10_10_REV */
      PIPE_FORMAT_B10G10R10A2_SSCALED,
      PIPE_FORMAT_B10G10R10A2_SNORM
   }
};


struct Ruda_Vertex_Format
{
   union Ruda_Vertex_Format_User User;
   enum pipe_format _PipeFormat:16; /**< pipe_format for Gallium */
   unsigned short _ElementSize; /**< Size of each element in bytes */
};


struct Ruda_Array_Attributes
{
   /** Points to client array data. Not used when a VBO is bound */
   const unsigned char *Ptr;
   /** Offset of the first element relative to the binding offset */
   uint RelativeOffset;
   /** Vertex format */
   struct Ruda_Vertex_Format Format;
   /** Stride as specified with gl*Pointer() */
   short Stride;
   /** Index into gl_vertex_array_object::BufferBinding[] array */
   unsigned char BufferBindingIndex;

   /**
    * Derived effective buffer binding index
    *
    * Index into the gl_vertex_buffer_binding array of the vao.
    * Similar to BufferBindingIndex, but with the mapping of the
    * position/generic0 attributes applied and with identical
    * gl_vertex_buffer_binding entries collapsed to a single
    * entry within the vao.
    *
    * The value is valid past calling _mesa_update_vao_derived_arrays.
    * Note that _mesa_update_vao_derived_arrays is called when binding
    * the VAO to Array._DrawVAO.
    */
   unsigned char _EffBufferBindingIndex;
   /**
    * Derived effective relative offset.
    *
    * Relative offset to the effective buffers offset in
    * gl_vertex_buffer_binding::_EffOffset.
    *
    * The value is valid past calling _mesa_update_vao_derived_arrays.
    * Note that _mesa_update_vao_derived_arrays is called when binding
    * the VAO to Array._DrawVAO.
    */
   unsigned short _EffRelativeOffset;
};

typedef enum
{
   VERT_ATTRIB_POS,
   VERT_ATTRIB_NORMAL,
   VERT_ATTRIB_COLOR0,
   VERT_ATTRIB_COLOR1,
   VERT_ATTRIB_FOG,
   VERT_ATTRIB_COLOR_INDEX,
   VERT_ATTRIB_TEX0,
   VERT_ATTRIB_TEX1,
   VERT_ATTRIB_TEX2,
   VERT_ATTRIB_TEX3,
   VERT_ATTRIB_TEX4,
   VERT_ATTRIB_TEX5,
   VERT_ATTRIB_TEX6,
   VERT_ATTRIB_TEX7,
   VERT_ATTRIB_POINT_SIZE,
   VERT_ATTRIB_GENERIC0,
   VERT_ATTRIB_GENERIC1,
   VERT_ATTRIB_GENERIC2,
   VERT_ATTRIB_GENERIC3,
   VERT_ATTRIB_GENERIC4,
   VERT_ATTRIB_GENERIC5,
   VERT_ATTRIB_GENERIC6,
   VERT_ATTRIB_GENERIC7,
   VERT_ATTRIB_GENERIC8,
   VERT_ATTRIB_GENERIC9,
   VERT_ATTRIB_GENERIC10,
   VERT_ATTRIB_GENERIC11,
   VERT_ATTRIB_GENERIC12,
   VERT_ATTRIB_GENERIC13,
   VERT_ATTRIB_GENERIC14,
   VERT_ATTRIB_GENERIC15,
   /* This must be last to keep VS inputs and vertex attributes in the same
    * order in st/mesa, and st/mesa always adds edgeflags as the last input.
    */
   VERT_ATTRIB_EDGEFLAG,
   VERT_ATTRIB_MAX
} Ruda_Vert_Attrib;

/**
 * This describes the buffer object used for a vertex array (or
 * multiple vertex arrays).  If BufferObj points to the default/null
 * buffer object, then the vertex array lives in user memory and not a VBO.
 */
struct gl_vertex_buffer_binding
{
   signed long int Offset;                    /**< User-specified offset */
   int Stride;                     /**< User-specified stride */
   uint InstanceDivisor;             /**< GL_ARB_instanced_arrays */
   struct Ruda_Buffer_Object *BufferObj; /**< GL_ARB_vertex_buffer_object */
   uint _BoundArrays;            /**< Arrays bound to this binding point */

   /**
    * Derived effective bound arrays.
    *
    * The effective binding handles enabled arrays past the
    * position/generic0 attribute mapping and reduces the refered
    * gl_vertex_buffer_binding entries to a unique subset.
    *
    * The value is valid past calling _mesa_update_vao_derived_arrays.
    * Note that _mesa_update_vao_derived_arrays is called when binding
    * the VAO to Array._DrawVAO.
    */
   uint _EffBoundArrays;
   /**
    * Derived offset.
    *
    * The absolute offset to that we can collapse some attributes
    * to this unique effective binding.
    * For user space array bindings this contains the smallest pointer value
    * in the bound and interleaved arrays.
    * For VBO bindings this contains an offset that lets the attributes
    * _EffRelativeOffset stay positive and in bounds with
    * Const.MaxVertexAttribRelativeOffset
    *
    * The value is valid past calling _mesa_update_vao_derived_arrays.
    * Note that _mesa_update_vao_derived_arrays is called when binding
    * the VAO to Array._DrawVAO.
    */
   signed long int _EffOffset;
};


struct Ruda_Vertex_Array_Object
{
	/** The index buffer (also known as the element array buffer in OpenGL). */
	struct Ruda_Buffer_Object *IndexBufferObj;

   /** Mask of VERT_BIT_* values indicating which arrays are enabled */
   unsigned int Enabled;

   char *Label;       /**< GL_KHR_debug */

   int RefCount;
      /**
    * Mask indicating which VertexAttrib and BufferBinding structures have
    * been changed since the VAO creation. No bit is ever cleared to 0 by
    * state updates. Setting to the default state doesn't update this.
    * (e.g. unbinding) Setting the derived state (_* fields) doesn't update
    * this either.
    */
   unsigned int NonDefaultStateMask;

   /** Vertex buffer bindings */
   struct gl_vertex_buffer_binding BufferBinding[VERT_ATTRIB_MAX];

   /** Mask indicating which vertex arrays have vertex buffer associated. */
   uint VertexAttribBufferMask;

   /** Mask indicating which vertex arrays have a non-zero instance divisor. */
   uint NonZeroDivisorMask;

   /** Mask of VERT_BIT_* values indicating which arrays are enabled */
   uint Enabled;




   /**
    * Marked to true if the object is shared between contexts and immutable.
    * Then reference counting is done using atomics and thread safe.
    * Is used for dlist VAOs.
    */
   bool SharedAndImmutable;
   
   /** Denotes the way the position/generic0 attribute is mapped */
   Ruda_Attribute_Map_Mode _AttributeMapMode;

   /** "Enabled" with the position/generic0 attribute aliasing resolved */
   unsigned int _EnabledWithMapMode;

      /**
    * Whether the VAO is changed by the application so often that some of
    * the derived fields are not updated at all to decrease overhead.
    * Also, interleaved arrays are not detected, because it's too expensive
    * to do that before every draw call.
    */
   bool IsDynamic;

   /** Vertex attribute arrays */
   struct Ruda_Array_Attributes VertexAttrib[VERT_ATTRIB_MAX];

};

struct gl_precision
{
   unsigned short RangeMin;   /**< min value exponent */
   unsigned short RangeMax;   /**< max value exponent */
   unsigned short Precision;  /**< number of mantissa bits */
};

/**
 * Vertex array state
 */

typedef enum
{
   VP_MODE_FF,     /**< legacy / fixed function */
   VP_MODE_SHADER, /**< ARB vertex program or GLSL vertex shader */
   VP_MODE_MAX     /**< for sizing arrays */
} Ruda_Vertex_Processing_Mode;

/**
 * Current attribute group (GL_CURRENT_BIT).
 */
struct Ruda_Current_Attrib
{
   /**
    * \name Current vertex attributes (color, texcoords, etc).
    * \note Values are valid only after FLUSH_VERTICES has been called.
    * \note Index and Edgeflag current values are stored as floats in the
    * SIX and SEVEN attribute slots.
    * \note We need double storage for 64-bit vertex attributes
    */
   float Attrib[VERT_ATTRIB_MAX][4*2];

   /**
    * \name Current raster position attributes (always up to date after a
    * glRasterPos call).
    */
   float RasterPos[4];
   float RasterDistance;
   float RasterColor[4];
   float RasterSecondaryColor[4];
   float RasterTexCoords[MAX_TEXTURE_COORD_UNITS][4];
   bool RasterPosValid;
};

struct Ruda_Array_Attrib
{
   /** Currently bound array object. */
   struct Ruda_Vertex_Array_Object *VAO;
   
	/* GL_ARB_vertex_buffer_object */
   struct Ruda_Buffer_Object *ArrayBufferObj;


   /**
    * Vertex array object that is used with the currently active draw command.
    * The _DrawVAO is either set to the currently bound VAO for array type
    * draws or to internal VAO's set up by the vbo module to execute immediate
    * mode or display list draws.
    */
   struct Ruda_Vertex_Array_Object *_DrawVAO;

   /**
    * If gallium vertex buffers are dirty, this flag indicates whether gallium
    * vertex elements are dirty too. If this is false, GL states corresponding
    * to vertex elements have not been changed. Thus, this affects what will
    * happen when ST_NEW_VERTEX_ARRAYS is set.
    *
    * The driver should clear this when it's done.
    */
   bool NewVertexElements;

   /**
    * Whether per-vertex edge flags are enabled and should be processed by
    * the vertex shader.
    */
   bool _PerVertexEdgeFlagsEnabled;
   /**
    * Whether all edge flags are false, causing all points and lines generated
    * by polygon mode to be not drawn. (i.e. culled)
    */
   bool _PolygonModeAlwaysCulls;

};
struct Ruda_Program_Constants
{
   /* logical limits */
   int MaxInstructions;
   int MaxAluInstructions;
   int MaxTexInstructions;
   int MaxTexIndirections;
   int MaxAttribs;
   int MaxTemps;
   int MaxAddressRegs;
   int MaxAddressOffset;  /**< [-MaxAddressOffset, MaxAddressOffset-1] */
   int MaxParameters;
   int MaxLocalParams;
   int MaxEnvParams;
   /* native/hardware limits */
   int MaxNativeInstructions;
   int MaxNativeAluInstructions;
   int MaxNativeTexInstructions;
   int MaxNativeTexIndirections;
   int MaxNativeAttribs;
   int MaxNativeTemps;
   int MaxNativeAddressRegs;
   int MaxNativeParameters;
   /* For shaders */
   int MaxUniformComponents;  /**< Usually == MaxParameters * 4 */

   /**
    * \name Per-stage input / output limits
    *
    * Previous to OpenGL 3.2, the intrastage data limits were advertised with
    * a single value: GL_MAX_VARYING_COMPONENTS (GL_MAX_VARYING_VECTORS in
    * ES).  This is stored as \c gl_constants::MaxVarying.
    *
    * Starting with OpenGL 3.2, the limits are advertised with per-stage
    * variables.  Each stage as a certain number of outputs that it can feed
    * to the next stage and a certain number inputs that it can consume from
    * the previous stage.
    *
    * Vertex shader inputs do not participate this in this accounting.
    * These are tracked exclusively by \c Ruda_Program_constants::MaxAttribs.
    *
    * Fragment shader outputs do not participate this in this accounting.
    * These are tracked exclusively by \c gl_constants::MaxDrawBuffers.
    */
   /*@{*/
   int MaxInputComponents;
   int MaxOutputComponents;
   /*@}*/

   /* ES 2.0 and GL_ARB_ES2_compatibility */
   struct gl_precision LowFloat, MediumFloat, HighFloat;
   struct gl_precision LowInt, MediumInt, HighInt;
   /* GL_ARB_uniform_buffer_object */
   int MaxUniformBlocks;
   uint64_t MaxCombinedUniformComponents;
   int MaxTextureImageUnits;

   /* GL_ARB_shader_atomic_counters */
   int MaxAtomicBuffers;
   int MaxAtomicCounters;

   /* GL_ARB_shader_image_load_store */
   int MaxImageUniforms;

   /* GL_ARB_shader_storage_buffer_object */
   int MaxShaderStorageBlocks;
};

struct Ruda_Program
{
   /** FIXME: This must be first until we split shader_info from nir_shader */
   //struct shader_info info;

   uint Id;
   int RefCount;
   unsigned char *String;  /**< Null-terminated program text */

   /** GL_VERTEX/FRAGMENT_PROGRAM_ARB, GL_GEOMETRY_PROGRAM_NV */
   unsigned short Target;
   unsigned short Format;    /**< String encoding format */

   bool _Used;        /**< Ever used for drawing? Used for debugging */

   struct nir_shader *nir;

   /* Saved and restored with metadata. Freed with ralloc. */
   void *driver_cache_blob;
   size_t driver_cache_blob_size;

   /** Is this program written to on disk shader cache */
   bool program_written_to_cache;

   /** whether to skip VARYING_SLOT_PSIZ in st_translate_stream_output_info() */
   bool skip_pointsize_xfb;

   /** A bitfield indicating which vertex shader inputs consume two slots
    *
    * This is used for mapping from single-slot input locations in the GL API
    * to dual-slot double input locations in the shader.  This field is set
    * once as part of linking and never updated again to ensure the mapping
    * remains consistent.
    *
    * Note: There may be dual-slot variables in the original shader source
    * which do not appear in this bitfield due to having been eliminated by
    * the compiler prior to DualSlotInputs being calculated.  There may also
    * be bits set in this bitfield which are set but which the shader never
    * reads due to compiler optimizations eliminating such variables after
    * DualSlotInputs is calculated.
    */
   uint64_t DualSlotInputs;
   /** Subset of OutputsWritten outputs written with non-zero index. */
   uint64_t SecondaryOutputsWritten;
   /** TEXTURE_x_BIT bitmask */
   uint16_t TexturesUsed[MAX_COMBINED_TEXTURE_IMAGE_UNITS];
   /** Bitfield of which samplers are used */
   unsigned int SamplersUsed;
   /** Texture units used for shadow sampling. */
   unsigned int ShadowSamplers;
   /** Texture units used for samplerExternalOES */
   unsigned int ExternalSamplersUsed;

   /** Named parameters, constants, etc. from program text */
   struct Ruda_Program_parameter_list *Parameters;

   /** Map from sampler unit to texture unit (set by glUniform1i()) */
   unsigned char SamplerUnits[MAX_SAMPLERS];

   struct pipe_shader_state state;
   struct ati_fragment_shader *ati_fs;
   uint64_t affected_states; /**< ST_NEW_* flags to mark dirty when binding */

   void *serialized_nir;
   unsigned serialized_nir_size;

   struct gl_shader_program *shader_program;

   struct st_variant *variants;

   union {
      /** Fields used by GLSL programs */
      struct {
         /** Data shared by Ruda_Program and gl_shader_program */
         struct gl_shader_program_data *data;

         struct gl_active_atomic_buffer **AtomicBuffers;

         /** Post-link transform feedback info. */
         struct gl_transform_feedback_info *LinkedTransformFeedback;

         /**
          * Number of types for subroutine uniforms.
          */
         uint NumSubroutineUniformTypes;

         /**
          * Subroutine uniform remap table
          * based on the program level uniform remap table.
          */
         uint NumSubroutineUniforms; /* non-sparse total */
         uint NumSubroutineUniformRemapTable;
         struct gl_uniform_storage **SubroutineUniformRemapTable;

         /**
          * Num of subroutine functions for this stage and storage for them.
          */
         uint NumSubroutineFunctions;
         uint MaxSubroutineFunctionIndex;
         struct gl_subroutine_function *SubroutineFunctions;

         /**
          * Map from image uniform index to image unit (set by glUniform1i())
          *
          * An image uniform index is associated with each image uniform by
          * the linker.  The image index associated with each uniform is
          * stored in the \c gl_uniform_storage::image field.
          */
         unsigned char ImageUnits[MAX_IMAGE_UNIFORMS];

         /** Access qualifier from linked shader
          */
         enum gl_access_qualifier image_access[MAX_IMAGE_UNIFORMS];

         uint NumUniformBlocks;
         struct gl_uniform_block **UniformBlocks;
         struct gl_uniform_block **ShaderStorageBlocks;

         /**
          * Bitmask of shader storage blocks not declared as read-only.
          */
         unsigned ShaderStorageBlocksWriteAccess;

         /** Which texture target is being sampled
          * (TEXTURE_1D/2D/3D/etc_INDEX)
          */
         unsigned char SamplerTargets[MAX_SAMPLERS];

         /**
          * Number of samplers declared with the bindless_sampler layout
          * qualifier as specified by ARB_bindless_texture.
          */
         uint NumBindlessSamplers;
         bool HasBoundBindlessSampler;
         struct gl_bindless_sampler *BindlessSamplers;

         /**
          * Number of images declared with the bindless_image layout qualifier
          * as specified by ARB_bindless_texture.
          */
         uint NumBindlessImages;
         bool HasBoundBindlessImage;
         struct gl_bindless_image *BindlessImages;
      } sh;

      /** ARB assembly-style program fields */
      struct {
         struct prog_instruction *Instructions;

         /**
          * Local parameters used by the program.
          *
          * It's dynamically allocated because it is rarely used (just
          * assembly-style programs), and MAX_PROGRAM_LOCAL_PARAMS entries
          * once it's allocated.
          */
         float (*LocalParams)[4];
         unsigned MaxLocalParams;

         /** Bitmask of which register files are read/written with indirect
          * addressing.  Mask of (1 << PROGRAM_x) bits.
          */
         unsigned int IndirectRegisterFiles;

         /** Logical counts */
         /*@{*/
         uint NumInstructions;
         uint NumTemporaries;
         uint NumParameters;
         uint NumAttributes;
         uint NumAddressRegs;
         uint NumAluInstructions;
         uint NumTexInstructions;
         uint NumTexIndirections;
         /*@}*/
         /** Native, actual h/w counts */
         /*@{*/
         uint NumNativeInstructions;
         uint NumNativeTemporaries;
         uint NumNativeParameters;
         uint NumNativeAttributes;
         uint NumNativeAddressRegs;
         uint NumNativeAluInstructions;
         uint NumNativeTexInstructions;
         uint NumNativeTexIndirections;
         /*@}*/

         /** Used by ARB assembly-style programs. Can only be true for vertex
          * programs.
          */
         bool IsPositionInvariant;

         /** Used by ARB_fp programs, enum gl_fog_mode */
         unsigned Fog;
      } arb;
   };
};




struct Ruda_Constants {
   struct Ruda_Program_Constants Program[MESA_SHADER_STAGES];
      /** Whether the vertex buffer offset is a signed 32-bit integer. */
   bool VertexBufferOffsetIsInt32;
      /** Use hardware accelerated GL_SELECT */
   bool HardwareAcceleratedSelect;

      /** GL_ARB_vertex_attrib_binding */
   int MaxVertexAttribRelativeOffset;

};

struct Ruda_Polygon_Attrib {
   unsigned FrontFace;		/**< Either GL_CW or GL_CCW */
   unsigned int FrontMode;		/**< Either GL_POINT, GL_LINE or GL_FILL */
   unsigned int BackMode;		/**< Either GL_POINT, GL_LINE or GL_FILL */
   bool CullFlag;		/**< Culling on/off flag */
   bool SmoothFlag;	/**< True if GL_POLYGON_SMOOTH is enabled */
   bool StippleFlag;	/**< True if GL_POLYGON_STIPPLE is enabled */
   unsigned CullFaceMode;	/**< Culling mode GL_FRONT or GL_BACK */
   float OffsetFactor;	/**< Polygon offset factor, from user */
   float OffsetUnits;		/**< Polygon offset units, from user */
   float OffsetClamp;		/**< Polygon offset clamp, from user */
   bool OffsetPoint;	/**< Offset in GL_POINT mode */
   bool OffsetLine;	/**< Offset in GL_LINE mode */
   bool OffsetFill;	/**< Offset in GL_FILL mode */
};

/**
 * Context state for vertex programs.
 */
struct Ruda_Vertex_Program_State
{
   bool Enabled;            /**< User-set GL_VERTEX_PROGRAM_ARB/NV flag */
   bool PointSizeEnabled;   /**< GL_VERTEX_PROGRAM_POINT_SIZE_ARB/NV */
   bool TwoSideEnabled;     /**< GL_VERTEX_PROGRAM_TWO_SIDE_ARB/NV */
   /** Whether the fixed-func program is being used right now. */
   bool _UsesTnlProgram;

   struct Ruda_Program *Current;  /**< User-bound vertex program */

   /** Currently enabled and valid vertex program (including internal
    * programs, user-defined vertex programs and GLSL vertex shaders).
    * This is the program we must use when rendering.
    */
   struct Ruda_Program *_Current;

   float Parameters[MAX_PROGRAM_ENV_PARAMS][4]; /**< Env params */

   /** Program to emulate fixed-function T&L (see above) */
   struct Ruda_Program *_TnlProgram;

   /** Cache of fixed-function programs */
   struct Ruda_Program_cache *Cache;

   bool _Overriden;

   bool _VPModeOptimizesConstantAttribs;

   /**
    * If we have a vertex program, a TNL program or no program at all.
    * Note that this value should be kept up to date all the time,
    * nevertheless its correctness is asserted in _mesa_update_state.
    * The reason is to avoid calling _mesa_update_state twice we need
    * this value on draw *before* actually calling _mesa_update_state.
    * Also it should need to get recomputed only on changes to the
    * vertex program which are heavyweight already.
    */
   Ruda_Vertex_Processing_Mode _VPMode;

   unsigned int _VaryingInputs;  /**< mask of VERT_BIT_* flags */
   unsigned int _VPModeInputFilter;
};


/** The MESA_VERBOSE var is a bitmask of these flags */
enum _verbose
{
   VERBOSE_VARRAY		= 0x0001,
   VERBOSE_TEXTURE		= 0x0002,
   VERBOSE_MATERIAL		= 0x0004,
   VERBOSE_PIPELINE		= 0x0008,
   VERBOSE_DRIVER		= 0x0010,
   VERBOSE_STATE		= 0x0020,
   VERBOSE_API			= 0x0040,
   VERBOSE_DISPLAY_LIST		= 0x0100,
   VERBOSE_LIGHTING		= 0x0200,
   VERBOSE_PRIMS		= 0x0400,
   VERBOSE_VERTS		= 0x0800,
   VERBOSE_DISASSEM		= 0x1000,
   VERBOSE_SWAPBUFFERS          = 0x4000
};


struct Ruda_Driver_Flags {
   /**
    * gl_context::AtomicBufferBindings
    */
   uint64_t NewAtomicBuffer;

   /** gl_context::Color::Alpha* */
   uint64_t NewAlphaTest;

   /** gl_context::Multisample::Enabled */
   uint64_t NewMultisampleEnable;

   /** gl_context::Multisample::(Min)SampleShading */
   uint64_t NewSampleShading;

   /** gl_context::Transform::ClipPlanesEnabled */
   uint64_t NewClipPlaneEnable;

   /** gl_context::Color::ClampFragmentColor */
   uint64_t NewFragClamp;

   /** Shader constants (uniforms, program parameters, state constants) */
   uint64_t NewShaderConstants[MESA_SHADER_STAGES];

   /** For GL_CLAMP emulation */
   uint64_t NewSamplersWithClamp;
};

struct Ruda_Shared_State
{
   simple_mtx_t Mutex;		   /**< for thread safety */
   int RefCount;			   /**< Reference count */
   bool DisplayListsAffectGLThread;

   struct _mesa_HashTable *DisplayList;	   /**< Display lists hash table */
   struct _mesa_HashTable *TexObjects;	   /**< Texture objects hash table */

   /** Default texture objects (shared by all texture units) */
   //struct gl_texture_object *DefaultTex[NUM_TEXTURE_TARGETS];

   /** Fallback texture used when a bound texture is incomplete */
   //struct gl_texture_object *FallbackTex[NUM_TEXTURE_TARGETS][2]; /**< [color, depth] */

   /**
    * \name Thread safety and statechange notification for texture
    * objects.
    *
    * \todo Improve the granularity of locking.
    */
   /*@{*/
   simple_mtx_t TexMutex;		/**< texobj thread safety */
   uint TextureStateStamp;	        /**< state notification for shared tex */
   /*@}*/

   /**
    * \name Vertex/geometry/fragment programs
    */
   /*@{*/
   struct _mesa_HashTable *Programs; /**< All vertex/fragment programs */
   //struct Ruda_Program *DefaultVertexProgram;
   //struct Ruda_Program *DefaultFragmentProgram;
   /*@}*/

   /* GL_ATI_fragment_shader */
   //struct _mesa_HashTable *ATIShaders;
   //struct ati_fragment_shader *DefaultFragmentShader;

   struct _mesa_HashTable *BufferObjects;

   /* Buffer objects released by a different context than the one that
    * created them. Since the creating context holds one global buffer
    * reference for each buffer it created and skips reference counting,
    * deleting a buffer by another context can't touch the buffer reference
    * held by the context that created it. Only the creating context can
    * remove its global buffer reference.
    *
    * This list contains all buffers that were deleted by a different context
    * than the one that created them. This list should be probed by all
    * contexts regularly and remove references of those buffers that they own.
    */
   //struct set *ZombieBufferObjects;

   /** Table of both gl_shader and gl_shader_program objects */
   struct _mesa_HashTable *ShaderObjects;

   /* GL_EXT_framebuffer_object */
   struct _mesa_HashTable *RenderBuffers;
   struct _mesa_HashTable *FrameBuffers;

   /* GL_ARB_sync */
   struct set *SyncObjects;

   /** GL_ARB_sampler_objects */
   struct _mesa_HashTable *SamplerObjects;

   /* GL_ARB_bindless_texture */
   struct hash_table_u64 *TextureHandles;
   struct hash_table_u64 *ImageHandles;
   mtx_t HandlesMutex; /**< For texture/image handles safety */

   /* GL_ARB_shading_language_include */
   struct shader_includes *ShaderIncludes;
   /* glCompileShaderInclude expects ShaderIncludes not to change while it is
    * in progress.
    */
   simple_mtx_t ShaderIncludeMutex;

   /** EXT_external_objects */
   struct _mesa_HashTable *MemoryObjects;

   /** EXT_semaphore */
   struct _mesa_HashTable *SemaphoreObjects;

   /**
    * Whether at least one image has been imported or exported, excluding
    * the default framebuffer. If this is false, glFlush can be executed
    * asynchronously because there is no invisible dependency on external
    * users.
    */
   bool HasExternallySharedImages;

   /* Small display list storage */
   struct {
      union gl_dlist_node *ptr;
      struct util_idalloc free_idx;
      unsigned size;
   } small_dlist_store;

   /* Global GLThread state. */
   struct {
      /* The last context that locked global mutexes. */
      struct gl_context *LastExecutingCtx;

      /* The last time LastExecutingCtx started executing after a different
       * context (the time of multiple active contexts).
       */
      int64_t LastContextSwitchTime;

      /* The time for which no context can lock global mutexes since
       * LastContextSwitchTime.
       */
      int64_t NoLockDuration;
   } GLThread;
};


struct Ruda_Context; 

struct Ruda_Context {
	/** State possibly shared with other contexts in the address space */
	struct Ruda_Shared_State *Shared;

	/** Whether Shared->BufferObjects has already been locked for this context. */
	bool BufferObjectsLocked;

	XGC xContext;

   Ruda_API API;

	struct pipe_context *pipe;
	struct vbo_context vbo_context;
	// binds together events + window objects
	XC xStore;
   
   struct Ruda_Current_Attrib Current;	/**< Current attributes */
	struct Ruda_Array_Attrib Array;	/**< Vertex arrays */
   struct Ruda_Polygon_Attrib	Polygon;	/**< Polygon attributes */
   struct Ruda_Vertex_Program_State VertexProgram;

   struct Ruda_Constants Const;

   uint64_t NewDriverState;  /**< bitwise-or of flags from DriverFlags */
   struct Ruda_Driver_Flags DriverFlags; /** Stores Flags for Driver*/

      /**
    * Device driver function pointer table
    */
   struct dd_function_table Driver;

   unsigned short RenderMode;      /**< either GL_RENDER, GL_SELECT, GL_FEEDBACK */
   uint NewState;      /**< bitwise-or of _NEW_* flags */
   uint PopAttribState; /**< Updated state since glPushAttrib */
   uint64_t NewDriverState;  /**< bitwise-or of flags from DriverFlags */

   /** Extension information */
   struct gl_extensions Extensions;

      /** Whether Shared->BufferObjects has already been locked for this context. */
   bool BufferObjectsLocked;
   /** Whether Shared->TexMutex has already been locked for this context. */
   bool TexturesLocked;

   uint TextureStateTimestamp; /**< detect changes to shared state */

   struct Ruda_Tess_Ctrl_Program_State TessCtrlProgram;

	Ruda_Context(XGC xContext) {this->xContext = xContext;};

};

typedef enum
{
   USAGE_UNIFORM_BUFFER = 0x1,
   USAGE_TEXTURE_BUFFER = 0x2,
   USAGE_ATOMIC_COUNTER_BUFFER = 0x4,
   USAGE_SHADER_STORAGE_BUFFER = 0x8,
   USAGE_TRANSFORM_FEEDBACK_BUFFER = 0x10,
   USAGE_PIXEL_PACK_BUFFER = 0x20,
   USAGE_ARRAY_BUFFER = 0x40,
   USAGE_DISABLE_MINMAX_CACHE = 0x100,
} Ruda_Buffer_Usage;

struct Ruda_Buffer_Mapping
{
   uint AccessFlags; /**< Mask of GL_MAP_x_BIT flags */
   void *Pointer;        /**< User-space address of mapping */
   signed long int Offset;        /**< Mapped offset */
   signed long int Length;      /**< Mapped length */
};


/**
 * GL_ARB_vertex/pixel_buffer_object buffer object
 */
struct Ruda_Buffer_Object
{
   int RefCount;
   uint Name;

   /**
    * The context that holds a global buffer reference for the lifetime of
    * the GL buffer ID to skip refcounting for all its private bind points.
    * Other contexts must still do refcounting as usual. Shared binding points
    * like TBO within gl_texture_object are always refcounted.
    *
    * Implementation details:
    * - Only the context that creates the buffer ("creating context") skips
    *   refcounting.
    * - Only buffers represented by an OpenGL buffer ID skip refcounting.
    *   Other internal buffers don't. (glthread requires refcounting for
    *   internal buffers, etc.)
    * - glDeleteBuffers removes the global buffer reference and increments
    *   RefCount for all private bind points where the deleted buffer is bound
    *   (e.g. unbound VAOs that are not changed by glDeleteBuffers),
    *   effectively enabling refcounting for that context. This is the main
    *   point where the global buffer reference is removed.
    * - glDeleteBuffers called from a different context adds the buffer into
    *   the ZombieBufferObjects list, which is a way to notify the creating
    *   context that it should remove its global buffer reference to allow
    *   freeing the buffer. The creating context walks over that list in a few
    *   GL functions.
    * - xxxDestroyContext walks over all buffers and removes its global
    *   reference from those buffers that it created.
    */
   struct Ruda_Context *Ctx;
   int CtxRefCount;   /**< Non-atomic references held by Ctx. */

   Ruda_Buffer_Usage UsageHistory; /**< How has this buffer been used so far? */

   struct pipe_resource *buffer;
   struct Ruda_Context* private_refcount_ctx;
   /* This mechanism allows passing buffer references to the driver without
    * using atomics to increase the reference count.
    *
    * This private refcount can be decremented without atomics but only one
    * context (ctx above) can use this counter to be thread-safe.
    *
    * This number is atomically added to buffer->reference.count at
    * initialization. If it's never used, the same number is atomically
    * subtracted from buffer->reference.count before destruction. If this
    * number is decremented, we can pass that reference to the driver without
    * touching reference.count. At buffer destruction we only subtract
    * the number of references we did not return. This can possibly turn
    * a million atomic increments into 1 add and 1 subtract atomic op.
    */
   int private_refcount;

   uint StorageFlags; /**< GL_MAP_PERSISTENT_BIT, etc. */

   /** Memoization of min/max index computations for static index buffers */
   unsigned MinMaxCacheHitIndices;
   unsigned MinMaxCacheMissIndices;
   struct hash_table *MinMaxCache;
   simple_mtx_t MinMaxCacheMutex;
   bool MinMaxCacheDirty:1;

   bool DeletePending:1;  /**< true if buffer object is removed from the hash */
   bool Immutable:1;    /**< GL_ARB_buffer_storage */
   bool HandleAllocated:1; /**< GL_ARB_bindless_texture */
   bool GLThreadInternal:1; /**< Created by glthread. */
   unsigned short Usage;      /**< GL_STREAM_DRAW_ARB, GL_STREAM_READ_ARB, etc. */
   char *Label;       /**< GL_KHR_debug */
   signed long int Size;  /**< Size of buffer storage in bytes */

   /** Counters used for buffer usage warnings */
   uint NumSubDataCalls;
   uint NumMapBufferWriteCalls;

   struct Ruda_Buffer_Mapping Mappings[3];
   struct pipe_transfer *transfer[3];
};



/* GL_ARB_vertex_buffer_object */
/* Modifies GL_MAP_UNSYNCHRONIZED_BIT to allow driver to fail (return
 * NULL) if buffer is unavailable for immediate mapping.
 *
 * Does GL_MAP_INVALIDATE_RANGE_BIT do this?  It seems so, but it
 * would require more book-keeping in the driver than seems necessary
 * at this point.
 *
 * Does GL_MAP_INVALDIATE_BUFFER_BIT do this?  Not really -- we don't
 * want to provoke the driver to throw away the old storage, we will
 * respect the contents of already referenced data.
 */
#define MESA_MAP_NOWAIT_BIT       0x4000

/* Mapping a buffer is allowed from any thread. */
#define MESA_MAP_THREAD_SAFE_BIT  0x8000

/* This buffer will only be mapped/unmapped once */
#define MESA_MAP_ONCE            0x10000

/* This BufferStorage flag indicates that the buffer will be used
 * by pipe_vertex_state, which doesn't track buffer busyness and doesn't
 * support invalidations.
 */
#define MESA_GALLIUM_VERTEX_STATE_STORAGE 0x20000


/**
 * Device driver function table.
 * Core Mesa uses these function pointers to call into device drivers.
 * Most of these functions directly correspond to OpenGL state commands.
 * Core Mesa will call these functions after error checking has been done
 * so that the drivers don't have to worry about error testing.
 *
 * Vertex transformation/clipping/lighting is patched into the T&L module.
 * Rasterization functions are patched into the swrast module.
 *
 * Note: when new functions are added here, the drivers/common/driverfuncs.c
 * file should be updated too!!!
 */
struct dd_function_table {
   /**
    * \name Vertex/fragment program functions
    */
   /** Allocate a new program */
   struct Ruda_Program * (*NewProgram)(struct gl_context *ctx,
                                     gl_shader_stage stage,
                                     uint id, bool is_arb_asm);
   /**
    * \name Draw functions. 
    */
   /*@{*/
   /**
    * For indirect array drawing:
    *
    *    typedef struct {
    *       uint count;
    *       uint primCount;
    *       uint first;
    *       uint baseInstance; // in GL 4.2 and later, must be zero otherwise
    *    } DrawArraysIndirectCommand;
    *
    * For indirect indexed drawing:
    *
    *    typedef struct {
    *       uint count;
    *       uint primCount;
    *       uint firstIndex;
    *       int  baseVertex;
    *       uint baseInstance; // in GL 4.2 and later, must be zero otherwise
    *    } DrawElementsIndirectCommand;
    */

   /**
    * Optimal Gallium version of Draw() that doesn't require translation
    * of draw info in the state tracker.
    *
    * The interface is identical to pipe_context::draw_vbo
    * with indirect == NULL.
    *
    * "info" is not const and the following fields can be changed by
    * the callee, so callers should be aware:
    * - info->index_bounds_valid (if false)
    * - info->min_index (if index_bounds_valid is false)
    * - info->max_index (if index_bounds_valid is false)
    * - info->drawid (if increment_draw_id is true)
    */
   void (*DrawGallium)(struct gl_context *ctx,
                       struct pipe_draw_info *info,
                       unsigned drawid_offset,
                       const struct pipe_draw_start_count_bias *draws,
                       unsigned num_draws);

   /**
    * Same as DrawGallium, but mode can also change between draws.
    *
    * "info" is not const and the following fields can be changed by
    * the callee in addition to the fields listed by DrawGallium:
    * - info->mode
    *
    * This function exists to decrease complexity of DrawGallium.
    */
   void (*DrawGalliumMultiMode)(struct gl_context *ctx,
                                struct pipe_draw_info *info,
                                const struct pipe_draw_start_count_bias *draws,
                                const unsigned char *mode,
                                unsigned num_draws);

   void (*DrawGalliumVertexState)(struct gl_context *ctx,
                                  struct pipe_vertex_state *state,
                                  struct pipe_draw_vertex_state_info info,
                                  const struct pipe_draw_start_count_bias *draws,
                                  const uint8_t *mode,
                                  unsigned num_draws);
   /*@}*/

   struct pipe_vertex_state *
      (*CreateGalliumVertexState)(struct gl_context *ctx,
                                  const struct gl_vertex_array_object *vao,
                                  struct gl_buffer_object *indexbuf,
                                  uint32_t enabled_attribs);

   /**
    * \name Support for multiple T&L engines
    */
   /*@{*/

   /**
    * Set by the driver-supplied T&L engine.  
    *
    * Set to PRIM_OUTSIDE_BEGIN_END when outside glBegin()/glEnd().
    */
   uint CurrentExecPrimitive;

   /**
    * Current glBegin state of an in-progress compilation.  May be
    * GL_POINTS, GL_TRIANGLE_STRIP, etc. or PRIM_OUTSIDE_BEGIN_END
    * or PRIM_UNKNOWN.
    */
   uint CurrentSavePrimitive;


#define FLUSH_STORED_VERTICES 0x1
#define FLUSH_UPDATE_CURRENT  0x2
   /**
    * Set by the driver-supplied T&L engine whenever vertices are buffered
    * between glBegin()/glEnd() objects or __struct gl_contextRec::Current
    * is not updated.  A bitmask of the FLUSH_x values above.
    *
    * The dd_function_table::FlushVertices call below may be used to resolve
    * these conditions.
    */
   unsigned int NeedFlush;

   /** Need to call vbo_save_SaveFlushVertices() upon state change? */
   bool SaveNeedFlush;

   /**@}*/

   /**
    * Query reset status for GL_ARB_robustness
    *
    * Per \c glGetGraphicsResetStatusARB, this function should return a
    * non-zero value once after a reset.  If a reset is non-atomic, the
    * non-zero status should be returned for the duration of the reset.
    */
   int (*GetGraphicsResetStatus)(struct gl_context *ctx);

   /**
    * \name GL_ARB_get_program_binary
    */
   /*@{*/
   /**
    * Calls to retrieve/store a binary serialized copy of the current program.
    */
   void (*ProgramBinarySerializeDriverBlob)(struct gl_context *ctx,
                                            struct gl_shader_program *shProg,
                                            struct Ruda_Program *prog);

   void (*ProgramBinaryDeserializeDriverBlob)(struct gl_context *ctx,
                                              struct gl_shader_program *shProg,
                                              struct Ruda_Program *prog);
   /*@}*/

   /**
    * \name Disk shader cache functions
    */
   /*@{*/
   /**
    * Called to initialize Ruda_Program::driver_cache_blob (and size) with a
    * ralloc allocated buffer.
    *
    * This buffer will be saved and restored as part of the Ruda_Program
    * serialization and deserialization.
    */
   void (*ShaderCacheSerializeDriverBlob)(struct gl_context *ctx,
                                          struct Ruda_Program *prog);
   /*@}*/

   //bool (*ValidateEGLImage)(struct gl_context *ctx, GLeglImageOES image_handle);
};

struct vbo_exec_context
{
   struct {
      /* Multi draw where the mode can vary between draws. */
      struct pipe_draw_info info;
      struct pipe_draw_start_count_bias draw[VBO_MAX_PRIM];
      GLubyte mode[VBO_MAX_PRIM];            /**< primitive modes per draw */
      struct vbo_markers markers[VBO_MAX_PRIM];
      unsigned prim_count;

      struct Ruda_Buffer_Object *bufferobj;

      uint vertex_size;       /* in dwords */
      uint vertex_size_no_pos;

      fi_type *buffer_map;
      fi_type *buffer_ptr;              /* cursor, points into buffer */
      uint   buffer_used;             /* in bytes */
      unsigned buffer_offset;           /* only for persistent mappings */
      fi_type vertex[VBO_ATTRIB_MAX*4]; /* current vertex */

      uint vert_count;   /**< Number of vertices currently in buffer */
      uint max_vert;     /**< Max number of vertices allowed in buffer */
      struct vbo_exec_copied_vtx copied;

      uint64_t enabled;             /**< mask of enabled vbo arrays. */

      /* Keep these packed in a structure for faster access. */
      struct {
         unsigned short type;       /**< GL_FLOAT, GL_DOUBLE, GL_INT, etc */
         unsigned char active_size; /**< number of components, but can shrink */
         unsigned char size;        /**< number of components (1..4) */
      } attr[VBO_ATTRIB_MAX];

      /** pointers into the current 'vertex' array, declared above */
      fi_type *attrptr[VBO_ATTRIB_MAX];
   } vtx;

   struct {
      bool recalculate_maps;
      struct vbo_exec_eval1_map map1[VERT_ATTRIB_MAX];
      struct vbo_exec_eval2_map map2[VERT_ATTRIB_MAX];
   } eval;

#ifndef NDEBUG
   int flush_call_depth;
#endif
};

struct vbo_exec_eval1_map {
   struct gl_1d_map *map;
   uint sz;
};

struct vbo_exec_eval2_map {
   struct gl_2d_map *map;
   uint sz;
};

struct vbo_exec_copied_vtx {
   fi_type buffer[VBO_ATTRIB_MAX * 4 * VBO_MAX_COPIED_VERTS];
   uint nr;
};


struct vbo_markers
{
   /**
    * If false and the primitive is a line loop, the first vertex is
    * the beginning of the line loop and it won't be drawn.
    * Instead, it will be moved to the end.
    *
    * Drivers shouldn't reset the line stipple pattern walker if begin is
    * false and mode is a line strip.
    */
   bool begin;

   /**
    * If true and the primitive is a line loop, it will be closed.
    */
   bool end;
};

typedef union { float f; int i; unsigned u; } fi_type;


/*
 * Note: The first 32 attributes match the VERT_ATTRIB_* definitions.
 * However, we have extra attributes for storing per-vertex glMaterial
 * values.  The material attributes get shifted into the generic positions
 * at draw time.
 *
 * One reason we can't alias materials and generics here is display lists.
 * A display list might contain both generic attributes and material
 * attributes which are selected at draw time depending on whether we're
 * using fixed function or a shader.  <sigh>
 */ 
enum vbo_attrib {
   VBO_ATTRIB_POS,
   VBO_ATTRIB_NORMAL,
   VBO_ATTRIB_COLOR0,
   VBO_ATTRIB_COLOR1,
   VBO_ATTRIB_FOG,
   VBO_ATTRIB_COLOR_INDEX,
   VBO_ATTRIB_TEX0,
   VBO_ATTRIB_TEX1,
   VBO_ATTRIB_TEX2,
   VBO_ATTRIB_TEX3,
   VBO_ATTRIB_TEX4,
   VBO_ATTRIB_TEX5,
   VBO_ATTRIB_TEX6,
   VBO_ATTRIB_TEX7,
   VBO_ATTRIB_POINT_SIZE,

   VBO_ATTRIB_GENERIC0, /* Not used? */
   VBO_ATTRIB_GENERIC1,
   VBO_ATTRIB_GENERIC2,
   VBO_ATTRIB_GENERIC3,
   VBO_ATTRIB_GENERIC4,
   VBO_ATTRIB_GENERIC5,
   VBO_ATTRIB_GENERIC6,
   VBO_ATTRIB_GENERIC7,
   VBO_ATTRIB_GENERIC8,
   VBO_ATTRIB_GENERIC9,
   VBO_ATTRIB_GENERIC10,
   VBO_ATTRIB_GENERIC11,
   VBO_ATTRIB_GENERIC12,
   VBO_ATTRIB_GENERIC13,
   VBO_ATTRIB_GENERIC14,
   VBO_ATTRIB_GENERIC15,
   VBO_ATTRIB_EDGEFLAG,

   /* XXX: in the vertex program inputs_read flag, we alias
    * materials and generics and use knowledge about the program
    * (whether it is a fixed-function emulation) to
    * differentiate.  Here we must keep them apart instead.
    */
   VBO_ATTRIB_MAT_FRONT_AMBIENT,
   VBO_ATTRIB_MAT_BACK_AMBIENT,
   VBO_ATTRIB_MAT_FRONT_DIFFUSE,
   VBO_ATTRIB_MAT_BACK_DIFFUSE,
   VBO_ATTRIB_MAT_FRONT_SPECULAR,
   VBO_ATTRIB_MAT_BACK_SPECULAR,
   VBO_ATTRIB_MAT_FRONT_EMISSION,
   VBO_ATTRIB_MAT_BACK_EMISSION,
   VBO_ATTRIB_MAT_FRONT_SHININESS,
   VBO_ATTRIB_MAT_BACK_SHININESS,
   VBO_ATTRIB_MAT_FRONT_INDEXES,
   VBO_ATTRIB_MAT_BACK_INDEXES,

   /* Offset into HW GL_SELECT result buffer. */
   VBO_ATTRIB_SELECT_RESULT_OFFSET,

   VBO_ATTRIB_MAX
};

/**
 * The VBO module implemented in src/vbo.
 */
struct vbo_context {
   struct Ruda_Array_Attributes current[VBO_ATTRIB_MAX];

   struct Ruda_Vertex_Array_Object *VAO;

   struct vbo_exec_context exec;
   struct vbo_save_context save;
};

struct vbo_save_context {
   uint64_t enabled; /**< mask of enabled vbo arrays. */
   unsigned char attrsz[VBO_ATTRIB_MAX];  /**< 1, 2, 3 or 4 */
   unsigned short attrtype[VBO_ATTRIB_MAX];  /**< GL_FLOAT, GL_INT, etc */
   unsigned char active_sz[VBO_ATTRIB_MAX];  /**< 1, 2, 3 or 4 */
   uint vertex_size;  /**< size in GLfloats */
   struct Ruda_Vertex_Array_Object *VAO[VP_MODE_MAX];

   struct vbo_save_vertex_store *vertex_store;
   struct vbo_save_primitive_store *prim_store;
   struct Ruda_Buffer_Object *current_bo;
   unsigned current_bo_bytes_used;

   fi_type vertex[VBO_ATTRIB_MAX*4];	   /* current values */
   fi_type *attrptr[VBO_ATTRIB_MAX];

   struct {
      fi_type *buffer;
      uint nr;
   } copied;

   fi_type *current[VBO_ATTRIB_MAX]; /* points into ctx->ListState */
   unsigned char *currentsz[VBO_ATTRIB_MAX];

   bool dangling_attr_ref;
   bool out_of_memory;  /**< True if last VBO allocation failed */
   bool no_current_update;
};

struct vbo_save_vertex_store {
   fi_type *buffer_in_ram;
   uint buffer_in_ram_size;
   uint used;           /**< Number of 4-byte words used in buffer */
};

struct vbo_save_primitive_store {
   struct Ruda_Prim *prims;
   uint used;
   uint size;
};

struct Ruda_Prim
{
   unsigned char mode;    /**< GL_POINTS, GL_LINES, GL_QUAD_STRIP, etc */

   /**
    * tnl: If true, line stipple emulation will reset the pattern walker.
    * vbo: If false and the primitive is a line loop, the first vertex is
    *      the beginning of the line loop and it won't be drawn.
    *      Instead, it will be moved to the end.
    */
   bool begin;

   /**
    * tnl: If true and the primitive is a line loop, it will be closed.
    * vbo: Same as tnl.
    */
   bool end;

   uint start;
   uint count;
   int basevertex;
   uint draw_id;
};

/**
 * Context state for tessellation control programs.
 */
struct Ruda_Tess_Ctrl_Program_State
{
   /** Currently bound and valid shader. */
   struct Ruda_Program *_Current;

   int patch_vertices;
   float patch_default_outer_level[4];
   float patch_default_inner_level[2];
};

/**
 * A framebuffer is a collection of renderbuffers (color, depth, stencil, etc).
 * In C++ terms, think of this as a base class from which device drivers
 * will make derived classes.
 */
struct Ruda_Framebuffer
{
   simple_mtx_t Mutex;  /**< for thread safety */
   /**
    * If zero, this is a window system framebuffer.  If non-zero, this
    * is a FBO framebuffer; note that for some devices (i.e. those with
    * a natural pixel coordinate system for FBOs that differs from the
    * OpenGL/Mesa coordinate system), this means that the viewport,
    * polygon face orientation, and polygon stipple will have to be inverted.
    */
   uint Name;
   GLint RefCount;

   GLchar *Label;       /**< GL_KHR_debug */

   bool DeletePending;

   /**
    * The framebuffer's visual. Immutable if this is a window system buffer.
    * Computed from attachments if user-made FBO.
    */
   struct Ruda_Config Visual;

   /**
    * Size of frame buffer in pixels. If there are no attachments, then both
    * of these are 0.
    */
   uint Width, Height;

   /**
    * In the case that the framebuffer has no attachment (i.e.
    * GL_ARB_framebuffer_no_attachments) then the geometry of
    * the framebuffer is specified by the default values.
    */
   struct {
     uint Width, Height, Layers, NumSamples;
     bool FixedSampleLocations;
     /* Derived from NumSamples by the driver so that it can choose a valid
      * value for the hardware.
      */
     uint _NumSamples;
   } DefaultGeometry;

   /** \name  Drawing bounds (Intersection of buffer size and scissor box)
    * The drawing region is given by [_Xmin, _Xmax) x [_Ymin, _Ymax),
    * (inclusive for _Xmin and _Ymin while exclusive for _Xmax and _Ymax)
    */
   /*@{*/
   int _Xmin, _Xmax;
   int _Ymin, _Ymax;
   /*@}*/

   /** \name  Derived Z buffer stuff */
   /*@{*/
   uint _DepthMax;	/**< Max depth buffer value */
   GLfloat _DepthMaxF;	/**< Float max depth buffer value */
   GLfloat _MRD;	/**< minimum resolvable difference in Z values */
   /*@}*/

   /** One of the GL_FRAMEBUFFER_(IN)COMPLETE_* tokens */
   unsigned short _Status;

   /** Whether one of Attachment has Type != GL_NONE
    * NOTE: the values for Width and Height are set to 0 in case of having
    * no attachments, a backend driver supporting the extension
    * GL_ARB_framebuffer_no_attachments must check for the flag _HasAttachments
    * and if GL_FALSE, must then use the values in DefaultGeometry to initialize
    * its viewport, scissor and so on (in particular _Xmin, _Xmax, _Ymin and
    * _Ymax do NOT take into account _HasAttachments being false). To get the
    * geometry of the framebuffer, the  helper functions
    *   _mesa_geometric_width(),
    *   _mesa_geometric_height(),
    *   _mesa_geometric_samples() and
    *   _mesa_geometric_layers()
    * are available that check _HasAttachments.
    */
   bool _HasAttachments;

   uint _IntegerBuffers;  /**< Which color buffers are integer valued */
   uint _BlendForceAlphaToOne;  /**< Which color buffers need blend factor adjustment */
   uint _IsRGB;  /**< Which color buffers have an RGB base format? */
   uint _FP32Buffers; /**< Which color buffers are FP32 */

   /* ARB_color_buffer_float */
   bool _AllColorBuffersFixedPoint; /* no integer, no float */
   bool _HasSNormOrFloatColorBuffer;

   /**
    * The maximum number of layers in the framebuffer, or 0 if the framebuffer
    * is not layered.  For cube maps and cube map arrays, each cube face
    * counts as a layer. As the case for Width, Height a backend driver
    * supporting GL_ARB_framebuffer_no_attachments must use DefaultGeometry
    * in the case that _HasAttachments is false
    */
   uint MaxNumLayers;

   /** Array of all renderbuffer attachments, indexed by BUFFER_* tokens. */
   struct gl_renderbuffer_attachment Attachment[BUFFER_COUNT];
   struct pipe_resource *resolve; /**< color resolve attachment */

   /* In unextended OpenGL these vars are part of the GL_COLOR_BUFFER
    * attribute group and GL_PIXEL attribute group, respectively.
    */
   unsigned short ColorDrawBuffer[MAX_DRAW_BUFFERS];
   unsigned short ColorReadBuffer;

   /* GL_ARB_sample_locations */
   GLfloat *SampleLocationTable; /**< If NULL, no table has been specified */
   bool ProgrammableSampleLocations;
   bool SampleLocationPixelGrid;

   /** Computed from ColorDraw/ReadBuffer above */
   uint _NumColorDrawBuffers;
   Ruda_Buffer_Index _ColorDrawBufferIndexes[MAX_DRAW_BUFFERS];
   Ruda_Buffer_Index _ColorReadBufferIndex;
   struct gl_renderbuffer *_ColorDrawBuffers[MAX_DRAW_BUFFERS];
   struct gl_renderbuffer *_ColorReadBuffer;

   /* GL_MESA_framebuffer_flip_y */
   bool FlipY;

   /** Delete this framebuffer */
   void (*Delete)(struct gl_framebuffer *fb);

   struct pipe_frontend_drawable *drawable;
   enum st_attachment_type statts[ST_ATTACHMENT_COUNT];
   unsigned num_statts;
   int32_t stamp;
   int32_t drawable_stamp;
   uint32_t drawable_ID;

   /* list of framebuffer objects */
   struct list_head head;
};