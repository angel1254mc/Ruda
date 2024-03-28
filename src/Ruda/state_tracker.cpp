#include "Ruda/ruda.h"
#include "Ruda/structs.h"
#include "program.h"

#define BITMAP_CACHE_WIDTH  512
#define BITMAP_CACHE_HEIGHT 32


static void
reset_cache(struct st_context *st)
{
   struct st_bitmap_cache *cache = &st->bitmap.cache;

   /*memset(cache->buffer, 0xff, sizeof(cache->buffer));*/
   cache->empty = GL_TRUE;

   cache->xmin = 1000000;
   cache->xmax = -1000000;
   cache->ymin = 1000000;
   cache->ymax = -1000000;

   _mesa_reference_program(st->ctx, &cache->fp, NULL);

   assert(!cache->texture);

   /* allocate a new texture */
   /**cache->texture = st_texture_create(st, st->internal_target,
                                      st->bitmap.tex_format, 0,
                                      BITMAP_CACHE_WIDTH, BITMAP_CACHE_HEIGHT,
                                      1, 1, 0,
                                      PIPE_BIND_SAMPLER_VIEW,
                                      false);*/
}


/**
 * If there's anything in the bitmap cache, draw/flush it now.
 */
void
st_flush_bitmap_cache(struct st_context *st)
{
   struct st_bitmap_cache *cache = &st->bitmap.cache;

   if (!cache->empty) {
      struct pipe_context *pipe = st->pipe;
      struct pipe_sampler_view *sv;

      assert(cache->xmin <= cache->xmax);

      if (0)
         printf("flush bitmap, size %d x %d  at %d, %d\n",
                cache->xmax - cache->xmin,
                cache->ymax - cache->ymin,
                cache->xpos, cache->ypos);

      /* The texture transfer has been mapped until now.
       * So unmap and release the texture transfer before drawing.
       */
      if (cache->trans && cache->buffer) {
         //print_cache(cache);
         pipe_texture_unmap(pipe, cache->trans);
         cache->buffer = NULL;
         cache->trans = NULL;
      }

      //sv = st_create_texture_sampler_view(st->pipe, cache->texture);
      /**if (sv) {
         draw_bitmap_quad(st->ctx,
                          cache->xpos,
                          cache->ypos,
                          cache->zpos,
                          BITMAP_CACHE_WIDTH, BITMAP_CACHE_HEIGHT,
                          sv,
                          cache->color,
                          cache->fp,
                          cache->scissor_enabled,
                          cache->clamp_frag_color);
      }*/

      /* release/free the texture */
      pipe_resource_reference(&cache->texture, NULL);

      reset_cache(st);
   }
}

/**
 * Free all basic program variants.
 */
