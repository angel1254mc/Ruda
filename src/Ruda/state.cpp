#include "Ruda/ruda.h"
#include "program.cpp"

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
 * Update the ctx->*Program._Current pointers to point to the
 * current/active programs.
 *
 * Programs may come from 3 sources: GLSL shaders, ARB/NV_vertex/fragment
 * programs or programs derived from fixed-function state.
 *
 * This function needs to be called after texture state validation in case
 * we're generating a fragment program from fixed-function texture state.
 *
 * \return bitfield which will indicate _NEW_PROGRAM state if a new vertex
 * or fragment program is being used.
 */

/**
 * Return a fragment program which implements the current
 * fixed-function texture, fog and color-sum operations.
 */
struct gl_program *
_mesa_get_fixed_func_fragment_program(struct gl_context *ctx)
{
   struct gl_program *prog;
   struct state_key key;
   GLuint keySize;

   keySize = make_state_key(ctx, &key);

   prog = (struct gl_program *)
      _mesa_search_program_cache(ctx->FragmentProgram.Cache,
                                 &key, keySize);

   if (!prog) {
      prog = ctx->Driver.NewProgram(ctx, MESA_SHADER_FRAGMENT, 0, false);
      if (!prog)
         return NULL;

      const struct nir_shader_compiler_options *options =
         st_get_nir_compiler_options(ctx->st, MESA_SHADER_FRAGMENT);

      nir_shader *s =
         create_new_program(&key, prog, options);

      prog->state.type = PIPE_SHADER_IR_NIR;
      prog->nir = s;

      prog->SamplersUsed = s->info.samplers_used[0];

      /* default mapping from samplers to texture units */
      for (unsigned i = 0; i < MAX_SAMPLERS; i++)
         prog->SamplerUnits[i] = i;

      st_program_string_notify(ctx, GL_FRAGMENT_PROGRAM_ARB, prog);

      _mesa_program_cache_insert(ctx, ctx->FragmentProgram.Cache,
                                 &key, keySize, prog);
   }

   return prog;
}


