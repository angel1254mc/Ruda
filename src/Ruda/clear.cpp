#include "Ruda/structs.h"
#include "RudaDI/di_structs.h"
#include <string.h>
#include "../src/Ruda/state_tracker/st_atom_list.h"
#include "buffer_object.cpp"

static inline struct vbo_context *
vbo_context(struct Ruda_Context *ctx)
{
   return &ctx->vbo_context;
}

#if defined(__has_cpp_attribute) && defined(__clang__)
/* We do not do the same trick as __has_attribute because parsing
 * clang::fallthrough in the preprocessor fails in GCC. */
#  define HAS_CLANG_FALLTHROUGH  __has_cpp_attribute(clang::fallthrough)
#else
#  define HAS_CLANG_FALLTHROUGH 0
#endif

#if (defined(__cplusplus) && (__cplusplus >= 201703L)) || \
    (defined(__STDC_VERSION__) && (__STDC_VERSION__ > 201710L))
/* Standard C++17/C23 attribute */
#define FALLTHROUGH [[fallthrough]]
#elif HAS_CLANG_FALLTHROUGH
/* Clang++ specific */
#define FALLTHROUGH [[clang::fallthrough]]
#elif __has_attribute(fallthrough)
/* Non-standard but supported by at least gcc and clang */
#define FALLTHROUGH __attribute__((fallthrough))
#else
#define FALLTHROUGH do { } while(0)
#endif


/**
 * Copy zero, one or two vertices from the current vertex buffer into
 * the temporary "copy" buffer.
 * This is used when a single primitive overflows a vertex buffer and
 * we need to continue the primitive in a new vertex buffer.
 * The temporary "copy" buffer holds the vertices which need to get
 * copied from the old buffer to the new one.
 */
unsigned
vbo_copy_vertices(struct Ruda_Context *ctx,
                  unsigned int mode,
                  unsigned start, unsigned *pcount, bool begin,
                  unsigned vertex_size,
                  bool in_dlist,
                  fi_type *dst,
                  const fi_type *src)
{
   const unsigned count = *pcount;
   unsigned copy = 0;

   switch (mode) {
   case RUDA_POINTS:
      return 0;
   case RUDA_LINES:
      copy = count % 2;
      break;
   case RUDA_TRIS:
      copy = count % 3;
      break;
   case RUDA_QUADS:
   case RUDA_LINES_ADJACENCY:
      copy = count % 4;
      break;
   case RUDA_TRIANGLES_ADJACENCY:
      copy = count % 6;
      break;
   case RUDA_LINE_STRIP:
      copy = MIN2(1, count);
      break;
   case RUDA_LINE_STRIP_ADJACENCY:
      /* We need to copy 3 vertices, because:
       *    Last strip:  ---o---o---x     (last line)
       *    Next strip:     x---o---o---  (next line)
       */
      copy = MIN2(3, count);
      break;
   case RUDA_PATCHES:
      if (in_dlist) {
         /* We don't know the value of RUDA_PATCH_VERTICES when compiling
          * a display list.
          *
          * Fail an assertion in debug builds and use the value of 3
          * in release builds, which is more likely than any other value.
          */
         assert(!"patch_vertices is unknown");
         copy = count % 3;
      } else {
         copy = count % ctx->TessCtrlProgram.patch_vertices;
      }
      break;
   case RUDA_LINE_LOOP:
      if (!in_dlist && begin == 0) {
         /* We're dealing with the second or later section of a split/wrapped
          * RUDA_LINE_LOOP.  Since we're converting line loops to line strips,
          * we've already incremented the last_prim->start counter by one to
          * skip the 0th vertex in the loop.  We need to undo that (effectively
          * subtract one from last_prim->start) so that we copy the 0th vertex
          * to the next vertex buffer.
          */
         assert(start > 0);
         src -= vertex_size;
      }
      FALLTHROUGH;
   case RUDA_TRI_FAN:
   case RUDA_POLYGON:
      if (count == 0) {
         return 0;
      } else if (count == 1) {
         memcpy(dst, src + 0, vertex_size * sizeof(GLfloat));
         return 1;
      } else {
         memcpy(dst, src + 0, vertex_size * sizeof(GLfloat));
         memcpy(dst + vertex_size, src + (count - 1) * vertex_size,
                vertex_size * sizeof(GLfloat));
         return 2;
      }
   case RUDA_TRI_STRIP:
      /* Draw an even number of triangles to keep front/back facing the same. */
      *pcount -= count % 2;
      FALLTHROUGH;
   case RUDA_QUAD_STRIP:
      if (count <= 1)
         copy = count;
      else
         copy = 2 + (count % 2);
      break;
   case PRIM_OUTSIDE_BEGIN_END:
      return 0;
   case RUDA_TRIANGLE_STRIP_ADJACENCY:
      /* TODO: Splitting tri strips with adjacency is too complicated. */
   default:
      unreachable("Unexpected primitive type");
      return 0;
   }

   memcpy(dst, src + (count - copy) * vertex_size,
          copy * vertex_size * sizeof(GLfloat));
   return copy;
}

