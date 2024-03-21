#include "Ruda/ruda.h"
#include "Ruda/structs.h"
#include "RudaDI/di_structs.h"
#include "hash.h"
#include "../../src/util/simple_mtx.h"
#include "../../src/util/hash_table.h"
#include "context.h"
#include "./state_tracker/st_atom.h"
#include "./state_tracker/st_atom_list.h"
#include "../util/format/u_formats.h"
#include "../../src/gallium/auxiliary/util/u_inlines.h"
#include "../../src/compiler/shader_enums.h"

/**
 * Formats for textures, surfaces and vertex data
 */
enum pipe_format {
   PIPE_FORMAT_NONE,
   /* Vertex formats must be first and must be <= 255. */
   PIPE_FORMAT_R64_UINT,    /**< raw doubles (ARB_vertex_attrib_64bit) */
   PIPE_FORMAT_R64G64_UINT,
   PIPE_FORMAT_R64G64B64_UINT,
   PIPE_FORMAT_R64G64B64A64_UINT,
   PIPE_FORMAT_R64_SINT,
   PIPE_FORMAT_R64G64_SINT,
   PIPE_FORMAT_R64G64B64_SINT,
   PIPE_FORMAT_R64G64B64A64_SINT,
   PIPE_FORMAT_R64_FLOAT,   /**< doubles converted to float */
   PIPE_FORMAT_R64G64_FLOAT,
   PIPE_FORMAT_R64G64B64_FLOAT,
   PIPE_FORMAT_R64G64B64A64_FLOAT,
   PIPE_FORMAT_R32_FLOAT,
   PIPE_FORMAT_R32G32_FLOAT,
   PIPE_FORMAT_R32G32B32_FLOAT,
   PIPE_FORMAT_R32G32B32A32_FLOAT,
   PIPE_FORMAT_R32_UNORM,
   PIPE_FORMAT_R32G32_UNORM,
   PIPE_FORMAT_R32G32B32_UNORM,
   PIPE_FORMAT_R32G32B32A32_UNORM,
   PIPE_FORMAT_R32_USCALED,
   PIPE_FORMAT_R32G32_USCALED,
   PIPE_FORMAT_R32G32B32_USCALED,
   PIPE_FORMAT_R32G32B32A32_USCALED,
   PIPE_FORMAT_R32_SNORM,
   PIPE_FORMAT_R32G32_SNORM,
   PIPE_FORMAT_R32G32B32_SNORM,
   PIPE_FORMAT_R32G32B32A32_SNORM,
   PIPE_FORMAT_R32_SSCALED,
   PIPE_FORMAT_R32G32_SSCALED,
   PIPE_FORMAT_R32G32B32_SSCALED,
   PIPE_FORMAT_R32G32B32A32_SSCALED,
   PIPE_FORMAT_R16_UNORM,
   PIPE_FORMAT_R16G16_UNORM,
   PIPE_FORMAT_R16G16B16_UNORM,
   PIPE_FORMAT_R16G16B16A16_UNORM,
   PIPE_FORMAT_R16_USCALED,
   PIPE_FORMAT_R16G16_USCALED,
   PIPE_FORMAT_R16G16B16_USCALED,
   PIPE_FORMAT_R16G16B16A16_USCALED,
   PIPE_FORMAT_R16_SNORM,
   PIPE_FORMAT_R16G16_SNORM,
   PIPE_FORMAT_R16G16B16_SNORM,
   PIPE_FORMAT_R16G16B16A16_SNORM,
   PIPE_FORMAT_R16_SSCALED,
   PIPE_FORMAT_R16G16_SSCALED,
   PIPE_FORMAT_R16G16B16_SSCALED,
   PIPE_FORMAT_R16G16B16A16_SSCALED,
   PIPE_FORMAT_R8_UNORM,
   PIPE_FORMAT_R8G8_UNORM,
   PIPE_FORMAT_R8G8B8_UNORM,
   PIPE_FORMAT_B8G8R8_UNORM,
   PIPE_FORMAT_R8G8B8A8_UNORM,
   PIPE_FORMAT_B8G8R8A8_UNORM,
   PIPE_FORMAT_R8_USCALED,
   PIPE_FORMAT_R8G8_USCALED,
   PIPE_FORMAT_R8G8B8_USCALED,
   PIPE_FORMAT_B8G8R8_USCALED,
   PIPE_FORMAT_R8G8B8A8_USCALED,
   PIPE_FORMAT_B8G8R8A8_USCALED,
   PIPE_FORMAT_A8B8G8R8_USCALED,
   PIPE_FORMAT_R8_SNORM,
   PIPE_FORMAT_R8G8_SNORM,
   PIPE_FORMAT_R8G8B8_SNORM,
   PIPE_FORMAT_B8G8R8_SNORM,
   PIPE_FORMAT_R8G8B8A8_SNORM,
   PIPE_FORMAT_B8G8R8A8_SNORM,
   PIPE_FORMAT_R8_SSCALED,
   PIPE_FORMAT_R8G8_SSCALED,
   PIPE_FORMAT_R8G8B8_SSCALED,
   PIPE_FORMAT_B8G8R8_SSCALED,
   PIPE_FORMAT_R8G8B8A8_SSCALED,
   PIPE_FORMAT_B8G8R8A8_SSCALED,
   PIPE_FORMAT_A8B8G8R8_SSCALED,
   PIPE_FORMAT_A8R8G8B8_UNORM,
   PIPE_FORMAT_R32_FIXED,
   PIPE_FORMAT_R32G32_FIXED,
   PIPE_FORMAT_R32G32B32_FIXED,
   PIPE_FORMAT_R32G32B32A32_FIXED,
   PIPE_FORMAT_R16_FLOAT,
   PIPE_FORMAT_R16G16_FLOAT,
   PIPE_FORMAT_R16G16B16_FLOAT,
   PIPE_FORMAT_R16G16B16A16_FLOAT,
   PIPE_FORMAT_R8_UINT,
   PIPE_FORMAT_R8G8_UINT,
   PIPE_FORMAT_R8G8B8_UINT,
   PIPE_FORMAT_B8G8R8_UINT,
   PIPE_FORMAT_R8G8B8A8_UINT,
   PIPE_FORMAT_B8G8R8A8_UINT,
   PIPE_FORMAT_R8_SINT,
   PIPE_FORMAT_R8G8_SINT,
   PIPE_FORMAT_R8G8B8_SINT,
   PIPE_FORMAT_B8G8R8_SINT,
   PIPE_FORMAT_R8G8B8A8_SINT,
   PIPE_FORMAT_B8G8R8A8_SINT,
   PIPE_FORMAT_R16_UINT,
   PIPE_FORMAT_R16G16_UINT,
   PIPE_FORMAT_R16G16B16_UINT,
   PIPE_FORMAT_R16G16B16A16_UINT,
   PIPE_FORMAT_R16_SINT,
   PIPE_FORMAT_R16G16_SINT,
   PIPE_FORMAT_R16G16B16_SINT,
   PIPE_FORMAT_R16G16B16A16_SINT,
   PIPE_FORMAT_R32_UINT,
   PIPE_FORMAT_R32G32_UINT,
   PIPE_FORMAT_R32G32B32_UINT,
   PIPE_FORMAT_R32G32B32A32_UINT,
   PIPE_FORMAT_R32_SINT,
   PIPE_FORMAT_R32G32_SINT,
   PIPE_FORMAT_R32G32B32_SINT,
   PIPE_FORMAT_R32G32B32A32_SINT,

