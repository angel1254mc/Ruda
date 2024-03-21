#include "RudaDI/di.h"
#include "RudaDI/di_structs.h"
#include "Ruda/rudax.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <limits.h>
#define NIL (0)
// Initializes  


DI_Structure* structure = nullptr;
DI_WindowConfig defaultWindowConfig = DI_WindowConfig();


const DI_FBConfig* diChooseFBConfig(const DI_FBConfig* desired,
                                         const DI_FBConfig* alternatives,
                                         unsigned int count)
{
    unsigned int i;
    unsigned int missing, leastMissing = UINT_MAX;
    unsigned int colorDiff, leastColorDiff = UINT_MAX;
    unsigned int extraDiff, leastExtraDiff = UINT_MAX;
    const DI_FBConfig* current;
    const DI_FBConfig* closest = NULL;

    for (i = 0;  i < count;  i++)
    {
        current = alternatives + i;

        if (desired->stereo > 0 && current->stereo == 0)
        {
            // Stereo is a hard constraint
            continue;
        }

        // Count number of missing buffers
        {
            missing = 0;

            if (desired->alphaBits > 0 && current->alphaBits == 0)
                missing++;

            if (desired->depthBits > 0 && current->depthBits == 0)
                missing++;

            if (desired->stencilBits > 0 && current->stencilBits == 0)
                missing++;

            if (desired->auxBuffers > 0 &&
                current->auxBuffers < desired->auxBuffers)
            {
                missing += desired->auxBuffers - current->auxBuffers;
            }

            if (desired->samples > 0 && current->samples == 0)
            {
                // Technically, several multisampling buffers could be
                // involved, but that's a lower level implementation detail and
                // not important to us here, so we count them as one
                missing++;
            }

            if (desired->transparent != current->transparent)
                missing++;
        }

        // These polynomials make many small channel size differences matter
        // less than one large channel size difference

        // Calculate color channel size difference value
        {
            colorDiff = 0;

            if (desired->redBits != -1)
            {
                colorDiff += (desired->redBits - current->redBits) *
                             (desired->redBits - current->redBits);
            }

            if (desired->greenBits != -1)
            {
                colorDiff += (desired->greenBits - current->greenBits) *
                             (desired->greenBits - current->greenBits);
            }

            if (desired->blueBits != -1)
            {
                colorDiff += (desired->blueBits - current->blueBits) *
                             (desired->blueBits - current->blueBits);
            }
        }

        // Calculate non-color channel size difference value
        {
            extraDiff = 0;

            if (desired->alphaBits != -1)
            {
                extraDiff += (desired->alphaBits - current->alphaBits) *
                             (desired->alphaBits - current->alphaBits);
            }

            if (desired->depthBits != -1)
            {
                extraDiff += (desired->depthBits - current->depthBits) *
                             (desired->depthBits - current->depthBits);
            }

            if (desired->stencilBits != -1)
            {
                extraDiff += (desired->stencilBits - current->stencilBits) *
                             (desired->stencilBits - current->stencilBits);
            }

            if (desired->accumRedBits != -1)
            {
                extraDiff += (desired->accumRedBits - current->accumRedBits) *
                             (desired->accumRedBits - current->accumRedBits);
            }

            if (desired->accumGreenBits != -1)
            {
                extraDiff += (desired->accumGreenBits - current->accumGreenBits) *
                             (desired->accumGreenBits - current->accumGreenBits);
            }

            if (desired->accumBlueBits != -1)
            {
                extraDiff += (desired->accumBlueBits - current->accumBlueBits) *
                             (desired->accumBlueBits - current->accumBlueBits);
            }

            if (desired->accumAlphaBits != -1)
            {
                extraDiff += (desired->accumAlphaBits - current->accumAlphaBits) *
                             (desired->accumAlphaBits - current->accumAlphaBits);
            }

            if (desired->samples != -1)
            {
                extraDiff += (desired->samples - current->samples) *
                             (desired->samples - current->samples);
            }

            if (desired->sRGB && !current->sRGB)
                extraDiff++;
        }

        // Figure out if the current one is better than the best one found so far
        // Least number of missing buffers is the most important heuristic,
        // then color buffer size match and lastly size match for other buffers

        if (missing < leastMissing)
            closest = current;
        else if (missing == leastMissing)
        {
            if ((colorDiff < leastColorDiff) ||
                (colorDiff == leastColorDiff && extraDiff < leastExtraDiff))
            {
                closest = current;
            }
        }

        if (current == closest)
        {
            leastMissing = missing;
            leastColorDiff = colorDiff;
            leastExtraDiff = extraDiff;
        }
    }

    return closest;
}


