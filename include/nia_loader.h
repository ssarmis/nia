#ifndef _NIA_LOADER_H_
#define _NIA_LOADER_H_

#include <windows.h>

#ifdef _WIN32
#   include "nia_wgl.h"
#elif defined __unix__
#   include "nia_xgl.h"
#endif

#include "nia_gl.h"

NIA_INTERNAL void niaLoadEverything(){
    bool glLoad = niaInitGL();
#ifdef _WIN32
    bool extLoad = niaInitWGL();
#elif defined __unix__
    bool extLoad = niaInitXGL();
#endif

    if(!(glLoad && extLoad)){
        NIA_ERROR("Could not load all extensions, please update you drivers.\n");
    }
}

#endif // _NIA_LOADER_H_