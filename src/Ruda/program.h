
#include "Ruda/ruda.h"
#include "../src/util/ralloc.h"
static inline void
_mesa_reference_program(struct Ruda_Context *ctx,
                        struct Ruda_Program **ptr,
                        struct Ruda_Program *prog);

void
_mesa_reference_program_(struct Ruda_Context *ctx,
                         struct Ruda_Program **ptr,
                         struct Ruda_Program *prog);

void
_mesa_delete_program(struct Ruda_Context *ctx, struct Ruda_Program *prog);