static inline struct Ruda_Context *
ruda_context_from_vbo_exec(struct vbo_exec_context *exec)
{
   return container_of(exec, struct Ruda_Context, vbo_context.exec);
}

static uint
vbo_exec_copy_vertices(struct vbo_exec_context *exec)
{
   struct Ruda_Context *ctx = ruda_context_from_vbo_exec(exec);
   const uint sz = exec->vtx.vertex_size;
   fi_type *dst = exec->vtx.copied.buffer;
   unsigned last = exec->vtx.prim_count - 1;
   unsigned start = exec->vtx.draw[last].start;
   const fi_type *src = exec->vtx.buffer_map + start * sz;

   return vbo_copy_vertices(ctx, ctx->Driver.CurrentExecPrimitive,
                            start,
                            &exec->vtx.draw[last].count,
                            exec->vtx.markers[last].begin,
                            sz, false, dst, src);
}



/**
 * Translate the bitmask of VBO_ATTRIB_BITs to VERT_ATTRIB_BITS.
 * Note that position/generic0 attribute aliasing is done
 * generically in the VAO.
 */
static inline uint
_vbo_get_vao_enabled_from_vbo(Ruda_Vertex_Processing_Mode vertex_processing_mode,
                              uint64_t enabled)
{
   if (vertex_processing_mode == VP_MODE_FF) {
      /* The materials mapped into the generic arrays */
      return (((uint)enabled) & VERT_BIT_FF_ALL)
         | (((uint)(enabled >> VBO_MATERIAL_SHIFT)) & VERT_BIT_MAT_ALL);
   } else {
      return enabled;
   }
}

/**
 * Get the filter mask for vbo draws depending on the vertex_processing_mode.
 */
static inline uint
_vbo_get_vao_filter(Ruda_Vertex_Processing_Mode vertex_processing_mode)
{
   if (vertex_processing_mode == VP_MODE_FF) {
      /* The materials mapped into the generic arrays */
      return VERT_BIT_FF_ALL | VERT_BIT_MAT_ALL;
   } else {
      return VERT_BIT_ALL;
   }
}

/**
 * Set the per-vertex edge flag enablement according to the "enable"
 * parameter. If "enable" is false, the zero-stride edge flag attribute value
 * will be used instead.
 *
 * This is used by VAOs, glBegin/End and display lists.
 */
void
_mesa_update_edgeflag_state_explicit(struct Ruda_Context *ctx,
                                     bool per_vertex_enable)
{
   if (ctx->API != API_OPENGL_COMPAT)
      return;

   /* Edge flags take effect only if the polygon mode is not FILL, and they
    * determine whether a line or point is drawn with that polygon mode.
    */
   bool edgeflags_have_effect = ctx->Polygon.FrontMode != RUDA_FILL ||
                                ctx->Polygon.BackMode != RUDA_FILL;
   per_vertex_enable &= edgeflags_have_effect;

   if (per_vertex_enable != ctx->Array._PerVertexEdgeFlagsEnabled) {
      ctx->Array._PerVertexEdgeFlagsEnabled = per_vertex_enable;

      struct Ruda_Program *vp = ctx->VertexProgram._Current;
      if (vp) {
         ctx->NewDriverState |= ST_NEW_VS_STATE |
                                ST_NEW_VERTEX_ARRAYS;
         ctx->Array.NewVertexElements = true;
      }
   }

   /* If there are no per-vertex edge flags and the zero-stride edge flag is
    * false, all front and back points and lines generated by polygon mode
    * are not drawn.
    */
   bool polygon_mode_always_culls = edgeflags_have_effect &&
                                    !ctx->Array._PerVertexEdgeFlagsEnabled &&
                                    !ctx->Current.Attrib[VERT_ATTRIB_EDGEFLAG][0];
   if (polygon_mode_always_culls != ctx->Array._PolygonModeAlwaysCulls) {
      ctx->Array._PolygonModeAlwaysCulls = polygon_mode_always_culls;
      ctx->NewDriverState |= ST_NEW_RASTERIZER;
   }
}


