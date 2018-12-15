#ifdef __unix__

#ifndef _NIA_XGL_H_
#define _NIA_XGL_H_

#include "nia_gl.h"
#include <GL/glxext.h>

#include "nia_general.h"

NIA_EXTERN NIA_CALL PFNXGLSWAPINTERVALEXTPROC _nia_xglSwapIntervalEXT;
#define xglSwapIntervalEXT _nia_xglSwapIntervalEXT

NIA_CALL bool niaInitXGL();

#endif
#endif