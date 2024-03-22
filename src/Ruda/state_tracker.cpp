#include "Ruda/ruda.h"

static void
st_unbind_program(struct st_context *st, struct Ruda_Program *p)
{
   struct Ruda_Context *ctx = st->ctx;

   /* Unbind the shader in cso_context and re-bind in st/mesa. */
   switch (p->info.stage) {
   case MESA_SHADER_VERTEX:
      cso_set_vertex_shader_handle(st->cso_context, NULL);
      ctx->NewDriverState |= ST_NEW_VS_STATE;
      break;
   case MESA_SHADER_TESS_CTRL:
      cso_set_tessctrl_shader_handle(st->cso_context, NULL);
      ctx->NewDriverState |= ST_NEW_TCS_STATE;
      break;
   case MESA_SHADER_TESS_EVAL:
      cso_set_tesseval_shader_handle(st->cso_context, NULL);
      ctx->NewDriverState |= ST_NEW_TES_STATE;
      break;
   case MESA_SHADER_GEOMETRY:
      cso_set_geometry_shader_handle(st->cso_context, NULL);
      ctx->NewDriverState |= ST_NEW_GS_STATE;
      break;
   case MESA_SHADER_FRAGMENT:
      cso_set_fragment_shader_handle(st->cso_context, NULL);
      ctx->NewDriverState |= ST_NEW_FS_STATE;
      break;
   case MESA_SHADER_COMPUTE:
      cso_set_compute_shader_handle(st->cso_context, NULL);
      ctx->NewDriverState |= ST_NEW_CS_STATE;
      break;
   default:
      unreachable("invalid shader type");
   }
}

/**
 * Free all basic program variants.
 */
void
st_release_variants(struct st_context *st, struct gl_program *p)
{
   struct st_variant *v;

   /* If we are releasing shaders, re-bind them, because we don't
    * know which shaders are bound in the driver.
    */
   if (p->variants)
      st_unbind_program(st, p);

   for (v = p->variants; v; ) {
      struct st_variant *next = v->next;
      delete_variant(st, v, p->Target);
      v = next;
   }

   p->variants = NULL;

   /* Note: Any setup of ->ir.nir that has had pipe->create_*_state called on
    * it has resulted in the driver taking ownership of the NIR.  Those
    * callers should be NULLing out the nir field in any pipe_shader_state
    * that might have this called in order to indicate that.
    *
    * GLSL IR and ARB programs will have set gl_program->nir to the same
    * shader as ir->ir.nir, so it will be freed by _mesa_delete_program().
    */
}