void
st_release_variants(struct st_context *st, struct Ruda_Program *p)
{
   struct st_variant *v;

   /* If we are releasing shaders, re-bind them, because we don't
    * know which shaders are bound in the driver.
    */
   //if (p->variants)
      //st_unbind_program(st, p);

   //for (v = p->variants; v; ) {
   //   struct st_variant *next = v->next;
   //   delete_variant(st, v, p->Target);
   //   v = next;
   //}

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




void
st_Clear(struct Ruda_Context *ctx, unsigned int mask)
{
   struct st_context *st = st_context(ctx);
   struct Ruda_Renderbuffer *depthRb
      = ctx->DrawBuffer->Attachment[BUFFER_DEPTH].Renderbuffer;
   struct Ruda_Renderbuffer *stencilRb
      = ctx->DrawBuffer->Attachment[BUFFER_STENCIL].Renderbuffer;
   unsigned int quad_buffers = 0x0;
   unsigned int clear_buffers = 0x0;
   bool have_scissor_buffers = false;
   GLuint i;

   st_flush_bitmap_cache(st);
   //st_invalidate_readpix_cache(st);

   /* This makes sure the pipe has the latest scissor, etc values */
   //st_validate_state(st, ST_PIPELINE_CLEAR_STATE_MASK);

   if (mask & BUFFER_BITS_COLOR) {
      for (i = 0; i < ctx->DrawBuffer->_NumColorDrawBuffers; i++) {
         Ruda_Buffer_Index b = ctx->DrawBuffer->_ColorDrawBufferIndexes[i];

         if (b != BUFFER_NONE && mask & (1 << b)) {
            struct Ruda_Renderbuffer *rb
               = ctx->DrawBuffer->Attachment[b].Renderbuffer;
            int colormask_index = ctx->Extensions.EXT_draw_buffers2 ? i : 0;

            if (!rb || !rb->surface)
               continue;

            unsigned colormask =
               GET_COLORMASK(ctx->Color.ColorMask, colormask_index);

            if (!colormask)
               continue;

            unsigned surf_colormask =
               util_format_colormask(util_format_description(rb->surface->format));

            bool scissor = is_scissor_enabled(ctx, rb);
            if ((scissor && !st->can_scissor_clear) ||
                is_window_rectangle_enabled(ctx) ||
                ((colormask & surf_colormask) != surf_colormask))
               quad_buffers |= PIPE_CLEAR_COLOR0 << i;
            else
               clear_buffers |= PIPE_CLEAR_COLOR0 << i;
            have_scissor_buffers |= scissor && st->can_scissor_clear;
         }
      }
   }

   if (mask & BUFFER_BIT_DEPTH) {
      if (depthRb->surface && ctx->Depth.Mask) {
         bool scissor = is_scissor_enabled(ctx, depthRb);
         if ((scissor && !st->can_scissor_clear) ||
             is_window_rectangle_enabled(ctx))
            quad_buffers |= PIPE_CLEAR_DEPTH;
         else
            clear_buffers |= PIPE_CLEAR_DEPTH;
         have_scissor_buffers |= scissor && st->can_scissor_clear;
      }
   }
   if (mask & BUFFER_BIT_STENCIL) {
      if (stencilRb->surface && !is_stencil_disabled(ctx, stencilRb)) {
         bool scissor = is_scissor_enabled(ctx, stencilRb);
         if ((scissor && !st->can_scissor_clear) ||
             is_window_rectangle_enabled(ctx) ||
             is_stencil_masked(ctx, stencilRb))
            quad_buffers |= PIPE_CLEAR_STENCIL;
         else
            clear_buffers |= PIPE_CLEAR_STENCIL;
         have_scissor_buffers |= scissor && st->can_scissor_clear;
      }
   }

   /* Always clear depth and stencil together.
    * This can only happen when the stencil writemask is not a full mask.
    */
   if (quad_buffers & PIPE_CLEAR_DEPTHSTENCIL &&
       clear_buffers & PIPE_CLEAR_DEPTHSTENCIL) {
      quad_buffers |= clear_buffers & PIPE_CLEAR_DEPTHSTENCIL;
      clear_buffers &= ~PIPE_CLEAR_DEPTHSTENCIL;
   }

   /* Only use quad-based clearing for the renderbuffers which cannot
    * use pipe->clear. We want to always use pipe->clear for the other
    * renderbuffers, because it's likely to be faster.
    */
   if (clear_buffers) {
      const struct gl_scissor_rect *scissor = &ctx->Scissor.ScissorArray[0];
      struct pipe_scissor_state scissor_state = {
         .minx = MAX2(scissor->X, 0),
         .miny = MAX2(scissor->Y, 0),
         .maxx = MAX2(scissor->X + scissor->Width, 0),
         .maxy = MAX2(scissor->Y + scissor->Height, 0),

      };

      /* Now invert Y if needed.
       * Gallium drivers use the convention Y=0=top for surfaces.
       */
      if (st->state.fb_orientation == Y_0_TOP) {
         const struct gl_framebuffer *fb = ctx->DrawBuffer;
         /* use intermediate variables to avoid uint underflow */
         GLint miny, maxy;
         miny = fb->Height - scissor_state.maxy;
         maxy = fb->Height - scissor_state.miny;
         scissor_state.miny = MAX2(miny, 0);
         scissor_state.maxy = MAX2(maxy, 0);
      }
      if (have_scissor_buffers) {
         const struct gl_framebuffer *fb = ctx->DrawBuffer;
         scissor_state.maxx = MIN2(scissor_state.maxx, fb->Width);
         scissor_state.maxy = MIN2(scissor_state.maxy, fb->Height);
         if (scissor_state.minx >= scissor_state.maxx ||
             scissor_state.miny >= scissor_state.maxy)
            return;
      }
      /* We can't translate the clear color to the colorbuffer format,
       * because different colorbuffers may have different formats.
       */
      st->pipe->clear(st->pipe, clear_buffers, have_scissor_buffers ? &scissor_state : NULL,
                      (union pipe_color_union*)&ctx->Color.ClearColor,
                      ctx->Depth.Clear, ctx->Stencil.Clear);
   }
   if (quad_buffers) {
      clear_with_quad(ctx, quad_buffers);
   }
   if (mask & BUFFER_BIT_ACCUM)
      _mesa_clear_accum_buffer(ctx);
}

