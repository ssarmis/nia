#ifndef _NIA_WINDOW_H_
#define _NIA_WINDOW_H_

#include "nia_event.h"
#include "nia_general.h"

#ifdef _WIN32
#   ifdef NIA_RENDERER_GL
#       include "arch/win32/nia_window_gl.h"
#   endif
#elif defined __unix__
#   ifdef NIA_RENDERER_GL
#       include "arch/linux/nia_window.h"
#   endif
#endif

#endif //_NIA_WINDOW_H_