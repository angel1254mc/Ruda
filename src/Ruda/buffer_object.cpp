#include "Ruda/ruda.h"
#include "RudaDI/di_structs.h"
#include "hash.h"
#include "../../src/util/simple_mtx.h"
#include "../../src/util/hash_table.h"
#include "context.h"

#include "../../src/gallium/auxiliary/util/u_inlines.h"


/**
 * Used as a placeholder for buffer objects between glGenBuffers() and
 * glBindBuffer() so that glIsBuffer() can work correctly.
 */
static struct Ruda_Buffer_Object DummyBufferObject = {
    .RefCount = 1000*1000*1000,  /* never delete */
    .MinMaxCacheMutex = SIMPLE_MTX_INITIALIZER
};


void rudaGenBuffer(uint* buffers) {
    rudaCreateBuffer(structure->ctx, buffers);
}

void rudaCreateBuffer(struct Ruda_Context *ctx, uint *buffers)
{
   struct Ruda_Buffer_Object *buf;

   if (!buffers)
      return;

   /*
    * This must be atomic (generation and allocation of buffer object IDs)
    */
   _mesa_HashLockMaybeLocked(ctx->Shared->BufferObjects,
                             ctx->BufferObjectsLocked);
   /* If one context only creates buffers and another context only deletes
    * buffers, buffers don't get released because it only produces zombie
    * buffers. Only the context that has created the buffers can release
    * them. Thus, when we create buffers, we prune the list of zombie
    * buffers.
    */
   //unreference_zombie_buffers_for_ctx(ctx);

   _mesa_HashFindFreeKeys(ctx->Shared->BufferObjects, buffers, 1);

   /* Insert the ID and pointer into the hash table. If non-DSA, insert a
    * DummyBufferObject.  Otherwise, create a new buffer object and insert
    * it.
    */
    buf = &DummyBufferObject;
    _mesa_HashInsertLocked(ctx->Shared->BufferObjects, buffers[0], buf, true);

   _mesa_HashUnlockMaybeLocked(ctx->Shared->BufferObjects,
                               ctx->BufferObjectsLocked);
}

bool
_mesa_bufferobj_unmap(struct Ruda_Context *ctx, struct Ruda_Buffer_Object *obj,
                      int index)
{

    if (obj->Mappings[index].Length)
        pipe_buffer_unmap(ctx->pipe, obj->transfer[index]);

   obj->transfer[index] = NULL;
   obj->Mappings[index].Pointer = NULL;
   obj->Mappings[index].Offset = 0;
   obj->Mappings[index].Length = 0;
   return true;
}




/** Is the given buffer object currently mapped by the GL user? */
static inline bool
_mesa_bufferobj_mapped(const struct Ruda_Buffer_Object *obj,
                       int index)
{
   return obj->Mappings[index].Pointer != NULL;
}


void
_mesa_buffer_unmap_all_mappings(struct Ruda_Context *ctx,
                                struct Ruda_Buffer_Object *bufObj)
{
   for (int i = 0; i < MAP_COUNT; i++) {
      if (_mesa_bufferobj_mapped(bufObj, i)) {
         _mesa_bufferobj_unmap(ctx, bufObj, i);
         assert(bufObj->Mappings[i].Pointer == NULL);
         bufObj->Mappings[i].AccessFlags = 0;
      }
   }
}


void
_mesa_bufferobj_release_buffer(struct Ruda_Buffer_Object *obj)
{
   if (!obj->buffer)
      return;

   /* Subtract the remaining private references before unreferencing
    * the buffer. See the header file for explanation.
    */
   if (obj->private_refcount) {
      assert(obj->private_refcount > 0);
      p_atomic_add(&obj->buffer->reference.count,
                   -obj->private_refcount);
      obj->private_refcount = 0;
   }
   obj->private_refcount_ctx = NULL;

   pipe_resource_reference(&obj->buffer, NULL);
}

static void
vbo_minmax_cache_delete_entry(struct hash_entry* entry)
{
   free(entry->data);
}


void
vbo_delete_minmax_cache(struct Ruda_Buffer_Object *bufferObj)
{
   _mesa_hash_table_destroy(bufferObj->MinMaxCache, vbo_minmax_cache_delete_entry);
   bufferObj->MinMaxCache = NULL;
}

/**
 * Delete a buffer object.
 *
 * Default callback for the \c dd_function_table::DeleteBuffer() hook.
 */
void
_mesa_delete_buffer_object(struct Ruda_Context *ctx,
                           struct Ruda_Buffer_Object *bufObj)
{
   assert(bufObj->RefCount == 0);
   _mesa_buffer_unmap_all_mappings(ctx, bufObj);
   _mesa_bufferobj_release_buffer(bufObj);

