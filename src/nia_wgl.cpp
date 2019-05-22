#ifdef _WIN32

#include "nia_wgl.h"

PFNWGLSWAPINTERVALEXTPROC _nia_wglSwapIntervalEXT = NULL;

bool niaInitWGL(){
    bool extensionLoadCheck = true; 

    extensionLoadCheck = ((wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT")) && extensionLoadCheck);

    return extensionLoadCheck;
}

#endif