/**
 * Set the edge flag state using the current VAO and the zero-stride
 * edge flag attribute value if per-vertex edge flags are disabled.
 */
void
_mesa_update_edgeflag_state_vao(struct Ruda_Context *ctx)
{
   _mesa_update_edgeflag_state_explicit(ctx,
                                        ctx->Array._DrawVAO->Enabled &
                                        VERT_BIT_EDGEFLAG);
}

/**
 * Apply the position/generic0 aliasing map to a bitfield from the vao.
 * Use for example to convert gl_vertex_array_object::Enabled
 * or gl_vertex_buffer_binding::_VertexBinding from the vao numbering to
 * the numbering used with vertex processing inputs.
 */
static inline uint
_mesa_vao_enable_to_vp_inputs(Ruda_Attribute_Map_Mode mode, uint enabled)
{
   switch (mode) {
   case ATTRIBUTE_MAP_MODE_IDENTITY:
      return enabled;
   case ATTRIBUTE_MAP_MODE_POSITION:
      /* Copy VERT_ATTRIB_POS enable bit into GENERIC0 position */
      return (enabled & ~VERT_BIT_GENERIC0)
         | ((enabled & VERT_BIT_POS) << VERT_ATTRIB_GENERIC0);
   case ATTRIBUTE_MAP_MODE_GENERIC0:
      /* Copy VERT_ATTRIB_GENERIC0 enable bit into POS position */
      return (enabled & ~VERT_BIT_POS)
         | ((enabled & VERT_BIT_GENERIC0) >> VERT_ATTRIB_GENERIC0);
   default:
      return 0;
   }
}



void
_mesa_disable_vertex_array_attribs(struct Ruda_Context *ctx,
                                   struct Ruda_Vertex_Array_Object *vao,
                                   uint attrib_bits)
{
   assert(!vao->SharedAndImmutable);

   /* Only work on bits that are enabled */
   attrib_bits &= vao->Enabled;
   if (attrib_bits) {
      vao->Enabled &= ~attrib_bits;
      ctx->NewDriverState |= ST_NEW_VERTEX_ARRAYS;
      ctx->Array.NewVertexElements = true;


      /* Update the map mode if needed */
      if (attrib_bits && (VERT_BIT_POS|VERT_BIT_GENERIC0)) update_attribute_map_mode(ctx, vao);

      if (attrib_bits & VERT_BIT_EDGEFLAG)
         _mesa_update_edgeflag_state_vao(ctx);

      vao->_EnabledWithMapMode =
         _mesa_vao_enable_to_vp_inputs(vao->_AttributeMapMode, vao->Enabled);
   }
}