   vbo_delete_minmax_cache(bufObj);

   /* assign strange values here to help w/ debugging */
   bufObj->RefCount = -1000;
   bufObj->Name = ~0;

    simple_mtx_destroy(&bufObj->MinMaxCacheMutex);
   free(bufObj->Label);
   free(bufObj);
}

static inline void
_mesa_reference_buffer_object(struct Ruda_Context *ctx,
                               struct Ruda_Buffer_Object **ptr,
                               struct Ruda_Buffer_Object *bufObj,
                               bool shared_binding)
{
   if (*ptr) {
      /* Unreference the old buffer */
      struct Ruda_Buffer_Object *oldObj = *ptr;

      assert(oldObj->RefCount >= 1);

      /* Count references only if the context doesn't own the buffer or if
       * ptr is a binding point shared by multiple contexts (such as a texture
       * buffer object being a buffer bound within a texture object).
       */
      if (shared_binding || ctx != oldObj->Ctx) {
         if (p_atomic_dec_zero(&oldObj->RefCount)) {
            _mesa_delete_buffer_object(ctx, oldObj);
         }
      } else {
         /* Update the private ref count. */
         assert(oldObj->CtxRefCount >= 1);
         oldObj->CtxRefCount--;
      }
   }

   if (bufObj) {
      /* reference new buffer */
      if (shared_binding || ctx != bufObj->Ctx)
         p_atomic_inc(&bufObj->RefCount);
      else
         bufObj->CtxRefCount++;
   }

   *ptr = bufObj;
}

struct Ruda_Buffer_Object *
_mesa_lookup_bufferobj(struct Ruda_Context *ctx, uint buffer)
{
   if (buffer == 0)
      return NULL;
   else
      return (struct Ruda_Buffer_Object *)
         _mesa_HashLookupMaybeLocked(ctx->Shared->BufferObjects, buffer,
                                     ctx->BufferObjectsLocked);
}



void bind_buffer_object(struct Ruda_Context *ctx, uint buffer)
{
   struct Ruda_Buffer_Object **bindTarget = &ctx->Array.ArrayBufferObj;
   struct Ruda_Buffer_Object *oldBufObj;
   struct Ruda_Buffer_Object *newBufObj;

   assert(bindTarget);

   /* Fast path that unbinds. It's better when NULL is a literal, so that
    * the compiler can simplify this code after inlining.
    */
   if (buffer == 0) {
      _mesa_reference_buffer_object(ctx, bindTarget, NULL, false);
      return;
   }

   /* Get pointer to old buffer object (to be unbound) */
   oldBufObj = *bindTarget;
   uint old_name = oldBufObj && !oldBufObj->DeletePending ? oldBufObj->Name : 0;
   if (unlikely(old_name == buffer))
      return;   /* rebinding the same buffer object- no change */

   newBufObj = _mesa_lookup_bufferobj(ctx, buffer);
   /* Get a new buffer object if it hasn't been created. */
//    if (unlikely(!handle_bind_buffer_gen(ctx, buffer, &newBufObj, "glBindBuffer"))
//       return;

   /* At this point, the compiler should deduce that newBufObj is non-NULL if
    * everything has been inlined, so the compiler should simplify this.
    */
   _mesa_reference_buffer_object(ctx, bindTarget, newBufObj, false);
}

static ALWAYS_INLINE bool
bufferobj_data(struct Ruda_Context *ctx,
               int size,
               const void *data,
               uint64_t offset,
               int usage,
               unsigned int storageFlags,
               struct Ruda_Buffer_Object *obj)
{
   struct pipe_context *pipe = ctx->pipe;
   struct pipe_screen *screen = pipe->screen;
   bool is_mapped = _mesa_bufferobj_mapped(obj, MAP_USER);

   if (size > UINT32_MAX || offset > UINT32_MAX) {
      /* pipe_resource.width0 is 32 bits only and increasing it
       * to 64 bits doesn't make much sense since hw support
       * for > 4GB resources is limited.
       */
      obj->Size = 0;
      return false;
   }

   if (size && obj->buffer &&
       obj->Size == size &&
       obj->Usage == usage &&
       obj->StorageFlags == storageFlags) {
      if (data) {
         /* Just discard the old contents and write new data.
          * This should be the same as creating a new buffer, but we avoid
          * a lot of validation in Mesa.
          *
          * If the buffer is mapped, we can't discard it.
          *
          * PIPE_MAP_DIRECTLY supresses implicit buffer range
          * invalidation.
          */
         pipe->buffer_subdata(pipe, obj->buffer,
                              is_mapped ? PIPE_MAP_DIRECTLY :
                                          PIPE_MAP_DISCARD_WHOLE_RESOURCE,
                              0, size, data);
         return true;
      } else if (is_mapped) {
         return true; /* can't reallocate, nothing to do */
      } else if (screen->get_param(screen, PIPE_CAP_INVALIDATE_BUFFER)) {
         pipe->invalidate_resource(pipe, obj->buffer);
         return true;
      }
   }