   PIPE_FORMAT_R10G10B10A2_UNORM,
   PIPE_FORMAT_R10G10B10A2_SNORM,
   PIPE_FORMAT_R10G10B10A2_USCALED,
   PIPE_FORMAT_R10G10B10A2_SSCALED,

   PIPE_FORMAT_B10G10R10A2_UNORM,
   PIPE_FORMAT_B10G10R10A2_SNORM,
   PIPE_FORMAT_B10G10R10A2_USCALED,
   PIPE_FORMAT_B10G10R10A2_SSCALED,

   PIPE_FORMAT_R11G11B10_FLOAT,

   PIPE_FORMAT_R10G10B10A2_UINT,
   PIPE_FORMAT_R10G10B10A2_SINT,

   PIPE_FORMAT_B10G10R10A2_UINT,
   PIPE_FORMAT_B10G10R10A2_SINT,

   /* End of vertex formats. */

   /* Texture-only formats are below. */
   PIPE_FORMAT_B8G8R8X8_UNORM,
   PIPE_FORMAT_X8B8G8R8_UNORM,
   PIPE_FORMAT_X8R8G8B8_UNORM,
   PIPE_FORMAT_B5G5R5A1_UNORM,
   PIPE_FORMAT_R4G4B4A4_UNORM,
   PIPE_FORMAT_B4G4R4A4_UNORM,
   PIPE_FORMAT_R5G6B5_UNORM,
   PIPE_FORMAT_B5G6R5_UNORM,
   PIPE_FORMAT_L8_UNORM,    /**< ubyte luminance */
   PIPE_FORMAT_A8_UNORM,    /**< ubyte alpha */
   PIPE_FORMAT_I8_UNORM,    /**< ubyte intensity */
   PIPE_FORMAT_L8A8_UNORM,  /**< ubyte alpha, luminance */
   PIPE_FORMAT_L16_UNORM,   /**< ushort luminance */
   PIPE_FORMAT_UYVY,
   PIPE_FORMAT_VYUY,
   PIPE_FORMAT_YUYV,
   PIPE_FORMAT_YVYU,
   PIPE_FORMAT_Z16_UNORM,
   PIPE_FORMAT_Z16_UNORM_S8_UINT,
   PIPE_FORMAT_Z32_UNORM,
   PIPE_FORMAT_Z32_FLOAT,
   PIPE_FORMAT_Z24_UNORM_S8_UINT,
   PIPE_FORMAT_S8_UINT_Z24_UNORM,
   PIPE_FORMAT_Z24X8_UNORM,
   PIPE_FORMAT_X8Z24_UNORM,
   PIPE_FORMAT_S8_UINT,     /**< ubyte stencil */

   /* sRGB formats */
   PIPE_FORMAT_L8_SRGB,
   PIPE_FORMAT_R8_SRGB,
   PIPE_FORMAT_L8A8_SRGB,
   PIPE_FORMAT_R8G8_SRGB,
   PIPE_FORMAT_R8G8B8_SRGB,
   PIPE_FORMAT_B8G8R8_SRGB,
   PIPE_FORMAT_A8B8G8R8_SRGB,
   PIPE_FORMAT_X8B8G8R8_SRGB,
   PIPE_FORMAT_B8G8R8A8_SRGB,
   PIPE_FORMAT_B8G8R8X8_SRGB,
   PIPE_FORMAT_A8R8G8B8_SRGB,
   PIPE_FORMAT_X8R8G8B8_SRGB,
   PIPE_FORMAT_R8G8B8A8_SRGB,

   /* compressed formats */
   PIPE_FORMAT_DXT1_RGB,
   PIPE_FORMAT_DXT1_RGBA,
   PIPE_FORMAT_DXT3_RGBA,
   PIPE_FORMAT_DXT5_RGBA,

   /* sRGB, compressed */
   PIPE_FORMAT_DXT1_SRGB,
   PIPE_FORMAT_DXT1_SRGBA,
   PIPE_FORMAT_DXT3_SRGBA,
   PIPE_FORMAT_DXT5_SRGBA,

