#include "Ruda/ruda.h"

void
_mesa_lock_context_textures( struct Ruda_Context *ctx )
{
   if (!ctx->TexturesLocked)
      simple_mtx_lock(&ctx->Shared->TexMutex);

   if (ctx->Shared->TextureStateStamp != ctx->TextureStateTimestamp) {
      ctx->NewState |= _NEW_TEXTURE_OBJECT;
      ctx->PopAttribState |= RUDA_TEXTURE_BIT;
      ctx->TextureStateTimestamp = ctx->Shared->TextureStateStamp;
   }
}


/**
 * Compute derived GL state.
 * If __struct gl_contextRec::NewState is non-zero then this function \b must
 * be called before rendering anything.
 *
 * Calls dd_function_table::UpdateState to perform any internal state
 * management necessary.
 * 
 * \sa _mesa_update_modelview_project(), _mesa_update_texture(),
 * _mesa_update_buffer_bounds(),
 * _mesa_update_lighting() and _mesa_update_tnl_spaces().
 */
void
_mesa_update_state_locked( struct Ruda_Context *ctx )
{
   uint new_state = ctx->NewState;
   uint new_prog_state = 0x0;
   const uint checked_states =
      _NEW_BUFFERS | _NEW_MODELVIEW | _NEW_PROJECTION | _NEW_TEXTURE_MATRIX |
      _NEW_TEXTURE_OBJECT | _NEW_TEXTURE_STATE | _NEW_PROGRAM |
      _NEW_LIGHT_CONSTANTS | _NEW_POINT | _NEW_FF_VERT_PROGRAM |
      _NEW_FF_FRAG_PROGRAM | _NEW_TNL_SPACES;

   /* we can skip a bunch of state validation checks if the dirty
    * state matches one or more bits in 'computed_states'.
    */
   if (!(new_state & checked_states))
      goto out;

//    if (MESA_VERBOSE & VERBOSE_STATE)
//       _mesa_print_state("_mesa_update_state", new_state);

   if (new_state & _NEW_BUFFERS)
      _mesa_update_framebuffer(ctx, ctx->ReadBuffer, ctx->DrawBuffer);

   /* Handle Core and Compatibility contexts separately. */
   if (_mesa_is_desktop_gl_compat(ctx) ||
       _mesa_is_gles1(ctx)) {
      /* Update derived state. */
      if (new_state & (_NEW_MODELVIEW|_NEW_PROJECTION))
         _mesa_update_modelview_project( ctx, new_state );

      if (new_state & _NEW_TEXTURE_MATRIX)
         new_state |= _mesa_update_texture_matrices(ctx);

      if (new_state & (_NEW_TEXTURE_OBJECT | _NEW_TEXTURE_STATE | _NEW_PROGRAM))
         new_state |= _mesa_update_texture_state(ctx);

      if (new_state & _NEW_LIGHT_CONSTANTS)
         new_state |= _mesa_update_lighting(ctx);

      /* ctx->_NeedEyeCoords is determined here.
       *
       * If the truth value of this variable has changed, update for the
       * new lighting space and recompute the positions of lights and the
       * normal transform.
       *
       * If the lighting space hasn't changed, may still need to recompute
       * light positions & normal transforms for other reasons.
       */
      if (new_state & (_NEW_TNL_SPACES | _NEW_LIGHT_CONSTANTS |
                       _NEW_MODELVIEW)) {
         if (_mesa_update_tnl_spaces(ctx, new_state))
            new_state |= _NEW_FF_VERT_PROGRAM;
      }

      if (new_state & _NEW_PROGRAM)
         update_fixed_func_program_usage(ctx);

      /* Determine which states affect fixed-func vertex/fragment program. */
      uint prog_flags = _NEW_PROGRAM;

      if (ctx->FragmentProgram._UsesTexEnvProgram) {
         prog_flags |= _NEW_BUFFERS | _NEW_TEXTURE_OBJECT |
                       _NEW_FF_FRAG_PROGRAM | _NEW_TEXTURE_STATE;
      }

      if (ctx->VertexProgram._UsesTnlProgram)
         prog_flags |= _NEW_FF_VERT_PROGRAM;

      if (new_state & prog_flags) {
         /* When we generate programs from fixed-function vertex/fragment state
          * this call may generate/bind a new program.  If so, we need to
          * propogate the _NEW_PROGRAM flag to the driver.
          */
         new_prog_state |= update_program(ctx);
      }
   } else {
      /* GL Core and GLES 2/3 contexts */
      if (new_state & (_NEW_TEXTURE_OBJECT | _NEW_PROGRAM))
         _mesa_update_texture_state(ctx);

      if (new_state & _NEW_PROGRAM)
         update_program(ctx);
   }

 out:
   new_prog_state |= update_program_constants(ctx);

   ctx->NewState |= new_prog_state;

   /*
    * Give the driver a chance to act upon the new_state flags.
    * The driver might plug in different span functions, for example.
    * Also, this is where the driver can invalidate the state of any
    * active modules (such as swrast_setup, swrast, tnl, etc).
    */
   st_invalidate_state(ctx);
   ctx->NewState = 0;
}


/* This is the usual entrypoint for state updates:
 */
void
_mesa_update_state( struct Ruda_Context *ctx )
{
   _mesa_lock_context_textures(ctx);
   _mesa_update_state_locked(ctx);
   _mesa_unlock_context_textures(ctx);
}
