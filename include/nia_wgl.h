#ifdef _WIN32

#ifndef _NIA_WGL_H_
#define _NIA_WGL_H_

#include <windows.h>

#include "nia_gl.h"
#include <GL/wglext.h>

#include "nia_general.h"

NIA_EXTERN NIA_CALL PFNWGLSWAPINTERVALEXTPROC _nia_wglSwapIntervalEXT;
#define wglSwapIntervalEXT _nia_wglSwapIntervalEXT

NIA_CALL bool niaInitWGL();

#endif // _NIA_WGL_H_
#endif