   /* rgtc compressed */
   PIPE_FORMAT_RGTC1_UNORM,
   PIPE_FORMAT_RGTC1_SNORM,
   PIPE_FORMAT_RGTC2_UNORM,
   PIPE_FORMAT_RGTC2_SNORM,

   PIPE_FORMAT_R8G8_B8G8_UNORM,
   PIPE_FORMAT_G8R8_G8B8_UNORM,

   /* mixed formats */
   PIPE_FORMAT_R8SG8SB8UX8U_NORM,
   PIPE_FORMAT_R5SG5SB6U_NORM,

   /* TODO: re-order these */
   PIPE_FORMAT_A8B8G8R8_UNORM,
   PIPE_FORMAT_B5G5R5X1_UNORM,
   PIPE_FORMAT_R9G9B9E5_FLOAT,
   PIPE_FORMAT_Z32_FLOAT_S8X24_UINT,
   PIPE_FORMAT_R1_UNORM,
   PIPE_FORMAT_R10G10B10X2_USCALED,
   PIPE_FORMAT_R10G10B10X2_SNORM,
   PIPE_FORMAT_L4A4_UNORM,
   PIPE_FORMAT_A2R10G10B10_UNORM,
   PIPE_FORMAT_A2B10G10R10_UNORM,
   PIPE_FORMAT_R10SG10SB10SA2U_NORM,
   PIPE_FORMAT_R8G8Bx_SNORM,
   PIPE_FORMAT_R8G8B8X8_UNORM,
   PIPE_FORMAT_B4G4R4X4_UNORM,

   /* some stencil samplers formats */
   PIPE_FORMAT_X24S8_UINT,
   PIPE_FORMAT_S8X24_UINT,
   PIPE_FORMAT_X32_S8X24_UINT,

   PIPE_FORMAT_R3G3B2_UNORM,
   PIPE_FORMAT_B2G3R3_UNORM,
   PIPE_FORMAT_L16A16_UNORM,
   PIPE_FORMAT_A16_UNORM,
   PIPE_FORMAT_I16_UNORM,

   PIPE_FORMAT_LATC1_UNORM,
   PIPE_FORMAT_LATC1_SNORM,
   PIPE_FORMAT_LATC2_UNORM,
   PIPE_FORMAT_LATC2_SNORM,

   PIPE_FORMAT_A8_SNORM,
   PIPE_FORMAT_L8_SNORM,
   PIPE_FORMAT_L8A8_SNORM,
   PIPE_FORMAT_I8_SNORM,
   PIPE_FORMAT_A16_SNORM,
   PIPE_FORMAT_L16_SNORM,
   PIPE_FORMAT_L16A16_SNORM,
   PIPE_FORMAT_I16_SNORM,

   PIPE_FORMAT_A16_FLOAT,
   PIPE_FORMAT_L16_FLOAT,
   PIPE_FORMAT_L16A16_FLOAT,
   PIPE_FORMAT_I16_FLOAT,
   PIPE_FORMAT_A32_FLOAT,
   PIPE_FORMAT_L32_FLOAT,
   PIPE_FORMAT_L32A32_FLOAT,
   PIPE_FORMAT_I32_FLOAT,

   PIPE_FORMAT_YV12,
   PIPE_FORMAT_YV16,
   PIPE_FORMAT_IYUV,  /**< aka I420 */
   PIPE_FORMAT_NV12,
   PIPE_FORMAT_NV21,

   PIPE_FORMAT_Y8_400_UNORM,
   /* PIPE_FORMAT_Y8_U8_V8_420_UNORM = IYUV */
   /* PIPE_FORMAT_Y8_U8V8_420_UNORM = NV12 */
   PIPE_FORMAT_Y8_U8_V8_422_UNORM,
   PIPE_FORMAT_Y8_U8V8_422_UNORM,
   PIPE_FORMAT_Y8_U8_V8_444_UNORM,

   PIPE_FORMAT_Y16_U16_V16_420_UNORM,
    /* PIPE_FORMAT_Y16_U16V16_420_UNORM */
   PIPE_FORMAT_Y16_U16_V16_422_UNORM,
   PIPE_FORMAT_Y16_U16V16_422_UNORM,
   PIPE_FORMAT_Y16_U16_V16_444_UNORM,

   PIPE_FORMAT_A4R4_UNORM,
   PIPE_FORMAT_R4A4_UNORM,
   PIPE_FORMAT_R8A8_UNORM,
   PIPE_FORMAT_A8R8_UNORM,

   PIPE_FORMAT_A8_UINT,
   PIPE_FORMAT_I8_UINT,
   PIPE_FORMAT_L8_UINT,
   PIPE_FORMAT_L8A8_UINT,

   PIPE_FORMAT_A8_SINT,
   PIPE_FORMAT_I8_SINT,
   PIPE_FORMAT_L8_SINT,
   PIPE_FORMAT_L8A8_SINT,

   PIPE_FORMAT_A16_UINT,
   PIPE_FORMAT_I16_UINT,
   PIPE_FORMAT_L16_UINT,
   PIPE_FORMAT_L16A16_UINT,

   PIPE_FORMAT_A16_SINT,
   PIPE_FORMAT_I16_SINT,
   PIPE_FORMAT_L16_SINT,
   PIPE_FORMAT_L16A16_SINT,

   PIPE_FORMAT_A32_UINT,
   PIPE_FORMAT_I32_UINT,
   PIPE_FORMAT_L32_UINT,
   PIPE_FORMAT_L32A32_UINT,

   PIPE_FORMAT_A32_SINT,
   PIPE_FORMAT_I32_SINT,
   PIPE_FORMAT_L32_SINT,
   PIPE_FORMAT_L32A32_SINT,

