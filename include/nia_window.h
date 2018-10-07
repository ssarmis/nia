#ifndef _NIA_WINDOW_H_
#define _NIA_WINDOW_H_

#include <windows.h>

#include "nia_event.h"
#include "nia_general.h"

#ifdef __WIN32__
#include "arch/win32/nia_window.h"
#elif __unix__
#endif

#endif //_NIA_WINDOW_H_