const GLubyte
_vbo_attribute_alias_map[VP_MODE_MAX][VERT_ATTRIB_MAX] = {
   /* VP_MODE_FF: */
   {
      VBO_ATTRIB_POS,                 /* VERT_ATTRIB_POS */
      VBO_ATTRIB_NORMAL,              /* VERT_ATTRIB_NORMAL */
      VBO_ATTRIB_COLOR0,              /* VERT_ATTRIB_COLOR0 */
      VBO_ATTRIB_COLOR1,              /* VERT_ATTRIB_COLOR1 */
      VBO_ATTRIB_FOG,                 /* VERT_ATTRIB_FOG */
      VBO_ATTRIB_COLOR_INDEX,         /* VERT_ATTRIB_COLOR_INDEX */
      VBO_ATTRIB_TEX0,                /* VERT_ATTRIB_TEX0 */
      VBO_ATTRIB_TEX1,                /* VERT_ATTRIB_TEX1 */
      VBO_ATTRIB_TEX2,                /* VERT_ATTRIB_TEX2 */
      VBO_ATTRIB_TEX3,                /* VERT_ATTRIB_TEX3 */
      VBO_ATTRIB_TEX4,                /* VERT_ATTRIB_TEX4 */
      VBO_ATTRIB_TEX5,                /* VERT_ATTRIB_TEX5 */
      VBO_ATTRIB_TEX6,                /* VERT_ATTRIB_TEX6 */
      VBO_ATTRIB_TEX7,                /* VERT_ATTRIB_TEX7 */
      VBO_ATTRIB_POINT_SIZE,          /* VERT_ATTRIB_POINT_SIZE */
      VBO_ATTRIB_GENERIC0,            /* VERT_ATTRIB_GENERIC0 */
      VBO_ATTRIB_GENERIC1,            /* VERT_ATTRIB_GENERIC1 */
      VBO_ATTRIB_GENERIC2,            /* VERT_ATTRIB_GENERIC2 */
      VBO_ATTRIB_SELECT_RESULT_OFFSET,/* VERT_ATTRIB_GENERIC3 */
      VBO_ATTRIB_MAT_FRONT_AMBIENT,   /* VERT_ATTRIB_GENERIC4 */
      VBO_ATTRIB_MAT_BACK_AMBIENT,    /* VERT_ATTRIB_GENERIC5 */
      VBO_ATTRIB_MAT_FRONT_DIFFUSE,   /* VERT_ATTRIB_GENERIC6 */
      VBO_ATTRIB_MAT_BACK_DIFFUSE,    /* VERT_ATTRIB_GENERIC7 */
      VBO_ATTRIB_MAT_FRONT_SPECULAR,  /* VERT_ATTRIB_GENERIC8 */
      VBO_ATTRIB_MAT_BACK_SPECULAR,   /* VERT_ATTRIB_GENERIC9 */
      VBO_ATTRIB_MAT_FRONT_EMISSION,  /* VERT_ATTRIB_GENERIC10 */
      VBO_ATTRIB_MAT_BACK_EMISSION,   /* VERT_ATTRIB_GENERIC11 */
      VBO_ATTRIB_MAT_FRONT_SHININESS, /* VERT_ATTRIB_GENERIC12 */
      VBO_ATTRIB_MAT_BACK_SHININESS,  /* VERT_ATTRIB_GENERIC13 */
      VBO_ATTRIB_MAT_FRONT_INDEXES,   /* VERT_ATTRIB_GENERIC14 */
      VBO_ATTRIB_MAT_BACK_INDEXES,    /* VERT_ATTRIB_GENERIC15 */
      VBO_ATTRIB_EDGEFLAG,            /* VERT_ATTRIB_EDGEFLAG */
   },

   /* VP_MODE_SHADER: */
   {
      VBO_ATTRIB_POS,                 /* VERT_ATTRIB_POS */
      VBO_ATTRIB_NORMAL,              /* VERT_ATTRIB_NORMAL */
      VBO_ATTRIB_COLOR0,              /* VERT_ATTRIB_COLOR0 */
      VBO_ATTRIB_COLOR1,              /* VERT_ATTRIB_COLOR1 */
      VBO_ATTRIB_FOG,                 /* VERT_ATTRIB_FOG */
      VBO_ATTRIB_COLOR_INDEX,         /* VERT_ATTRIB_COLOR_INDEX */
      VBO_ATTRIB_TEX0,                /* VERT_ATTRIB_TEX0 */
      VBO_ATTRIB_TEX1,                /* VERT_ATTRIB_TEX1 */
      VBO_ATTRIB_TEX2,                /* VERT_ATTRIB_TEX2 */
      VBO_ATTRIB_TEX3,                /* VERT_ATTRIB_TEX3 */
      VBO_ATTRIB_TEX4,                /* VERT_ATTRIB_TEX4 */
      VBO_ATTRIB_TEX5,                /* VERT_ATTRIB_TEX5 */
      VBO_ATTRIB_TEX6,                /* VERT_ATTRIB_TEX6 */
      VBO_ATTRIB_TEX7,                /* VERT_ATTRIB_TEX7 */
      VBO_ATTRIB_POINT_SIZE,          /* VERT_ATTRIB_POINT_SIZE */
      VBO_ATTRIB_GENERIC0,            /* VERT_ATTRIB_GENERIC0 */
      VBO_ATTRIB_GENERIC1,            /* VERT_ATTRIB_GENERIC1 */
      VBO_ATTRIB_GENERIC2,            /* VERT_ATTRIB_GENERIC2 */
      VBO_ATTRIB_GENERIC3,            /* VERT_ATTRIB_GENERIC3 */
      VBO_ATTRIB_GENERIC4,            /* VERT_ATTRIB_GENERIC4 */
      VBO_ATTRIB_GENERIC5,            /* VERT_ATTRIB_GENERIC5 */
      VBO_ATTRIB_GENERIC6,            /* VERT_ATTRIB_GENERIC6 */
      VBO_ATTRIB_GENERIC7,            /* VERT_ATTRIB_GENERIC7 */
      VBO_ATTRIB_GENERIC8,            /* VERT_ATTRIB_GENERIC8 */
      VBO_ATTRIB_GENERIC9,            /* VERT_ATTRIB_GENERIC9 */
      VBO_ATTRIB_GENERIC10,           /* VERT_ATTRIB_GENERIC10 */
      VBO_ATTRIB_GENERIC11,           /* VERT_ATTRIB_GENERIC11 */
      VBO_ATTRIB_GENERIC12,           /* VERT_ATTRIB_GENERIC12 */
      VBO_ATTRIB_GENERIC13,           /* VERT_ATTRIB_GENERIC13 */
      VBO_ATTRIB_GENERIC14,           /* VERT_ATTRIB_GENERIC14 */
      VBO_ATTRIB_GENERIC15,           /* VERT_ATTRIB_GENERIC15 */
      VBO_ATTRIB_EDGEFLAG,            /* VERT_ATTRIB_EDGEFLAG */
   }
};


