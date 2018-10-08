#ifdef __unix__

#ifndef _NIA_EVENT_LINUX_H_
#define _NIA_EVENT_LINUX_H_

#include <X11/X.h>
#include <X11/Xlib.h>

#include "nia_general.h"

NIA_CLASS niaEvent {
public:
    NIA_CALL niaEvent();
    NIA_CALL ~niaEvent();

    XEvent xEvent;
};

#endif //_NIA_EVENT_LINUX_H_

#endif //__unix__