#include "Ruda/ruda.h"
#include "../src/util/ralloc.h"
/**********************************************************************/
/*** Shader Program object functions                                ***/
/**********************************************************************/



/**
 * Sever all connections with all pieces of driver storage for all uniforms
 *
 * \warning
 * This function does \b not release any of the \c data pointers
 * previously passed in to \c _mesa_uniform_attach_driver_stoarge.
 */
void
_mesa_uniform_detach_all_driver_storage(struct gl_uniform_storage *uni)
{
   free(uni->driver_storage);
   uni->driver_storage = NULL;
   uni->num_driver_storage = 0;
}

void
_mesa_reference_shader_program_data(struct Ruda_Shader_Program_Data **ptr,
                                    struct Ruda_Shader_Program_Data *data)
{
   if (*ptr == data)
      return;

   if (*ptr) {
      struct Ruda_Shader_Program_Data *oldData = *ptr;

      assert(oldData->RefCount > 0);

      if (p_atomic_dec_zero(&oldData->RefCount)) {
         assert(oldData->NumUniformStorage == 0 ||
                oldData->UniformStorage);

         for (unsigned i = 0; i < oldData->NumUniformStorage; ++i)
            _mesa_uniform_detach_all_driver_storage(&oldData->UniformStorage[i]);

         ralloc_free(oldData);
      }

      *ptr = NULL;
   }

   if (data)
      p_atomic_inc(&data->RefCount);

   *ptr = data;
}