    obj->Size = size;
    obj->Usage = usage;
    obj->StorageFlags = storageFlags;

    _mesa_bufferobj_release_buffer(obj);

    unsigned bindings = buffer_target_to_bind_flags(target);

//    if (ST_DEBUG & DEBUG_BUFFER) {
//       debug_printf("Create buffer size %" PRId64 " bind 0x%x\n",
//                    (int64_t) size, bindings);
//    }

    if (size != 0) {
        struct pipe_resource buffer;
        memset(&buffer, 0, sizeof buffer);
        buffer.target = PIPE_BUFFER;
        buffer.format = PIPE_FORMAT_R8_UNORM; /* want TYPELESS or similar */
        buffer.bind = bindings;
        buffer.usage = buffer_usage(target, obj->Immutable, storageFlags, usage);
        buffer.flags = storage_flags_to_buffer_flags(storageFlags);
        buffer.width0 = size;
        buffer.height0 = 1;
        buffer.depth0 = 1;
        buffer.array_size = 1;
        
        obj->buffer = screen->resource_create(screen, &buffer);

         if (obj->buffer && data) pipe_buffer_write(pipe, obj->buffer, 0, size, data);

      if (!obj->buffer) {
         /* out of memory */
         obj->Size = 0;
         return false;
      }

      obj->private_refcount_ctx = ctx;
   }

   /* The current buffer may be bound, so we have to revalidate all atoms that
    * might be using it.
    */
   if (obj->UsageHistory & USAGE_ARRAY_BUFFER)
      ctx->NewDriverState |= ST_NEW_VERTEX_ARRAYS;
   if (obj->UsageHistory & USAGE_UNIFORM_BUFFER)
      ctx->NewDriverState |= ST_NEW_UNIFORM_BUFFER;
   if (obj->UsageHistory & USAGE_SHADER_STORAGE_BUFFER)
      ctx->NewDriverState |= ST_NEW_STORAGE_BUFFER;
   if (obj->UsageHistory & USAGE_TEXTURE_BUFFER)
      ctx->NewDriverState |= ST_NEW_SAMPLER_VIEWS | ST_NEW_IMAGE_UNITS;
   if (obj->UsageHistory & USAGE_ATOMIC_COUNTER_BUFFER)
      ctx->NewDriverState |= ctx->DriverFlags.NewAtomicBuffer;

   return true;
}


















bool
_mesa_bufferobj_data(struct Ruda_Context *ctx,
                  int size,
                  const void *data,
                  int usage,
                  unsigned int storageFlags,
                  struct Ruda_Buffer_Object *obj)
{
   return bufferobj_data(ctx, size, data, 0, usage, storageFlags, obj);
}


// We assume this is an array, so no need to select a target   a
void rudaBufferArrayData(int size, const void *data, int usage) {
   // Our fake "get_buffer" logic
   Ruda_Context* ctx = structure->ctx;
   struct Ruda_Buffer_Object* bufObj = ctx->Array.ArrayBufferObj;


   if (!bufObj) {
      std::cout << "rudaBufferArrayData: No Buffer Data found!" << std::endl;
      return;
   };
   
   if (size < 0) {
      std::cout << "rudaBufferArrayData: Invalid Value for Size!" << std::endl;
      return;
   }
   // Assume usage is valid
   // Check buffer object for immutability
   if (bufObj->Immutable || bufObj->HandleAllocated) {
      std::cout << "rudaBufferArrayData: Can't modify immutable buffer!" << std::endl;
      return;
   }
   
   // Then, unmap all mappings
    _mesa_buffer_unmap_all_mappings(ctx, bufObj);
    FLUSH_VERTICES(ctx, 0, 0);
    bufObj->MinMaxCacheDirty = true;


    _mesa_bufferobj_data(ctx, size, data, usage, MAP_READ_BIT | MAP_WRITE_BIT | DYNAMIC_STORAGE_BIT, bufObj);
 };




 // START ENABLE VERTEX ATTRIBUTE DATA //
   
void rudaEnableVertexAttribArray(int index) {
   Ruda_Context* ctx = structure->ctx;
   Ruda_Vertex_Array_Object* vao = ctx->Array.VAO;
   // First check that index is valid
   if (index >= ctx->Const.Program["RUDA_SHADER_VERTEX"].MaxAttribs) {
         std::cout << "Error: invalid index at rudaEnableVertexAttribArray"
         return;
   }
}
 // END ENABLE VERTEX ATTRIBUTE DATA //