/**
 * Return if format is integer. The immediate mode commands only emit floats
 * for non-integer types, thus everything else is integer.
 */
static inline GLboolean
vbo_attrtype_to_integer_flag(uint format)
{
   switch (format) {
   case RUDA_FLOAT:
   case RUDA_DOUBLE:
      return RUDA_FALSE;
   case RUDA_INT:
   case RUDA_UNSIGNED_INT:
   case RUDA_UNSIGNED_INT64_ARB:
      return RUDA_TRUE;
   default:
      unreachable("Bad vertex attribute type");
      return RUDA_FALSE;
   }
}

static inline GLboolean
vbo_attrtype_to_double_flag(GLenum format)
{
   switch (format) {
   case RUDA_FLOAT:
   case RUDA_INT:
   case RUDA_UNSIGNED_INT:
      return RUDA_FALSE;
   case RUDA_UNSIGNED_INT64_ARB:
   case RUDA_DOUBLE:
      return RUDA_TRUE;
   default:
      unreachable("Bad vertex attribute type");
      return RUDA_FALSE;
   }
}

/**
 * Set the vertex attrib for vbo draw use.
 */
static inline void
_vbo_set_attrib_format(struct Ruda_Context *ctx,
                       struct Ruda_Vertex_Array_Object *vao,
                       Ruda_Vert_Attrib attr, signed long int buffer_offset,
                       unsigned char size, unsigned short type, uint offset)
{
   const GLboolean integer = vbo_attrtype_to_integer_flag(type);
   const GLboolean doubles = vbo_attrtype_to_double_flag(type);

   if (doubles)
      size /= 2;
   _mesa_update_array_format(ctx, vao, attr, size, type, RUDA_RGBA, integer, doubles, offset);

   if (vao->Enabled & VERT_BIT(attr)) {
      ctx->NewDriverState |= ST_NEW_VERTEX_ARRAYS;
      ctx->Array.NewVertexElements = true;
   }

   vao->VertexAttrib[attr].Ptr = ADD_POINTERS(buffer_offset, offset);
}

void
_mesa_save_and_set_draw_vao(struct Ruda_Context *ctx,
                            struct Ruda_Vertex_Array_Object *vao,
                            uint vp_input_filter,
                            struct Ruda_Vertex_Array_Object **old_vao,
                            uint *old_vp_input_filter)
{
   *old_vao = ctx->Array._DrawVAO;
   *old_vp_input_filter = ctx->VertexProgram._VPModeInputFilter;

   ctx->Array._DrawVAO = NULL;
   ctx->VertexProgram._VPModeInputFilter = vp_input_filter;
   _mesa_set_draw_vao(ctx, vao);
}

/**
 * Want to figure out which fragment program inputs are actually
 * constant/current values from ctx->Current.  These should be
 * referenced as a tracked state variable rather than a fragment
 * program input, to save the overhead of putting a constant value in
 * every submitted vertex, transferring it to hardware, interpolating
 * it across the triangle, etc...
 *
 * When there is a VP bound, just use vp->outputs.  But when we're
 * generating vp from fixed function state, basically want to
 * calculate:
 *
 * vp_out_2_fp_in( vp_in_2_vp_out( varying_inputs ) |
 *                 potential_vp_outputs )
 *
 * Where potential_vp_outputs is calculated by looking at enabled
 * texgen, etc.
 *
 * The generated fragment program should then only declare inputs that
 * may vary or otherwise differ from the ctx->Current values.
 * Otherwise, the fp should track them as state values instead.
 */
void
_mesa_set_varying_vp_inputs(struct Ruda_Context *ctx, uint varying_inputs)
{
   if (ctx->VertexProgram._VPModeOptimizesConstantAttribs &&
       ctx->VertexProgram._VaryingInputs != varying_inputs) {
      ctx->VertexProgram._VaryingInputs = varying_inputs;
      ctx->NewState |= _NEW_FF_VERT_PROGRAM | _NEW_FF_FRAG_PROGRAM;
   }
}




