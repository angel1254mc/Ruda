#pragma once
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <threads.h>
#include "include/Ruda/Util/u_idalloc.h"
// Abstracts the Graphics Context struct from X11 to XGC to avoid ambiguity
#define XGC GC
#define XC XContext

typedef enum
{
   TEXTURE_2D_MULTISAMPLE_INDEX,
   TEXTURE_2D_MULTISAMPLE_ARRAY_INDEX,
   TEXTURE_CUBE_ARRAY_INDEX,
   TEXTURE_BUFFER_INDEX,
   TEXTURE_2D_ARRAY_INDEX,
   TEXTURE_1D_ARRAY_INDEX,
   TEXTURE_EXTERNAL_INDEX,
   TEXTURE_CUBE_INDEX,
   TEXTURE_3D_INDEX,
   TEXTURE_RECT_INDEX,
   TEXTURE_2D_INDEX,
   TEXTURE_1D_INDEX,
   NUM_TEXTURE_TARGETS
} Ruda_Texture_Index;


typedef enum
{
   MAP_USER,
   MAP_INTERNAL,
   MAP_GLTHREAD,
   MAP_COUNT
} Ruda_Map_Buffer_Index;


struct Ruda_Vertex_Array_Object
{
	/** The index buffer (also known as the element array buffer in OpenGL). */
	struct Ruda_Buffer_Object *IndexBufferObj;
};



/**
 * Vertex array state
 */
struct Ruda_Array_Attrib
{
   /** Currently bound array object. */
   struct gl_vertex_array_object *VAO;
   
	/* GL_ARB_vertex_buffer_object */
   struct Ruda_Buffer_Object *ArrayBufferObj;

};




struct Ruda_Shared_State
{
   simple_mtx_t Mutex;		   /**< for thread safety */
   int RefCount;			   /**< Reference count */
   bool DisplayListsAffectGLThread;

   struct _mesa_HashTable *DisplayList;	   /**< Display lists hash table */
   struct _mesa_HashTable *TexObjects;	   /**< Texture objects hash table */

   /** Default texture objects (shared by all texture units) */
   //struct gl_texture_object *DefaultTex[NUM_TEXTURE_TARGETS];

   /** Fallback texture used when a bound texture is incomplete */
   //struct gl_texture_object *FallbackTex[NUM_TEXTURE_TARGETS][2]; /**< [color, depth] */

   /**
    * \name Thread safety and statechange notification for texture
    * objects.
    *
    * \todo Improve the granularity of locking.
    */
   /*@{*/
   simple_mtx_t TexMutex;		/**< texobj thread safety */
   uint TextureStateStamp;	        /**< state notification for shared tex */
   /*@}*/

   /**
    * \name Vertex/geometry/fragment programs
    */
   /*@{*/
   struct _mesa_HashTable *Programs; /**< All vertex/fragment programs */
   //struct gl_program *DefaultVertexProgram;
   //struct gl_program *DefaultFragmentProgram;
   /*@}*/

   /* GL_ATI_fragment_shader */
   //struct _mesa_HashTable *ATIShaders;
   //struct ati_fragment_shader *DefaultFragmentShader;

   struct _mesa_HashTable *BufferObjects;

   /* Buffer objects released by a different context than the one that
    * created them. Since the creating context holds one global buffer
    * reference for each buffer it created and skips reference counting,
    * deleting a buffer by another context can't touch the buffer reference
    * held by the context that created it. Only the creating context can
    * remove its global buffer reference.
    *
    * This list contains all buffers that were deleted by a different context
    * than the one that created them. This list should be probed by all
    * contexts regularly and remove references of those buffers that they own.
    */
   //struct set *ZombieBufferObjects;

   /** Table of both gl_shader and gl_shader_program objects */
   struct _mesa_HashTable *ShaderObjects;

   /* GL_EXT_framebuffer_object */
   struct _mesa_HashTable *RenderBuffers;
   struct _mesa_HashTable *FrameBuffers;

   /* GL_ARB_sync */
   struct set *SyncObjects;

   /** GL_ARB_sampler_objects */
   struct _mesa_HashTable *SamplerObjects;

   /* GL_ARB_bindless_texture */
   struct hash_table_u64 *TextureHandles;
   struct hash_table_u64 *ImageHandles;
   mtx_t HandlesMutex; /**< For texture/image handles safety */

   /* GL_ARB_shading_language_include */
   struct shader_includes *ShaderIncludes;
   /* glCompileShaderInclude expects ShaderIncludes not to change while it is
    * in progress.
    */
   simple_mtx_t ShaderIncludeMutex;

   /** EXT_external_objects */
   struct _mesa_HashTable *MemoryObjects;

   /** EXT_semaphore */
   struct _mesa_HashTable *SemaphoreObjects;

   /**
    * Whether at least one image has been imported or exported, excluding
    * the default framebuffer. If this is false, glFlush can be executed
    * asynchronously because there is no invisible dependency on external
    * users.
    */
   bool HasExternallySharedImages;

   /* Small display list storage */
   struct {
      union gl_dlist_node *ptr;
      struct util_idalloc free_idx;
      unsigned size;
   } small_dlist_store;

   /* Global GLThread state. */
   struct {
      /* The last context that locked global mutexes. */
      struct gl_context *LastExecutingCtx;

