#include "Ruda/structs.h"

#define FLUSH_VERTICES(ctx, newstate, pop_attrib_mask)          \
do {								\		\
   if (ctx->Driver.NeedFlush & FLUSH_STORED_VERTICES)		\
      vbo_exec_FlushVertices(ctx, FLUSH_STORED_VERTICES);	\
   ctx->NewState |= newstate;					\
   ctx->PopAttribState |= pop_attrib_mask;                      \
} while (0)