/* TODO: populate these as the vertex is defined:
 */
static void
vbo_exec_bind_arrays(struct Ruda_Context *ctx,
                     struct Ruda_Vertex_Array_Object **old_vao,
                     uint *old_vp_input_filter)
{
   struct vbo_context *vbo = vbo_context(ctx);
   struct Ruda_Vertex_Array_Object *vao = vbo->VAO;
   struct vbo_exec_context *exec = &vbo->exec;

   GLintptr buffer_offset;
   if (exec->vtx.bufferobj) {
      assert(exec->vtx.bufferobj->Mappings[MAP_INTERNAL].Pointer);
      buffer_offset = exec->vtx.bufferobj->Mappings[MAP_INTERNAL].Offset +
                      exec->vtx.buffer_offset;
   } else {
      /* Ptr into ordinary app memory */
      buffer_offset = (GLbyte *)exec->vtx.buffer_map - (GLbyte *)NULL;
   }

   const Ruda_Vertex_Processing_Mode mode = ctx->VertexProgram._VPMode;

   uint vao_enabled, vao_filter;
   if (_mesa_hw_select_enabled(ctx)) {
      /* HW RUDA_SELECT has fixed input */
      vao_enabled = vao_filter = VERT_BIT_POS | VERT_BIT_SELECT_RESULT_OFFSET;
   } else {
      vao_enabled = _vbo_get_vao_enabled_from_vbo(mode, exec->vtx.enabled);
      vao_filter = _vbo_get_vao_filter(mode);
   }

   /* At first disable arrays no longer needed */
   _mesa_disable_vertex_array_attribs(ctx, vao, ~vao_enabled);
   assert((~vao_enabled & vao->Enabled) == 0);

   /* Bind the buffer object */
   const GLuint stride = exec->vtx.vertex_size*sizeof(GLfloat);
   _mesa_bind_vertex_buffer(ctx, vao, 0, exec->vtx.bufferobj, buffer_offset,
                            stride, false, false);

   /* Retrieve the mapping from VBO_ATTRIB to VERT_ATTRIB space
    * Note that the position/generic0 aliasing is done in the VAO.
    */
   const unsigned char *const vao_to_vbo_map = _vbo_attribute_alias_map[mode];
   /* Now set the enabled arrays */
   GLbitfield mask = vao_enabled;
   while (mask) {
      const int vao_attr = u_bit_scan(&mask);
      const unsigned char vbo_attr = vao_to_vbo_map[vao_attr];

      const unsigned char size = exec->vtx.attr[vbo_attr].size;
      const unsigned short type = exec->vtx.attr[vbo_attr].type;
      const uint offset = (GLuint)((GLbyte *)exec->vtx.attrptr[vbo_attr] -
                                     (GLbyte *)exec->vtx.vertex);
      assert(offset <= ctx->Const.MaxVertexAttribRelativeOffset);

      /* Set and enable */
      _vbo_set_attrib_format(ctx, vao, (Ruda_Vert_Attrib) vao_attr, buffer_offset,
                             size, type, offset);

      /* The vao is initially created with all bindings set to 0. */
      assert(vao->VertexAttrib[vao_attr].BufferBindingIndex == 0);
   }
   enable_vertex_array_attribs(ctx, vao, vao_enabled);
   assert(vao_enabled == vao->Enabled);
   assert(!exec->vtx.bufferobj ||
          (vao_enabled & ~vao->VertexAttribBufferMask) == 0);

   _mesa_save_and_set_draw_vao(ctx, vao, vao_filter,
                               old_vao, old_vp_input_filter);
   _mesa_set_varying_vp_inputs(ctx, vao_filter &
                               ctx->Array._DrawVAO->_EnabledWithMapMode);
}
// true facts
/**
 * Execute the buffer and save copied verts.
 */
