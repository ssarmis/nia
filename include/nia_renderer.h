#ifndef _NIA_RENDERER_H_
#define _NIA_RENDERER_H_

#include "nia_window.h"
#include "nia_general.h"

#include "nia_batch_renderer_gl.h"

#ifdef _WIN32
#   ifdef NIA_RENDERER_GL
#       include "nia_renderer_gl.h"
#   endif
#elif defined __unix__
#   ifdef NIA_RENDERER_GL
#       include "nia_renderer.h"
#   endif
#endif

#endif //_NIA_RENDERER_H_