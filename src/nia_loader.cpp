#include "nia_loader.h"
#include "nia_texture_streaming.h"

#include "nia_gl.h"

#ifdef _WIN32
#   include <windows.h>
#   include "nia_wgl.h"
#elif defined __unix__
#   include "nia_xgl.h"
#endif


void niaLoader::loadEverything(){
    bool glLoad = niaInitGL();
#ifdef _WIN32
    bool extLoad = niaInitWGL();
#elif defined __unix__
    bool extLoad = niaInitXGL();
#endif

    if(!(glLoad && extLoad)){
        NIA_ERROR("Could not load all extensions, please update you drivers.\n");
    }
#ifdef _WIN32
    niaTextureStreaming::initializeStream();
#endif
}