void
vbo_exec_vtx_flush(struct vbo_exec_context *exec)
{
   struct Ruda_Context *ctx = ruda_context_from_vbo_exec(exec);

   /* Only unmap if persistent mappings are unsupported. */
   bool persistent_mapping = ctx->Extensions.ARB_buffer_storage &&
                             exec->vtx.bufferobj &&
                             exec->vtx.buffer_map;

   // if (0)
   //    vbo_exec_debug_verts(exec);

   if (exec->vtx.prim_count &&
       exec->vtx.vert_count) {

      exec->vtx.copied.nr = vbo_exec_copy_vertices(exec);

      if (exec->vtx.copied.nr != exec->vtx.vert_count) {
         struct Ruda_Vertex_Array_Object *old_vao;
         GLbitfield old_vp_input_filter;

         /* Prepare and set the Begin/End internal VAO for drawing. */
         vbo_exec_bind_arrays(ctx, &old_vao, &old_vp_input_filter);

         if (ctx->NewState)
            _mesa_update_state(ctx);

         if (!persistent_mapping)
            vbo_exec_vtx_unmap(exec);

         assert(ctx->NewState == 0);

         if (0)
            printf("%s %d %d\n", __func__, exec->vtx.prim_count,
                   exec->vtx.vert_count);

         ctx->Driver.DrawGalliumMultiMode(ctx, &exec->vtx.info,
                                          exec->vtx.draw,
                                          exec->vtx.mode,
                                          exec->vtx.prim_count);

         /* Get new storage -- unless asked not to. */
         if (!persistent_mapping)
            vbo_exec_vtx_map(exec);

         _mesa_restore_draw_vao(ctx, old_vao, old_vp_input_filter);
      }
   }

   if (persistent_mapping) {
      exec->vtx.buffer_used += (exec->vtx.buffer_ptr - exec->vtx.buffer_map) *
                               sizeof(float);
      exec->vtx.buffer_map = exec->vtx.buffer_ptr;

      /* Set the buffer offset for the next draw. */
      exec->vtx.buffer_offset = exec->vtx.buffer_used;

      if (!vbo_exec_buffer_has_space(exec)) {
         /* This will allocate a new buffer. */
         vbo_exec_vtx_unmap(exec);
         vbo_exec_vtx_map(exec);
      }
   }

   if (exec->vtx.vertex_size == 0)
      exec->vtx.max_vert = 0;
   else
      exec->vtx.max_vert = vbo_compute_max_verts(exec);

   exec->vtx.buffer_ptr = exec->vtx.buffer_map;
   exec->vtx.prim_count = 0;
   exec->vtx.vert_count = 0;
}


/**
 * Flush (draw) vertices.
 *
 * \param flags  bitmask of FLUSH_STORED_VERTICES, FLUSH_UPDATE_CURRENT
 */
static void
vbo_exec_FlushVertices_internal(struct vbo_exec_context *exec, unsigned flags)
{
   struct Ruda_Context *ctx = ruda_context_from_vbo_exec(exec);

   if (flags & FLUSH_STORED_VERTICES) {
      if (exec->vtx.vert_count) {
         vbo_exec_vtx_flush(exec);
      }

      if (exec->vtx.vertex_size) {
         vbo_exec_copy_to_current(exec);
         vbo_reset_all_attr(exec);
      }

      /* All done. */
      ctx->Driver.NeedFlush = 0;
   } else {
      assert(flags == FLUSH_UPDATE_CURRENT);

      /* Note that the vertex size is unchanged.
       * (vbo_reset_all_attr isn't called)
       */
      vbo_exec_copy_to_current(exec);

      /* Only FLUSH_UPDATE_CURRENT is done. */
      ctx->Driver.NeedFlush = ~FLUSH_UPDATE_CURRENT;
   }
}

/**
 * If inside glBegin()/glEnd(), it should assert(0).  Otherwise, if
 * FLUSH_STORED_VERTICES bit in \p flags is set flushes any buffered
 * vertices, if FLUSH_UPDATE_CURRENT bit is set updates
 * __struct gl_contextRec::Current and gl_light_attrib::Material
 *
 * Note that the default T&L engine never clears the
 * FLUSH_UPDATE_CURRENT bit, even after performing the update.
 *
 * \param flags  bitmask of FLUSH_STORED_VERTICES, FLUSH_UPDATE_CURRENT
 */
void
vbo_exec_FlushVertices(struct Ruda_Context *ctx, uint flags)
{
   struct vbo_exec_context *exec = &vbo_context(ctx)->exec;

#ifndef NDEBUG
   /* debug check: make sure we don't get called recursively */
   exec->flush_call_depth++;
   assert(exec->flush_call_depth == 1);
#endif

   if (_mesa_inside_begin_end(ctx)) {
      /* We've had glBegin but not glEnd! */
#ifndef NDEBUG
      exec->flush_call_depth--;
      assert(exec->flush_call_depth == 0);
#endif
      return;
   }

   /* Flush (draw). */
   vbo_exec_FlushVertices_internal(exec, flags);

#ifndef NDEBUG
   exec->flush_call_depth--;
   assert(exec->flush_call_depth == 0);
#endif
}

