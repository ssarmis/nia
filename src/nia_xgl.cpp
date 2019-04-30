#ifdef __unix__

#include "nia_xgl.h"

PFNGLXSWAPINTERVALEXTPROC _nia_glXSwapIntervalEXT = NULL;

NIA_CALL bool niaInitXGL(){
    bool extensionLoadCheck = true; 

    extensionLoadCheck = ((glXSwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC)glXGetProcAddress("glXSwapIntervalEXT")) && extensionLoadCheck);

    return extensionLoadCheck;
}

#endif