   PIPE_FORMAT_A8R8G8B8_UINT,
   PIPE_FORMAT_A8B8G8R8_UINT,
   PIPE_FORMAT_A2R10G10B10_UINT,
   PIPE_FORMAT_A2B10G10R10_UINT,
   PIPE_FORMAT_R5G6B5_UINT,
   PIPE_FORMAT_B5G6R5_UINT,
   PIPE_FORMAT_R5G5B5A1_UINT,
   PIPE_FORMAT_B5G5R5A1_UINT,
   PIPE_FORMAT_A1R5G5B5_UINT,
   PIPE_FORMAT_A1B5G5R5_UINT,
   PIPE_FORMAT_R4G4B4A4_UINT,
   PIPE_FORMAT_B4G4R4A4_UINT,
   PIPE_FORMAT_A4R4G4B4_UINT,
   PIPE_FORMAT_A4B4G4R4_UINT,
   PIPE_FORMAT_R3G3B2_UINT,
   PIPE_FORMAT_B2G3R3_UINT,

   PIPE_FORMAT_ETC1_RGB8,

   PIPE_FORMAT_R8G8_R8B8_UNORM,
   PIPE_FORMAT_R8B8_R8G8_UNORM,
   PIPE_FORMAT_G8R8_B8R8_UNORM,
   PIPE_FORMAT_B8R8_G8R8_UNORM,
   PIPE_FORMAT_G8B8_G8R8_UNORM,
   PIPE_FORMAT_B8G8_R8G8_UNORM,

   PIPE_FORMAT_R8G8B8X8_SNORM,
   PIPE_FORMAT_R8G8B8X8_SRGB,
   PIPE_FORMAT_R8G8B8X8_UINT,
   PIPE_FORMAT_R8G8B8X8_SINT,
   PIPE_FORMAT_B10G10R10X2_UNORM,
   PIPE_FORMAT_R16G16B16X16_UNORM,
   PIPE_FORMAT_R16G16B16X16_SNORM,
   PIPE_FORMAT_R16G16B16X16_FLOAT,
   PIPE_FORMAT_R16G16B16X16_UINT,
   PIPE_FORMAT_R16G16B16X16_SINT,
   PIPE_FORMAT_R32G32B32X32_FLOAT,
   PIPE_FORMAT_R32G32B32X32_UINT,
   PIPE_FORMAT_R32G32B32X32_SINT,

   PIPE_FORMAT_R8A8_SNORM,
   PIPE_FORMAT_R16A16_UNORM,
   PIPE_FORMAT_R16A16_SNORM,
   PIPE_FORMAT_R16A16_FLOAT,
   PIPE_FORMAT_R32A32_FLOAT,
   PIPE_FORMAT_R8A8_UINT,
   PIPE_FORMAT_R8A8_SINT,
   PIPE_FORMAT_R16A16_UINT,
   PIPE_FORMAT_R16A16_SINT,
   PIPE_FORMAT_R32A32_UINT,
   PIPE_FORMAT_R32A32_SINT,

   PIPE_FORMAT_B5G6R5_SRGB,

   PIPE_FORMAT_BPTC_RGBA_UNORM,
   PIPE_FORMAT_BPTC_SRGBA,
   PIPE_FORMAT_BPTC_RGB_FLOAT,
   PIPE_FORMAT_BPTC_RGB_UFLOAT,

   PIPE_FORMAT_G8R8_UNORM,
   PIPE_FORMAT_G8R8_SNORM,
   PIPE_FORMAT_G16R16_UNORM,
   PIPE_FORMAT_G16R16_SNORM,

   PIPE_FORMAT_A8B8G8R8_SNORM,
   PIPE_FORMAT_X8B8G8R8_SNORM,

   PIPE_FORMAT_ETC2_RGB8,
   PIPE_FORMAT_ETC2_SRGB8,
   PIPE_FORMAT_ETC2_RGB8A1,
   PIPE_FORMAT_ETC2_SRGB8A1,
   PIPE_FORMAT_ETC2_RGBA8,
   PIPE_FORMAT_ETC2_SRGBA8,
   PIPE_FORMAT_ETC2_R11_UNORM,
   PIPE_FORMAT_ETC2_R11_SNORM,
   PIPE_FORMAT_ETC2_RG11_UNORM,
   PIPE_FORMAT_ETC2_RG11_SNORM,

   PIPE_FORMAT_ASTC_4x4,
   PIPE_FORMAT_ASTC_5x4,
   PIPE_FORMAT_ASTC_5x5,
   PIPE_FORMAT_ASTC_6x5,
   PIPE_FORMAT_ASTC_6x6,
   PIPE_FORMAT_ASTC_8x5,
   PIPE_FORMAT_ASTC_8x6,
   PIPE_FORMAT_ASTC_8x8,
   PIPE_FORMAT_ASTC_10x5,
   PIPE_FORMAT_ASTC_10x6,
   PIPE_FORMAT_ASTC_10x8,
   PIPE_FORMAT_ASTC_10x10,
   PIPE_FORMAT_ASTC_12x10,
   PIPE_FORMAT_ASTC_12x12,

   PIPE_FORMAT_ASTC_4x4_SRGB,
   PIPE_FORMAT_ASTC_5x4_SRGB,
   PIPE_FORMAT_ASTC_5x5_SRGB,
   PIPE_FORMAT_ASTC_6x5_SRGB,
   PIPE_FORMAT_ASTC_6x6_SRGB,
   PIPE_FORMAT_ASTC_8x5_SRGB,
   PIPE_FORMAT_ASTC_8x6_SRGB,
   PIPE_FORMAT_ASTC_8x8_SRGB,
   PIPE_FORMAT_ASTC_10x5_SRGB,
   PIPE_FORMAT_ASTC_10x6_SRGB,
   PIPE_FORMAT_ASTC_10x8_SRGB,
   PIPE_FORMAT_ASTC_10x10_SRGB,
   PIPE_FORMAT_ASTC_12x10_SRGB,
   PIPE_FORMAT_ASTC_12x12_SRGB,

