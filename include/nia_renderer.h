#ifndef _NIA_RENDERER_H_
#define _NIA_RENDERER_H_

#include "nia_window.h"
#include "nia_general.h"

#ifdef _WIN32
#include "arch/win32/nia_renderer.h"
#elif defined __unix__
#include "arch/linux/nia_renderer.h"
#endif

#endif //_NIA_RENDERER_H_