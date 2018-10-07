#ifndef _NIA_RENDERER_H_
#define _NIA_RENDERER_H_

#include <windows.h>

#include "nia_window.h"
#include "nia_general.h"

#ifdef __WIN32__
#include "arch/win32/nia_renderer.h"
#elif __unix__
#endif

#endif //_NIA_RENDERER_H_