   PIPE_FORMAT_ASTC_3x3x3,
   PIPE_FORMAT_ASTC_4x3x3,
   PIPE_FORMAT_ASTC_4x4x3,
   PIPE_FORMAT_ASTC_4x4x4,
   PIPE_FORMAT_ASTC_5x4x4,
   PIPE_FORMAT_ASTC_5x5x4,
   PIPE_FORMAT_ASTC_5x5x5,
   PIPE_FORMAT_ASTC_6x5x5,
   PIPE_FORMAT_ASTC_6x6x5,
   PIPE_FORMAT_ASTC_6x6x6,

   PIPE_FORMAT_ASTC_3x3x3_SRGB,
   PIPE_FORMAT_ASTC_4x3x3_SRGB,
   PIPE_FORMAT_ASTC_4x4x3_SRGB,
   PIPE_FORMAT_ASTC_4x4x4_SRGB,
   PIPE_FORMAT_ASTC_5x4x4_SRGB,
   PIPE_FORMAT_ASTC_5x5x4_SRGB,
   PIPE_FORMAT_ASTC_5x5x5_SRGB,
   PIPE_FORMAT_ASTC_6x5x5_SRGB,
   PIPE_FORMAT_ASTC_6x6x5_SRGB,
   PIPE_FORMAT_ASTC_6x6x6_SRGB,

   PIPE_FORMAT_FXT1_RGB,
   PIPE_FORMAT_FXT1_RGBA,

   PIPE_FORMAT_P010,
   PIPE_FORMAT_P012,
   PIPE_FORMAT_P016,
   PIPE_FORMAT_P030,

   PIPE_FORMAT_Y210,
   PIPE_FORMAT_Y212,
   PIPE_FORMAT_Y216,

   PIPE_FORMAT_Y410,
   PIPE_FORMAT_Y412,
   PIPE_FORMAT_Y416,

   PIPE_FORMAT_R10G10B10X2_UNORM,
   PIPE_FORMAT_A1R5G5B5_UNORM,
   PIPE_FORMAT_A1B5G5R5_UNORM,
   PIPE_FORMAT_X1B5G5R5_UNORM,
   PIPE_FORMAT_R5G5B5A1_UNORM,
   PIPE_FORMAT_A4R4G4B4_UNORM,
   PIPE_FORMAT_A4B4G4R4_UNORM,

   PIPE_FORMAT_G8R8_SINT,
   PIPE_FORMAT_A8B8G8R8_SINT,
   PIPE_FORMAT_X8B8G8R8_SINT,

   PIPE_FORMAT_ATC_RGB,
   PIPE_FORMAT_ATC_RGBA_EXPLICIT,
   PIPE_FORMAT_ATC_RGBA_INTERPOLATED,

   PIPE_FORMAT_Z24_UNORM_S8_UINT_AS_R8G8B8A8,

   PIPE_FORMAT_AYUV,
   PIPE_FORMAT_XYUV,

   PIPE_FORMAT_R8_G8B8_420_UNORM,
   PIPE_FORMAT_R8_B8G8_420_UNORM,
   PIPE_FORMAT_G8_B8R8_420_UNORM,
   PIPE_FORMAT_R8_G8_B8_420_UNORM,
   PIPE_FORMAT_R8_B8_G8_420_UNORM,
   PIPE_FORMAT_G8_B8_R8_420_UNORM,
   PIPE_FORMAT_R8_G8_B8_UNORM,
   PIPE_FORMAT_Y8_UNORM,

   PIPE_FORMAT_B8G8R8X8_SNORM,
   PIPE_FORMAT_B8G8R8X8_UINT,
   PIPE_FORMAT_B8G8R8X8_SINT,
   PIPE_FORMAT_A8R8G8B8_SNORM,
   PIPE_FORMAT_A8R8G8B8_SINT,
   PIPE_FORMAT_X8R8G8B8_SNORM,
   PIPE_FORMAT_X8R8G8B8_SINT,
   PIPE_FORMAT_R5G5B5X1_UNORM,
   PIPE_FORMAT_X1R5G5B5_UNORM,
   PIPE_FORMAT_R4G4B4X4_UNORM,
   PIPE_FORMAT_B10G10R10X2_SNORM,
   PIPE_FORMAT_R5G6B5_SRGB,
   PIPE_FORMAT_R10G10B10X2_SINT,
   PIPE_FORMAT_B10G10R10X2_SINT,
   PIPE_FORMAT_G16R16_SINT,

   PIPE_FORMAT_COUNT
};

