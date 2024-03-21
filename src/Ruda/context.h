#include "Ruda/structs.h"

#ifndef MESA_VERBOSE
int MESA_VERBOSE = 0;
#endif



/**
 * Flush vertices.
 *
 * \param ctx Ruda context.
 * \param newstate new state.
 *
 * Checks if dd_function_table::NeedFlush is marked to flush stored vertices,
 * and calls dd_function_table::FlushVertices if so. Marks
 * __struct gl_contextRec::NewState with \p newstate.
 */
#define FLUSH_VERTICES(ctx, newstate, pop_attrib_mask)          \
do {								\
   ctx->NewState |= newstate;					\
   ctx->PopAttribState |= pop_attrib_mask;                      \
} while (0)

//   if (ctx->Driver.NeedFlush & FLUSH_STORED_VERTICES)		
//    vbo_exec_FlushVertices(ctx, FLUSH_STORED_VERTICES);	

/**
 * Are we currently between glBegin and glEnd?
 * During execution, not display list compilation.
 */
static inline bool
_mesa_inside_begin_end(const struct Ruda_Context *ctx)
{
   return ctx->Driver.CurrentExecPrimitive != PRIM_OUTSIDE_BEGIN_END;
}

static inline bool
_mesa_hw_select_enabled(const struct Ruda_Context *ctx)
{
   return ctx->RenderMode == RUDA_SELECT &&
      ctx->Const.HardwareAcceleratedSelect;
}