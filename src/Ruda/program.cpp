#include "Ruda/ruda.h"
#include "../src/util/ralloc.h"

/**
 * Delete a program and remove it from the hash table, ignoring the
 * reference count.
 */
void
_mesa_delete_program(struct Ruda_Context *ctx, struct Ruda_Program *prog)
{
   struct st_context *st = st_context(ctx);
   assert(prog);
   assert(prog->RefCount==0);

   st_release_variants(st, prog);

   free(prog->serialized_nir);

   if (prog == &_mesa_DummyProgram)
      return;

   if (prog->Parameters) {
      _mesa_free_parameter_list(prog->Parameters);
   }

   if (prog->nir) {
      ralloc_free(prog->nir);
   }

   if (prog->sh.BindlessSamplers) {
      ralloc_free(prog->sh.BindlessSamplers);
   }

   if (prog->sh.BindlessImages) {
      ralloc_free(prog->sh.BindlessImages);
   }

   if (prog->driver_cache_blob) {
      ralloc_free(prog->driver_cache_blob);
   }

   ralloc_free(prog);
}


void
_mesa_reference_program_(struct Ruda_Context *ctx,
                         struct Ruda_Program **ptr,
                         struct Ruda_Program *prog)
{
#ifndef NDEBUG
   assert(ptr);
   if (*ptr && prog) {
      /* sanity check */
      if ((*ptr)->Target == RUDA_VERTEX_PROGRAM_ARB)
         assert(prog->Target == RUDA_VERTEX_PROGRAM_ARB);
      else if ((*ptr)->Target == RUDA_FRAGMENT_PROGRAM_ARB)
         assert(prog->Target == RUDA_FRAGMENT_PROGRAM_ARB ||
                prog->Target == RUDA_FRAGMENT_PROGRAM_NV);
      else if ((*ptr)->Target == RUDA_GEOMETRY_PROGRAM_NV)
         assert(prog->Target == RUDA_GEOMETRY_PROGRAM_NV);
   }
#endif

   if (*ptr) {
      struct Ruda_Program *oldProg = *ptr;

      assert(oldProg->RefCount > 0);

      if (p_atomic_dec_zero(&oldProg->RefCount)) {
         assert(ctx);
         _mesa_reference_shader_program_data(&oldProg->sh.data, NULL);
         _mesa_delete_program(ctx, oldProg);
      }

      *ptr = NULL;
   }

   assert(!*ptr);
   if (prog) {
      p_atomic_inc(&prog->RefCount);
   }

   *ptr = prog;
}



static inline void
_mesa_reference_program(struct Ruda_Context *ctx,
                        struct Ruda_Program **ptr,
                        struct Ruda_Program *prog)
{
   if (*ptr != prog)
      _mesa_reference_program_(ctx, ptr, prog);
}