#if UTIL_ARCH_LITTLE_ENDIAN
#define PIPE_FORMAT_RGBA8888_UNORM PIPE_FORMAT_R8G8B8A8_UNORM
#define PIPE_FORMAT_RGBX8888_UNORM PIPE_FORMAT_R8G8B8X8_UNORM
#define PIPE_FORMAT_BGRA8888_UNORM PIPE_FORMAT_B8G8R8A8_UNORM
#define PIPE_FORMAT_BGRX8888_UNORM PIPE_FORMAT_B8G8R8X8_UNORM
#define PIPE_FORMAT_ARGB8888_UNORM PIPE_FORMAT_A8R8G8B8_UNORM
#define PIPE_FORMAT_XRGB8888_UNORM PIPE_FORMAT_X8R8G8B8_UNORM
#define PIPE_FORMAT_ABGR8888_UNORM PIPE_FORMAT_A8B8G8R8_UNORM
#define PIPE_FORMAT_XBGR8888_UNORM PIPE_FORMAT_X8B8G8R8_UNORM
#define PIPE_FORMAT_RGBA8888_SNORM PIPE_FORMAT_R8G8B8A8_SNORM
#define PIPE_FORMAT_RGBX8888_SNORM PIPE_FORMAT_R8G8B8X8_SNORM
#define PIPE_FORMAT_ABGR8888_SNORM PIPE_FORMAT_A8B8G8R8_SNORM
#define PIPE_FORMAT_XBGR8888_SNORM PIPE_FORMAT_X8B8G8R8_SNORM
#define PIPE_FORMAT_RGBA8888_SRGB PIPE_FORMAT_R8G8B8A8_SRGB
#define PIPE_FORMAT_RGBX8888_SRGB PIPE_FORMAT_R8G8B8X8_SRGB
#define PIPE_FORMAT_BGRA8888_SRGB PIPE_FORMAT_B8G8R8A8_SRGB
#define PIPE_FORMAT_BGRX8888_SRGB PIPE_FORMAT_B8G8R8X8_SRGB
#define PIPE_FORMAT_ARGB8888_SRGB PIPE_FORMAT_A8R8G8B8_SRGB
#define PIPE_FORMAT_XRGB8888_SRGB PIPE_FORMAT_X8R8G8B8_SRGB
#define PIPE_FORMAT_ABGR8888_SRGB PIPE_FORMAT_A8B8G8R8_SRGB
#define PIPE_FORMAT_XBGR8888_SRGB PIPE_FORMAT_X8B8G8R8_SRGB
#define PIPE_FORMAT_RGBA8888_USCALED PIPE_FORMAT_R8G8B8A8_USCALED
#define PIPE_FORMAT_RGBA8888_SSCALED PIPE_FORMAT_R8G8B8A8_SSCALED
#define PIPE_FORMAT_RGBA8888_UINT PIPE_FORMAT_R8G8B8A8_UINT
#define PIPE_FORMAT_BGRA8888_UINT PIPE_FORMAT_B8G8R8A8_UINT
#define PIPE_FORMAT_ARGB8888_UINT PIPE_FORMAT_A8R8G8B8_UINT
#define PIPE_FORMAT_ABGR8888_UINT PIPE_FORMAT_A8B8G8R8_UINT
#define PIPE_FORMAT_RGBA8888_SINT PIPE_FORMAT_R8G8B8A8_SINT
#define PIPE_FORMAT_RG88_UNORM PIPE_FORMAT_R8G8_UNORM
#define PIPE_FORMAT_GR88_UNORM PIPE_FORMAT_G8R8_UNORM
#define PIPE_FORMAT_RG88_SNORM PIPE_FORMAT_R8G8_SNORM
#define PIPE_FORMAT_GR88_SNORM PIPE_FORMAT_G8R8_SNORM
#define PIPE_FORMAT_RG1616_UNORM PIPE_FORMAT_R16G16_UNORM
#define PIPE_FORMAT_GR1616_UNORM PIPE_FORMAT_G16R16_UNORM
#define PIPE_FORMAT_RG1616_SNORM PIPE_FORMAT_R16G16_SNORM
#define PIPE_FORMAT_GR1616_SNORM PIPE_FORMAT_G16R16_SNORM
#elif UTIL_ARCH_BIG_ENDIAN
#define PIPE_FORMAT_ABGR8888_UNORM PIPE_FORMAT_R8G8B8A8_UNORM
#define PIPE_FORMAT_XBGR8888_UNORM PIPE_FORMAT_R8G8B8X8_UNORM
#define PIPE_FORMAT_ARGB8888_UNORM PIPE_FORMAT_B8G8R8A8_UNORM
#define PIPE_FORMAT_XRGB8888_UNORM PIPE_FORMAT_B8G8R8X8_UNORM
#define PIPE_FORMAT_BGRA8888_UNORM PIPE_FORMAT_A8R8G8B8_UNORM
#define PIPE_FORMAT_BGRX8888_UNORM PIPE_FORMAT_X8R8G8B8_UNORM
#define PIPE_FORMAT_RGBA8888_UNORM PIPE_FORMAT_A8B8G8R8_UNORM
#define PIPE_FORMAT_RGBX8888_UNORM PIPE_FORMAT_X8B8G8R8_UNORM
#define PIPE_FORMAT_ABGR8888_SNORM PIPE_FORMAT_R8G8B8A8_SNORM
#define PIPE_FORMAT_XBGR8888_SNORM PIPE_FORMAT_R8G8B8X8_SNORM
#define PIPE_FORMAT_RGBA8888_SNORM PIPE_FORMAT_A8B8G8R8_SNORM
#define PIPE_FORMAT_RGBX8888_SNORM PIPE_FORMAT_X8B8G8R8_SNORM
#define PIPE_FORMAT_ABGR8888_SRGB PIPE_FORMAT_R8G8B8A8_SRGB
#define PIPE_FORMAT_XBGR8888_SRGB PIPE_FORMAT_R8G8B8X8_SRGB
#define PIPE_FORMAT_ARGB8888_SRGB PIPE_FORMAT_B8G8R8A8_SRGB
#define PIPE_FORMAT_XRGB8888_SRGB PIPE_FORMAT_B8G8R8X8_SRGB
#define PIPE_FORMAT_BGRA8888_SRGB PIPE_FORMAT_A8R8G8B8_SRGB
#define PIPE_FORMAT_BGRX8888_SRGB PIPE_FORMAT_X8R8G8B8_SRGB
#define PIPE_FORMAT_RGBA8888_SRGB PIPE_FORMAT_A8B8G8R8_SRGB
#define PIPE_FORMAT_RGBX8888_SRGB PIPE_FORMAT_X8B8G8R8_SRGB
#define PIPE_FORMAT_RGBA8888_USCALED PIPE_FORMAT_A8B8G8R8_USCALED
#define PIPE_FORMAT_RGBA8888_SSCALED PIPE_FORMAT_A8B8G8R8_SSCALED
#define PIPE_FORMAT_RGBA8888_UINT PIPE_FORMAT_A8B8G8R8_UINT
#define PIPE_FORMAT_BGRA8888_UINT PIPE_FORMAT_A8R8G8B8_UINT
#define PIPE_FORMAT_ARGB8888_UINT PIPE_FORMAT_B8G8R8A8_UINT
#define PIPE_FORMAT_ABGR8888_UINT PIPE_FORMAT_R8G8B8A8_UINT
#define PIPE_FORMAT_RGBA8888_SINT PIPE_FORMAT_A8B8G8R8_SINT
#define PIPE_FORMAT_RG88_UNORM PIPE_FORMAT_G8R8_UNORM
#define PIPE_FORMAT_GR88_UNORM PIPE_FORMAT_R8G8_UNORM
#define PIPE_FORMAT_RG88_SNORM PIPE_FORMAT_G8R8_SNORM
#define PIPE_FORMAT_GR88_SNORM PIPE_FORMAT_R8G8_SNORM
#define PIPE_FORMAT_RG1616_UNORM PIPE_FORMAT_G16R16_UNORM
#define PIPE_FORMAT_GR1616_UNORM PIPE_FORMAT_R16G16_UNORM
#define PIPE_FORMAT_RG1616_SNORM PIPE_FORMAT_G16R16_SNORM
#define PIPE_FORMAT_GR1616_SNORM PIPE_FORMAT_R16G16_SNORM
#endif