// Util function for grabbing attributes from FBConfig object
//
static int getGLXFBConfigAttrib(GLXFBConfig fbconfig, int attrib)
{
    int value;
    glXGetFBConfigAttrib(structure->display->xDisplay, fbconfig, attrib, &value);
    return value;
}

// Return the GLXFBConfig most closely matching waht we have in our DI_FBConfig
static bool chooseGLXFBConfig(const DI_FBConfig* desired,
                                  GLXFBConfig* result)
{
    GLXFBConfig* nativeConfigs;
    DI_FBConfig* usableConfigs;
    const DI_FBConfig* closest;
    int nativeCount, usableCount;
    const char* vendor;
    bool trustWindowBit = true;

    nativeConfigs =
        glXGetFBConfigs(structure->display->xDisplay, structure->screen, &nativeCount);
    if (!nativeConfigs || !nativeCount)
    {
        std::cout << "chooseFLXFBConfig: glXGetFBConfigs failed, no configs found!";
        return false;
    }

    usableConfigs = (DI_FBConfig*)(calloc(nativeCount, sizeof(DI_FBConfig)));
    usableCount = 0;

    for (int i = 0;  i < nativeCount;  i++)
    {
        const GLXFBConfig n = nativeConfigs[i];
        DI_FBConfig* u = usableConfigs + usableCount;

        // Only consider RGBA GLXFBConfigs
        if (!(getGLXFBConfigAttrib(n, GLX_RENDER_TYPE) & GLX_RGBA_BIT))
            continue;

        // Only consider window GLXFBConfigs
        if (!(getGLXFBConfigAttrib(n, GLX_DRAWABLE_TYPE) & GLX_WINDOW_BIT))
        {
            if (trustWindowBit)
                continue;
        }

        if (getGLXFBConfigAttrib(n, GLX_DOUBLEBUFFER) != desired->doublebuffer)
            continue;

        if (desired->transparent)
        {
            XVisualInfo* vi = glXGetVisualFromFBConfig(structure->display->xDisplay, n);
            if (vi)
            {
                u->transparent = isVisualTransparent(vi->visual);
                XFree(vi);
            }
        }

        u->redBits = getGLXFBConfigAttrib(n, GLX_RED_SIZE);
        u->greenBits = getGLXFBConfigAttrib(n, GLX_GREEN_SIZE);
        u->blueBits = getGLXFBConfigAttrib(n, GLX_BLUE_SIZE);

        u->alphaBits = getGLXFBConfigAttrib(n, GLX_ALPHA_SIZE);
        u->depthBits = getGLXFBConfigAttrib(n, GLX_DEPTH_SIZE);
        u->stencilBits = getGLXFBConfigAttrib(n, GLX_STENCIL_SIZE);

        u->accumRedBits = getGLXFBConfigAttrib(n, GLX_ACCUM_RED_SIZE);
        u->accumGreenBits = getGLXFBConfigAttrib(n, GLX_ACCUM_GREEN_SIZE);
        u->accumBlueBits = getGLXFBConfigAttrib(n, GLX_ACCUM_BLUE_SIZE);
        u->accumAlphaBits = getGLXFBConfigAttrib(n, GLX_ACCUM_ALPHA_SIZE);

        u->auxBuffers = getGLXFBConfigAttrib(n, GLX_AUX_BUFFERS);

        if (getGLXFBConfigAttrib(n, GLX_STEREO))
            u->stereo = true;
        // Denotes support for the multi-sample buffer which performs antialising between
        // colors under the hood (application level does not need to handle it).
        u->samples = getGLXFBConfigAttrib(n, GLX_SAMPLES);
        // Denotes support for SRGB Color Scheme
        u->sRGB = getGLXFBConfigAttrib(n, GLX_FRAMEBUFFER_SRGB_CAPABLE_ARB);

        u->handle = (uintptr_t) n;
        usableCount++;
    }

    closest = diChooseFBConfig(desired, usableConfigs, usableCount);
    if (closest)
        *result = (GLXFBConfig) closest->handle;

    XFree(nativeConfigs);
    free(usableConfigs);

    return closest != NULL;
}


