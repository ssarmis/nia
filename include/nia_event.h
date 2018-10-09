#ifndef _NIA_EVENT_H_
#define _NIA_EVENT_H_

#ifdef _WIN32
#include "arch/win32/nia_event.h"
#elif defined __unix__
#include "arch/linux/nia_event.h"
#endif

#include "nia_general.h"

#endif //_NIA_EVENT_H_