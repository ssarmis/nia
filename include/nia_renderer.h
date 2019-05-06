#ifndef _NIA_RENDERER_H_
#define _NIA_RENDERER_H_

#include "nia_window.h"
#include "nia_general.h"

#include "renderers/nia_batch_renderer_gl.h"
#include "renderers/nia_font_renderer_gl.h"

#ifdef _WIN32
#   ifdef NIA_RENDERER_GL
#       include "renderers/nia_renderer_gl.h"
#   endif
#elif defined __unix__
#   ifdef NIA_RENDERER_GL
#       include "renderers/nia_renderer_gl.h"
#   endif
#endif

#endif //_NIA_RENDERER_H_