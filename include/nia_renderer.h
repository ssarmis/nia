#ifndef _NIA_RENDERER_H_
#define _NIA_RENDERER_H_

#include "nia_window.h"
#include "nia_general.h"

#ifdef _WIN32
#   ifdef NIA_RENDERER_GL
#       include "arch/win32/nia_renderer_gl.h"
#   endif
#elif defined __unix__
#   ifdef NIA_RENDERER_GL
#       include "arch/linux/nia_renderer.h"
#   endif
#endif

#endif //_NIA_RENDERER_H_