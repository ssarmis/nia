#ifdef _WIN32

#ifndef _NIA_EVENT_WIN32_H_
#define _NIA_EVENT_WIN32_H_

#include <windows.h>

#include "nia_general.h"

NIA_CLASS niaEvent {
public:
    NIA_CALL niaEvent();
    NIA_CALL ~niaEvent();

    MSG msg;
};

#endif //_NIA_EVENT_WIN32_H_

#endif