#include "Ruda/ruda.h"



/**
 * Update state related to the draw/read framebuffers.
 */
void
_mesa_update_framebuffer(struct Ruda_Context *ctx,
                         struct Ruda_Framebuffer *readFb,
                         struct Ruda_Framebuffer *drawFb)
{
   assert(ctx);

   update_framebuffer(ctx, drawFb);
   if (readFb != drawFb)
      update_framebuffer(ctx, readFb);

   _mesa_update_clamp_vertex_color(ctx, drawFb);
   _mesa_update_clamp_fragment_color(ctx, drawFb);
}