/**
 * Flush vertices.
 *
 * \param ctx GL context.
 * \param newstate new state.
 *
 * Checks if dd_function_table::NeedFlush is marked to flush stored vertices,
 * and calls dd_function_table::FlushVertices if so. Marks
 * __struct gl_contextRec::NewState with \p newstate.
 */
#define FLUSH_VERTICES(ctx, newstate, pop_attrib_mask)          \
do {								\
   if (ctx->Driver.NeedFlush & FLUSH_STORED_VERTICES)		\
      vbo_exec_FlushVertices(ctx, FLUSH_STORED_VERTICES);	\
   ctx->NewState |= newstate;					\
   ctx->PopAttribState |= pop_attrib_mask;                      \
} while (0)

/**
 * Clear buffers.
 *
 * \param mask bit-mask indicating the buffers to be cleared.
 *
 * Flushes the vertices and verifies the parameter.
 * If __struct gl_contextRec::NewState is set then calls _mesa_update_clear_state()
 * to update gl_frame_buffer::_Xmin, etc.  If the rasterization mode is set to
 * RUDA_RENDER then requests the driver to clear the buffers, via the
 * dd_function_table::Clear callback.
 */
static ALWAYS_INLINE void
clear(struct Ruda_Context *ctx, uint mask, bool no_error)
{
   FLUSH_VERTICES(ctx, 0, 0);

   // if (!no_error) {
   //    if (mask & ~(RUDA_COLOR_BUFFER_BIT |
   //                 RUDA_DEPTH_BUFFER_BIT |
   //                 RUDA_STENCIL_BUFFER_BIT |
   //                 RUDA_ACCUM_BUFFER_BIT)) {
   //       _mesa_error( ctx, RUDA_INVALID_VALUE, "glClear(0x%x)", mask);
   //       return;
   //    }

   //    /* Accumulation buffers were removed in core contexts, and they never
   //     * existed in OpenGL ES.
   //     */
   //    if ((mask & RUDA_ACCUM_BUFFER_BIT) != 0
   //        && (_mesa_is_desktop_gl_core(ctx) || _mesa_is_gles(ctx))) {
   //       _mesa_error( ctx, RUDA_INVALID_VALUE, "glClear(RUDA_ACCUM_BUFFER_BIT)");
   //       return;
   //    }
   // }

   if (ctx->NewState) {
      _mesa_update_clear_state( ctx );	/* update _Xmin, etc */
   }

   if (!no_error && ctx->DrawBuffer->_Status != RUDA_FRAMEBUFFER_COMPLETE_EXT) {
      _mesa_error(ctx, RUDA_INVALID_FRAMEBUFFER_OPERATION_EXT,
                  "glClear(incomplete framebuffer)");
      return;
   }

   if (ctx->RasterDiscard)
      return;

   if (ctx->RenderMode == RUDA_RENDER) {
      GLbitfield bufferMask;

      /* don't clear depth buffer if depth writing disabled */
      if (!ctx->Depth.Mask)
         mask &= ~RUDA_DEPTH_BUFFER_BIT;

      /* Build the bitmask to send to device driver's Clear function.
       * Note that the RUDA_COLOR_BUFFER_BIT flag will expand to 0, 1, 2 or 4
       * of the BUFFER_BIT_FRONT/BACK_LEFT/RIGHT flags, or one of the
       * BUFFER_BIT_COLORn flags.
       */
      bufferMask = 0;
      if (mask & RUDA_COLOR_BUFFER_BIT) {
         GLuint i;
         for (i = 0; i < ctx->DrawBuffer->_NumColorDrawBuffers; i++) {
            gl_buffer_index buf = ctx->DrawBuffer->_ColorDrawBufferIndexes[i];

            if (buf != BUFFER_NONE && color_buffer_writes_enabled(ctx, i)) {
               bufferMask |= 1 << buf;
            }
         }
      }

      if ((mask & RUDA_DEPTH_BUFFER_BIT)
          && ctx->DrawBuffer->Visual.depthBits > 0) {
         bufferMask |= BUFFER_BIT_DEPTH;
      }

      if ((mask & RUDA_STENCIL_BUFFER_BIT)
          && ctx->DrawBuffer->Visual.stencilBits > 0) {
         bufferMask |= BUFFER_BIT_STENCIL;
      }

      if ((mask & RUDA_ACCUM_BUFFER_BIT)
          && ctx->DrawBuffer->Visual.accumRedBits > 0) {
         bufferMask |= BUFFER_BIT_ACCUM;
      }

      st_Clear(ctx, bufferMask);
   }
}


void _mesa_Clear(uint mask)
{
   Ruda_Context* ctx = structure->ctx;
   clear(ctx, mask, false);
};