static uint
update_program(struct Ruda_Context *ctx)
{
   struct Ruda_Program *vsProg =
      ctx->_Shader->CurrentProgram[MESA_SHADER_VERTEX];
   struct Ruda_Program *tcsProg =
      ctx->_Shader->CurrentProgram[MESA_SHADER_TESS_CTRL];
   struct Ruda_Program *tesProg =
      ctx->_Shader->CurrentProgram[MESA_SHADER_TESS_EVAL];
   struct Ruda_Program *gsProg =
      ctx->_Shader->CurrentProgram[MESA_SHADER_GEOMETRY];
   struct Ruda_Program *fsProg =
      ctx->_Shader->CurrentProgram[MESA_SHADER_FRAGMENT];
   struct Ruda_Program *csProg =
      ctx->_Shader->CurrentProgram[MESA_SHADER_COMPUTE];
   const struct Ruda_Program *prevVP = ctx->VertexProgram._Current;
   const struct Ruda_Program *prevFP = ctx->FragmentProgram._Current;
   const struct Ruda_Program *prevGP = ctx->GeometryProgram._Current;
   const struct Ruda_Program *prevTCP = ctx->TessCtrlProgram._Current;
   const struct Ruda_Program *prevTEP = ctx->TessEvalProgram._Current;
   const struct Ruda_Program *prevCP = ctx->ComputeProgram._Current;
   uint64_t prev_vp_affected_states = prevVP ? prevVP->affected_states : 0;
   uint64_t prev_tcp_affected_states = prevTCP ? prevTCP->affected_states : 0;
   uint64_t prev_tep_affected_states = prevTEP ? prevTEP->affected_states : 0;
   uint64_t prev_gp_affected_states = prevGP ? prevGP->affected_states : 0;
   uint64_t prev_fp_affected_states = prevFP ? prevFP->affected_states : 0;
   uint64_t prev_cp_affected_states = prevCP ? prevCP->affected_states : 0;

   /*
    * Set the ctx->VertexProgram._Current and ctx->FragmentProgram._Current
    * pointers to the programs that should be used for rendering.  If either
    * is NULL, use fixed-function code paths.
    *
    * These programs may come from several sources.  The priority is as
    * follows:
    *   1. OpenGL 2.0/ARB vertex/fragment shaders
    *   2. ARB/NV vertex/fragment programs
    *   3. ATI fragment shader
    *   4. Programs derived from fixed-function state.
    *
    * Note: it's possible for a vertex shader to get used with a fragment
    * program (and vice versa) here, but in practice that shouldn't ever
    * come up, or matter.
    */

   if (fsProg) {
      /* Use GLSL fragment shader */
      _mesa_reference_program(ctx, &ctx->FragmentProgram._Current, fsProg);
      _mesa_reference_program(ctx, &ctx->FragmentProgram._TexEnvProgram,
                              NULL);
   // } else if (_mesa_arb_fragment_program_enabled(ctx)) {
   //    /* Use user-defined fragment program */
   //    _mesa_reference_program(ctx, &ctx->FragmentProgram._Current,
   //                            ctx->FragmentProgram.Current);
   //    _mesa_reference_program(ctx, &ctx->FragmentProgram._TexEnvProgram,
	// 		      NULL);
   // } else if (_mesa_ati_fragment_shader_enabled(ctx) &&
   //          ctx->ATIFragmentShader.Current->Program) {
   //     /* Use the enabled ATI fragment shader's associated program */
   //    _mesa_reference_program(ctx, &ctx->FragmentProgram._Current,
   //                            ctx->ATIFragmentShader.Current->Program);
   //    _mesa_reference_program(ctx, &ctx->FragmentProgram._TexEnvProgram,
   //                            NULL);
   } else {
      /* Use fragment program generated from fixed-function state */
      _mesa_reference_program(ctx, &ctx->FragmentProgram._Current,
                              _mesa_get_fixed_func_fragment_program(ctx));
      _mesa_reference_program(ctx, &ctx->FragmentProgram._TexEnvProgram,
                              ctx->FragmentProgram._Current);
   }

   /* Examine vertex program after fragment program as
    * _mesa_get_fixed_func_vertex_program() needs to know active
    * fragprog inputs.
    */
   if (vsProg) {
      /* Use GLSL vertex shader */
      assert(VP_MODE_SHADER == ctx->VertexProgram._VPMode);
      _mesa_reference_program(ctx, &ctx->VertexProgram._Current, vsProg);
   // } else if (_mesa_arb_vertex_program_enabled(ctx)) {
   //    /* Use user-defined vertex program */
   //    assert(VP_MODE_SHADER == ctx->VertexProgram._VPMode);
   //    _mesa_reference_program(ctx, &ctx->VertexProgram._Current,
   //                            ctx->VertexProgram.Current);
   } else {
      /* Use vertex program generated from fixed-function state */
      assert(VP_MODE_FF == ctx->VertexProgram._VPMode);
      _mesa_reference_program(ctx, &ctx->VertexProgram._Current,
                              _mesa_get_fixed_func_vertex_program(ctx));
      _mesa_reference_program(ctx, &ctx->VertexProgram._TnlProgram,
                              ctx->VertexProgram._Current);
   }

   /* Bind or unbind these shaders. (NULL = unbind) */
   _mesa_reference_program(ctx, &ctx->GeometryProgram._Current, gsProg);
   _mesa_reference_program(ctx, &ctx->TessEvalProgram._Current, tesProg);
   _mesa_reference_program(ctx, &ctx->TessCtrlProgram._Current, tcsProg);
   _mesa_reference_program(ctx, &ctx->ComputeProgram._Current, csProg);

   bool vp_changed = ctx->VertexProgram._Current != prevVP;
   bool tcp_changed = ctx->TessCtrlProgram._Current != prevTCP;
   bool tep_changed = ctx->TessEvalProgram._Current != prevTEP;
   bool gp_changed = ctx->GeometryProgram._Current != prevGP;
   bool fp_changed = ctx->FragmentProgram._Current != prevFP;
   bool cp_changed = ctx->ComputeProgram._Current != prevCP;

   /* Set NewDriverState depending on which shaders have changed. */
   uint64_t dirty = 0;

   /* Flag states used by both new and old shaders to rebind shader resources
    * (because shaders pack them and reorder them) and to unbind shader
    * resources properly when transitioning to shaders that don't use them.
    */
   if (vp_changed) {
      ctx->Array.NewVertexElements = true;
      dirty |= prev_vp_affected_states;
      if (ctx->VertexProgram._Current)
         dirty |= ST_NEW_VERTEX_PROGRAM(ctx, ctx->VertexProgram._Current);
   }

   if (tcp_changed) {
      dirty |= prev_tcp_affected_states;
      if (ctx->TessCtrlProgram._Current)
         dirty |= ctx->TessCtrlProgram._Current->affected_states;
   }

   if (tep_changed) {
      dirty |= prev_tep_affected_states;
      if (ctx->TessEvalProgram._Current)
         dirty |= ctx->TessEvalProgram._Current->affected_states;
   }

   if (gp_changed) {
      dirty |= prev_gp_affected_states;
      if (ctx->GeometryProgram._Current)
         dirty |= ctx->GeometryProgram._Current->affected_states;
   }

   if (fp_changed) {
      dirty |= prev_fp_affected_states;
      if (ctx->FragmentProgram._Current)
         dirty |= ctx->FragmentProgram._Current->affected_states;

      if (!ctx->st->needs_texcoord_semantic)
         dirty |= ST_NEW_RASTERIZER;
   }

   if (cp_changed) {
      dirty |= prev_cp_affected_states;
      if (ctx->ComputeProgram._Current)
         dirty |= ctx->ComputeProgram._Current->affected_states;
   }

   struct Ruda_Program *last_vertex_stage;
   bool last_vertex_stage_dirty;

   if (ctx->GeometryProgram._Current) {
      last_vertex_stage = ctx->GeometryProgram._Current;
      last_vertex_stage_dirty = gp_changed;
   } else if (ctx->TessEvalProgram._Current) {
      last_vertex_stage = ctx->TessEvalProgram._Current;
      last_vertex_stage_dirty = gp_changed | tep_changed;
   } else {
      last_vertex_stage = ctx->VertexProgram._Current;
      last_vertex_stage_dirty = gp_changed | tep_changed | vp_changed;
   }

   /* Find out the number of viewports. This determines how many scissors
    * and viewport states we need to update.
    */
   struct st_context *st = ctx->st;
   unsigned num_viewports = 1;

   if (last_vertex_stage &&
       last_vertex_stage->info.outputs_written & (
             VARYING_BIT_VIEWPORT | VARYING_BIT_VIEWPORT_MASK))
      num_viewports = ctx->Const.MaxViewports;

   if (st->state.num_viewports != num_viewports) {
      st->state.num_viewports = num_viewports;
      dirty |= ST_NEW_VIEWPORT;

      if (ctx->Scissor.EnableFlags & u_bit_consecutive(0, num_viewports))
         dirty |= ST_NEW_SCISSOR;
   }

   if (st->lower_point_size && last_vertex_stage_dirty &&
       !ctx->VertexProgram.PointSizeEnabled && !ctx->PointSizeIsSet) {
      if (ctx->GeometryProgram._Current) {
         ctx->NewDriverState |= ST_NEW_GS_CONSTANTS;
      } else if (ctx->TessEvalProgram._Current) {
         ctx->NewDriverState |= ST_NEW_TES_CONSTANTS;
      } else {
         ctx->NewDriverState |= ST_NEW_VS_CONSTANTS;
      }
   }

   ctx->NewDriverState |= dirty;

   /* Let the driver know what's happening: */
   if (fp_changed || vp_changed || gp_changed || tep_changed ||
       tcp_changed || cp_changed) {
      /* This will mask out unused shader resources. */
      st->active_states = _mesa_get_active_states(ctx);

      /* Some drivers need to clean up previous states too */
      if (st->validate_all_dirty_states)
         st->active_states |= dirty;

      return _NEW_PROGRAM;
   }

   return 0;
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
   // if (_mesa_is_desktop_gl_compat(ctx) ||
   //     _mesa_is_gles1(ctx)) {
   //    /* Update derived state. */
   //    if (new_state & (_NEW_MODELVIEW|_NEW_PROJECTION))
   //       _mesa_update_modelview_project( ctx, new_state );

   //    if (new_state & _NEW_TEXTURE_MATRIX)
   //       new_state |= _mesa_update_texture_matrices(ctx);

   //    if (new_state & (_NEW_TEXTURE_OBJECT | _NEW_TEXTURE_STATE | _NEW_PROGRAM))
   //       new_state |= _mesa_update_texture_state(ctx);

   //    if (new_state & _NEW_LIGHT_CONSTANTS)
   //       new_state |= _mesa_update_lighting(ctx);

   //    /* ctx->_NeedEyeCoords is determined here.
   //     *
   //     * If the truth value of this variable has changed, update for the
   //     * new lighting space and recompute the positions of lights and the
   //     * normal transform.
   //     *
   //     * If the lighting space hasn't changed, may still need to recompute
   //     * light positions & normal transforms for other reasons.
   //     */
   //    if (new_state & (_NEW_TNL_SPACES | _NEW_LIGHT_CONSTANTS |
   //                     _NEW_MODELVIEW)) {
   //       if (_mesa_update_tnl_spaces(ctx, new_state))
   //          new_state |= _NEW_FF_VERT_PROGRAM;
   //    }

   //    if (new_state & _NEW_PROGRAM)
   //       update_fixed_func_program_usage(ctx);

   //    /* Determine which states affect fixed-func vertex/fragment program. */
   //    uint prog_flags = _NEW_PROGRAM;

   //    if (ctx->FragmentProgram._UsesTexEnvProgram) {
   //       prog_flags |= _NEW_BUFFERS | _NEW_TEXTURE_OBJECT |
   //                     _NEW_FF_FRAG_PROGRAM | _NEW_TEXTURE_STATE;
   //    }

   //    if (ctx->VertexProgram._UsesTnlProgram)
   //       prog_flags |= _NEW_FF_VERT_PROGRAM;

   //    if (new_state & prog_flags) {
   //       /* When we generate programs from fixed-function vertex/fragment state
   //        * this call may generate/bind a new program.  If so, we need to
   //        * propogate the _NEW_PROGRAM flag to the driver.
   //        */
   //       new_prog_state |= update_program(ctx);
   //    }
   // } else {

   // }

   // /* GL Core and GLES 2/3 contexts */
   // if (new_state & (_NEW_TEXTURE_OBJECT | _NEW_PROGRAM))
   //    _mesa_update_texture_state(ctx);
   if (new_state & _NEW_PROGRAM)
         update_program(ctx);

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