bool diCreateContextRudaX(DI_Window* window, DI_ContextConfig* ctxconfig, DI_FBConfig* fbconfig) {
    int attribs[40];
    GLXFBConfig native = NULL;
    GLXContext share = NULL;

    if (ctxconfig->share)
        share = *ctxconfig->share->rudaXContext;

    if (!chooseGLXFBConfig(fbconfig, &native))
    {
        std::cout << "diCreateContextRudaX: Failed to find appropriate GLXFB Config" << std::endl;
        return false;
    }

    // We don't need to worry about ctxConfigs, as they pretty much just allow us to choose
    // a specific version if we don't want to use the newest one. This is more
    // of a problem for older systems that want to have a specific version but need
    // newer functionality via backports. Our systems are pretty new so we can stick to
    // a singular ctxconfig that tells the system to work w/ the newest version of glx
    GLXContext ctx = glXCreateNewContext(structure->display->xDisplay,
                                              native,
                                              GLX_RGBA_TYPE,
                                              share,
                                              true);
    window->rudaXContext = &ctx;

    if (window->rudaXContext == nullptr || !window->rudaXContext) {
        std::cout << "diCreateContextRudaX: There was an error creating the rudaX context!" << std::endl;
        return false;
    }
    GLXWindow wnd = glXCreateWindow(structure->display->xDisplay, native, window->xWindow, NULL);
    window->rudaXWindow = &wnd;

    if (!window->rudaXWindow) {
        std::cout << "diCreateContextRudaX: There was an error creating the rudaX Window!"  << std::endl;
    }

    return true;
}

bool diInit() {
	structure = new DI_Structure(); //global structure referenced everywhere
    structure->display = new DI_Display(XOpenDisplay(NULL));
    structure->screen = DefaultScreen(structure->display);
    structure->ctx->xStore = XUniqueContext();
    structure->root = RootWindow(structure->display->xDisplay, structure->screen);
    structure->currentConfig = &defaultWindowConfig;
    
    // initialize variables such as
    // shouldClose
    // isMaximized
    // is Minimized
    // isFullScreen
    // defaultCursor
    // etc.

    // Initialize GLX after all variables have been initialized
    if (!structure->display || (structure->screen == -1) || !structure->root) {
        std::cout << structure->screen << std::endl;
        std::cout << structure->display << std::endl;
        std::cout << structure->root << std::endl;
        throw std::runtime_error("Display, Screen, and/or RootWindow not allocated correctly");
        return false;
    }
    return true;
};

DI_Structure* diGetStructure() {
    return structure;
}

int diWindowHint(unsigned short hint, int value) {
    return structure->currentConfig->diWindowHint(hint, value);
};

DI_Window* diCreateWindow(const str title) {
    return diCreateWindow(title, structure->currentConfig->width, structure->currentConfig->height, nullptr, nullptr);
};

DI_Window* diCreateWindow(const str title, unsigned int width, unsigned int height, DI_Monitor* monitor, DI_Window* parentWindow) {
    DI_ContextConfig ctxConfig;
    XVisual* visual = DefaultVisual(structure->display, structure->screen);
    int depth = DefaultDepth(structure->display, structure->screen);

    // Create Simple Window
    XWindow xWindow = XCreateSimpleWindow(structure->display->xDisplay, structure->root, structure->currentConfig->get_x_position(), structure->currentConfig->get_y_position(), width, height, 0, BlackPixel(structure->display->xDisplay, structure->screen), BlackPixel(structure->display->xDisplay, structure->screen));

    // Map Window to make it viewable

    
    XSelectInput(structure->display->xDisplay, xWindow, KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask);

    XMapWindow(structure->display->xDisplay, xWindow);
    XStoreName(structure->display->xDisplay, xWindow, title.c_str());
    XFlush(structure->display->xDisplay);

    
    // Do everything else in window constructor
    // such as:
    // create graphics context
    // set foreground
    
    structure->currentWindow = new DI_Window(xWindow, title, width, height, parentWindow);

        
    // Initialize our intermediary between Ruda and the X Window System
    if (!diCreateContextRudaX(structure->currentWindow, &ctxConfig, structure->fbConfig)) {
        std::cout << "Initialization of connection between X Window System and Ruda was not successful!" << std::endl;
    }

    // The reason why XSaveContext matters is because it allows us to FIND our DI_Window object on events
	// For instance, an XEvent will provide the xWindow where the event occured, but not the DI_Window
	// By saving a pointer to our DI_Window alongside the xWindow, we can just call XFindContext(xWindow) to return a pointer
	// to DI_Window
	if (XSaveContext(structure->display->xDisplay,
				xWindow,
				structure->ctx->xStore,
				(XPointer)structure->currentWindow) != 0) {
                    std::cout << "Error saving context" << std::endl;
                } 
    
    structure->windows.push_back(structure->currentWindow);
    return structure->currentWindow;
}
