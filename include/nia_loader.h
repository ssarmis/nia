#ifndef _NIA_LOADER_H_
#define _NIA_LOADER_H_

#include <windows.h>

#include "nia_wgl.h"
#include "nia_gl.h"

NIA_INTERNAL void niaLoadEverything(){
    bool glLoad = niaInitGL();
    bool wglLoad = niaInitWGL();

    if(!(glLoad && wglLoad)){
        NIA_ERROR("Could not load all extensions, please update you drivers.\n");
    }
}

#endif // _NIA_LOADER_H_