enum pipe_video_chroma_format
{
   PIPE_VIDEO_CHROMA_FORMAT_400,
   PIPE_VIDEO_CHROMA_FORMAT_420,
   PIPE_VIDEO_CHROMA_FORMAT_422,
   PIPE_VIDEO_CHROMA_FORMAT_444,
   PIPE_VIDEO_CHROMA_FORMAT_NONE
};

/**
 * Texture & format swizzles
 */
enum pipe_swizzle {
   PIPE_SWIZZLE_X,
   PIPE_SWIZZLE_Y,
   PIPE_SWIZZLE_Z,
   PIPE_SWIZZLE_W,
   PIPE_SWIZZLE_0,
   PIPE_SWIZZLE_1,
   PIPE_SWIZZLE_NONE,
   PIPE_SWIZZLE_MAX, /**< Number of enums counter (must be last) */
};

#define PIPE_MASK_R  0x1
#define PIPE_MASK_G  0x2
#define PIPE_MASK_B  0x4
#define PIPE_MASK_A  0x8
#define PIPE_MASK_RGBA 0xf
#define PIPE_MASK_Z  0x10
#define PIPE_MASK_S  0x20
#define PIPE_MASK_ZS 0x30
#define PIPE_MASK_RGBAZS (PIPE_MASK_RGBA|PIPE_MASK_ZS)

union pipe_color_union
{
   float f[4];
   int i[4];
   unsigned int ui[4];
};


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
                      unsigned int index)
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
                       unsigned int index)
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




void rudaBindVBO(uint* buffer) {
   bind_buffer_object(structure->ctx, *buffer);
}



















/**
 * From a buffer object's target, immutability flag, storage flags and
 * usage hint, return a pipe_resource_usage value (PIPE_USAGE_DYNAMIC,
 * STREAM, etc).
 */
static enum pipe_resource_usage
buffer_usage(bool immutable,
             unsigned int storageFlags, int usage)
{
   /* "immutable" means that "storageFlags" was set by the user and "usage"
    * was guessed by Mesa. Otherwise, "usage" was set by the user and
    * storageFlags was guessed by Mesa.
    *
    * Therefore, use storageFlags with immutable, else use "usage".
    */
   if (immutable) {
      /* BufferStorage */
      if (storageFlags & GL_MAP_READ_BIT)
         return PIPE_USAGE_STAGING;
      else if (storageFlags & GL_CLIENT_STORAGE_BIT)
         return PIPE_USAGE_STREAM;
      else
         return PIPE_USAGE_DEFAULT;
   }
   else {
      /* These are often read by the CPU, so enable CPU caches. */

      /* BufferData */
      switch (usage) {
      case GL_DYNAMIC_DRAW:
      case GL_DYNAMIC_COPY:
         return PIPE_USAGE_DYNAMIC;
      case GL_STREAM_DRAW:
      case GL_STREAM_COPY:
         return PIPE_USAGE_STREAM;
      case GL_STATIC_READ:
      case GL_DYNAMIC_READ:
      case GL_STREAM_READ:
         return PIPE_USAGE_STAGING;
      case GL_STATIC_DRAW:
      case GL_STATIC_COPY:
      default:
         return PIPE_USAGE_DEFAULT;
      }
   }
}


/**
 * Return bitmask of PIPE_RESOURCE_x flags corresponding to GL_MAP_x flags.
 */
