#ifdef __unix__

#include "nia_xgl.h"

PFNXGLSWAPINTERVALEXTPROC _nia_xglSwapIntervalEXT = NULL;

NIA_CALL bool niaInitXGL(){
    bool extensionLoadCheck = true; 

    extensionLoadCheck = ((xglSwapIntervalEXT = (PFNXGLSWAPINTERVALEXTPROC)glXGetProcAddress("xglSwapIntervalEXT")) && extensionLoadCheck);

    return extensionLoadCheck;
}

#endif
