#ifndef _NIA_WGL_H_
#define _NIA_WGL_H_

#include <windows.h>

#include <GL/wglext.h>

#include "nia_general.h"

NIA_CALL PFNWGLSWAPINTERVALEXTPROC _nia_wglSwapIntervalEXT;
#define wglSwapIntervalEXT _nia_wglSwapIntervalEXT

bool niaInitWGL(){
    bool extensionLoadCheck = true; 

    extensionLoadCheck = ((wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT")) && extensionLoadCheck);

    return extensionLoadCheck;
}


#endif // _NIA_WGL_H_