static unsigned
storage_flags_to_buffer_flags(unsigned int storageFlags)
{
   unsigned flags = 0;
   if (storageFlags & GL_MAP_PERSISTENT_BIT)
      flags |= PIPE_RESOURCE_FLAG_MAP_PERSISTENT;
   if (storageFlags & GL_MAP_COHERENT_BIT)
      flags |= PIPE_RESOURCE_FLAG_MAP_COHERENT;
   if (storageFlags & GL_SPARSE_STORAGE_BIT_ARB)
      flags |= PIPE_RESOURCE_FLAG_SPARSE;
   return flags;
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

    unsigned bindings = PIPE_BIND_VERTEX_BUFFER;

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
        buffer.usage = buffer_usage(obj->Immutable, storageFlags, usage);
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
void rudaBufferArrayData(int size, const void *data) {
   // Our fake "get_buffer" logic
   Ruda_Context* ctx = structure->ctx;
   struct Ruda_Buffer_Object* bufObj = ctx->Array.ArrayBufferObj;
   int usage = GL_STATIC_DRAW;
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


/**
 * Depending on the position and generic0 attributes enable flags select
 * the one that is used for both attributes.
 * The generic0 attribute takes precedence.
 */
static inline void
update_attribute_map_mode(const struct Ruda_Context *ctx,
                          struct Ruda_Vertex_Array_Object *vao)
{
   /*
    * There is no need to change the mapping away from the
    * identity mapping if we are not in compat mode.
    */
   if (ctx->API != API_OPENGL_COMPAT)
      return;
   /* The generic0 attribute superseeds the position attribute */
   const unsigned int enabled = vao->Enabled;
   if (enabled & VERT_BIT_GENERIC0)
      vao->_AttributeMapMode = ATTRIBUTE_MAP_MODE_GENERIC0;
   else if (enabled & VERT_BIT_POS)
      vao->_AttributeMapMode = ATTRIBUTE_MAP_MODE_POSITION;
   else
      vao->_AttributeMapMode = ATTRIBUTE_MAP_MODE_IDENTITY;
}

void update_edgeflag_state_vao(struct Ruda_Context* ctx) {
   bool per_vertex_enable = ctx->Array._DrawVAO->Enabled & VERT_BIT_EDGEFLAG;
   if (ctx->API != API_OPENGL_COMPAT)
      return;

   /* Edge flags take effect only if the polygon mode is not FILL, and they
    * determine whether a line or point is drawn with that polygon mode.
    */
   bool edgeflags_have_effect = ctx->Polygon.FrontMode != GL_FILL ||
                                ctx->Polygon.BackMode != GL_FILL;
   per_vertex_enable &= edgeflags_have_effect;

   if (per_vertex_enable != ctx->Array._PerVertexEdgeFlagsEnabled) {
      ctx->Array._PerVertexEdgeFlagsEnabled = per_vertex_enable;

      struct Ruda_Program *vp = ctx->VertexProgram._Current;
      if (vp) {
         ctx->NewDriverState |= ST_NEW_VS_STATE |
                                ST_NEW_VERTEX_ARRAYS;
         ctx->Array.NewVertexElements = true;
      }
   }

   /* If there are no per-vertex edge flags and the zero-stride edge flag is
    * false, all front and back points and lines generated by polygon mode
    * are not drawn.
    */
   bool polygon_mode_always_culls = edgeflags_have_effect &&
                                    !ctx->Array._PerVertexEdgeFlagsEnabled &&
                                    !ctx->Current.Attrib[VERT_ATTRIB_EDGEFLAG][0];
   if (polygon_mode_always_culls != ctx->Array._PolygonModeAlwaysCulls) {
      ctx->Array._PolygonModeAlwaysCulls = polygon_mode_always_culls;
      ctx->NewDriverState |= ST_NEW_RASTERIZER;
   }
}

unsigned int vao_enable_to_vp_inputs(Ruda_Attribute_Map_Mode mode, unsigned int enabled) {
   switch (mode) {
   case ATTRIBUTE_MAP_MODE_IDENTITY:
      return enabled;
   case ATTRIBUTE_MAP_MODE_POSITION:
      /* Copy VERT_ATTRIB_POS enable bit into GENERIC0 position */
      return (enabled & ~VERT_BIT_GENERIC0)
         | ((enabled & VERT_BIT_POS) << VERT_ATTRIB_GENERIC0);
   case ATTRIBUTE_MAP_MODE_GENERIC0:
      /* Copy VERT_ATTRIB_GENERIC0 enable bit into POS position */
      return (enabled & ~VERT_BIT_POS)
         | ((enabled & VERT_BIT_GENERIC0) >> VERT_ATTRIB_GENERIC0);
   default:
      return 0;
   }
}


 void enable_vertex_array_attribs(struct Ruda_Context *ctx,
                                 struct Ruda_Vertex_Array_Object *vao,
                                 unsigned int attrib_bits)  {
   assert(!vao->SharedAndImmutable);

   /* Only work on bits that are disabled */
   attrib_bits &= ~vao->Enabled;
   if (attrib_bits) {
      /* was disabled, now being enabled */
      vao->Enabled |= attrib_bits;
      vao->NonDefaultStateMask |= attrib_bits;
      ctx->NewDriverState |= ST_NEW_VERTEX_ARRAYS;
      ctx->Array.NewVertexElements = true;

      /* Update the map mode if needed */
      if (attrib_bits & (VERT_BIT_POS|VERT_BIT_GENERIC0))
         update_attribute_map_mode(ctx, vao);

      if (attrib_bits & VERT_BIT_EDGEFLAG)
         update_edgeflag_state_vao(ctx);

      vao->_EnabledWithMapMode =
         vao_enable_to_vp_inputs(vao->_AttributeMapMode, vao->Enabled);
   }
}

void enable_vertex_array_attrib(struct Ruda_Context *ctx,
                                 struct Ruda_Vertex_Array_Object *vao,
                                 Ruda_Vert_Attrib attrib)
{
   assert(attrib < VERT_ATTRIB_MAX);
   enable_vertex_array_attribs(ctx, vao, VERT_BIT(attrib));
};
   
void rudaEnableVertexAttribArray(unsigned int index) {
   Ruda_Context* ctx = structure->ctx;
   Ruda_Vertex_Array_Object* vao = ctx->Array.VAO;
   // First check that index is valid
   if (index >= ctx->Const.Program[MESA_SHADER_VERTEX].MaxAttribs) {
         std::cout << "Error: invalid index at rudaEnableVertexAttribArray" << std::endl;
         return;
   }
   enable_vertex_array_attrib(ctx, vao, (Ruda_Vert_Attrib)VERT_ATTRIB_GENERIC(index));

}
 // END ENABLE VERTEX ATTRIBUTE DATA //