      /* The last time LastExecutingCtx started executing after a different
       * context (the time of multiple active contexts).
       */
      int64_t LastContextSwitchTime;

      /* The time for which no context can lock global mutexes since
       * LastContextSwitchTime.
       */
      int64_t NoLockDuration;
   } GLThread;
};


struct Ruda_Context; 

struct Ruda_Context {
	/** State possibly shared with other contexts in the address space */
	struct Ruda_Shared_State *Shared;

	/** Whether Shared->BufferObjects has already been locked for this context. */
	bool BufferObjectsLocked;

	XGC xContext;

	struct pipe_context *pipe;
	
	// binds together events + window objects
	XC xStore;

	struct Ruda_Array_Attrib Array;	/**< Vertex arrays */

	Ruda_Context(XGC xContext) {this->xContext = xContext;};

};

typedef enum
{
   USAGE_UNIFORM_BUFFER = 0x1,
   USAGE_TEXTURE_BUFFER = 0x2,
   USAGE_ATOMIC_COUNTER_BUFFER = 0x4,
   USAGE_SHADER_STORAGE_BUFFER = 0x8,
   USAGE_TRANSFORM_FEEDBACK_BUFFER = 0x10,
   USAGE_PIXEL_PACK_BUFFER = 0x20,
   USAGE_ARRAY_BUFFER = 0x40,
   USAGE_DISABLE_MINMAX_CACHE = 0x100,
} Ruda_Buffer_Usage;

struct Ruda_Buffer_Mapping
{
   uint AccessFlags; /**< Mask of GL_MAP_x_BIT flags */
   void *Pointer;        /**< User-space address of mapping */
   signed long int Offset;        /**< Mapped offset */
   signed long int Length;      /**< Mapped length */
};


/**
 * GL_ARB_vertex/pixel_buffer_object buffer object
 */
struct Ruda_Buffer_Object
{
   int RefCount;
   uint Name;

   /**
    * The context that holds a global buffer reference for the lifetime of
    * the GL buffer ID to skip refcounting for all its private bind points.
    * Other contexts must still do refcounting as usual. Shared binding points
    * like TBO within gl_texture_object are always refcounted.
    *
    * Implementation details:
    * - Only the context that creates the buffer ("creating context") skips
    *   refcounting.
    * - Only buffers represented by an OpenGL buffer ID skip refcounting.
    *   Other internal buffers don't. (glthread requires refcounting for
    *   internal buffers, etc.)
    * - glDeleteBuffers removes the global buffer reference and increments
    *   RefCount for all private bind points where the deleted buffer is bound
    *   (e.g. unbound VAOs that are not changed by glDeleteBuffers),
    *   effectively enabling refcounting for that context. This is the main
    *   point where the global buffer reference is removed.
    * - glDeleteBuffers called from a different context adds the buffer into
    *   the ZombieBufferObjects list, which is a way to notify the creating
    *   context that it should remove its global buffer reference to allow
    *   freeing the buffer. The creating context walks over that list in a few
    *   GL functions.
    * - xxxDestroyContext walks over all buffers and removes its global
    *   reference from those buffers that it created.
    */
   struct Ruda_Context *Ctx;
   int CtxRefCount;   /**< Non-atomic references held by Ctx. */

   Ruda_Buffer_Usage UsageHistory; /**< How has this buffer been used so far? */

   struct pipe_resource *buffer;
   struct gl_context *private_refcount_ctx;
   /* This mechanism allows passing buffer references to the driver without
    * using atomics to increase the reference count.
    *
    * This private refcount can be decremented without atomics but only one
    * context (ctx above) can use this counter to be thread-safe.
    *
    * This number is atomically added to buffer->reference.count at
    * initialization. If it's never used, the same number is atomically
    * subtracted from buffer->reference.count before destruction. If this
    * number is decremented, we can pass that reference to the driver without
    * touching reference.count. At buffer destruction we only subtract
    * the number of references we did not return. This can possibly turn
    * a million atomic increments into 1 add and 1 subtract atomic op.
    */
   int private_refcount;

   uint StorageFlags; /**< GL_MAP_PERSISTENT_BIT, etc. */

   /** Memoization of min/max index computations for static index buffers */
   unsigned MinMaxCacheHitIndices;
   unsigned MinMaxCacheMissIndices;
   struct hash_table *MinMaxCache;
   simple_mtx_t MinMaxCacheMutex;
   bool MinMaxCacheDirty:1;

   bool DeletePending:1;  /**< true if buffer object is removed from the hash */
   bool Immutable:1;    /**< GL_ARB_buffer_storage */
   bool HandleAllocated:1; /**< GL_ARB_bindless_texture */
   bool GLThreadInternal:1; /**< Created by glthread. */
   unsigned short Usage;      /**< GL_STREAM_DRAW_ARB, GL_STREAM_READ_ARB, etc. */
   char *Label;       /**< GL_KHR_debug */
   signed long int Size;  /**< Size of buffer storage in bytes */

   /** Counters used for buffer usage warnings */
   uint NumSubDataCalls;
   uint NumMapBufferWriteCalls;

   struct Ruda_Buffer_Mapping Mappings[3];
   struct pipe_transfer *transfer[3];
};