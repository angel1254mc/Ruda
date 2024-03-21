#ifndef __GL_CONFIG_H__
#define __GL_CONFIG_H__

/**
 * Framebuffer configuration (aka visual / pixelformat)
 * Note: some of these fields should be boolean, but it appears that
 * code in drivers/dri/common/util.c requires int-sized fields.
 */
struct gl_config
{
   bool floatMode;
   uint doubleBufferMode;
   uint stereoMode;

   int redBits, greenBits, blueBits, alphaBits;	/* bits per comp */
   uint redMask, greenMask, blueMask, alphaMask;
   int redShift, greenShift, blueShift, alphaShift;
   int rgbBits;		/* total bits for rgb */

   int accumRedBits, accumGreenBits, accumBlueBits, accumAlphaBits;
   int depthBits;
   int stencilBits;

   /* ARB_multisample / SGIS_multisample */
   uint samples;

   /* EXT_framebuffer_sRGB */
   int sRGBCapable;
};


#endif