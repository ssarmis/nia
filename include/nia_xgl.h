#ifdef __unix__

#ifndef _NIA_XGL_H_
#define _NIA_XGL_H_

#include <X11/X.h>
#include <X11/Xlib.h>

// #include "nia_gl.h"
#include <GL/glx.h>

#include "nia_general.h"

NIA_EXTERN NIA_CALL PFNGLXSWAPINTERVALEXTPROC _nia_glXSwapIntervalEXT;
                    
#define glXSwapIntervalEXT _nia_glXSwapIntervalEXT

NIA_CALL bool niaInitXGL();

#endif
#endif