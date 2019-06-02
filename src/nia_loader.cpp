/*
 * File: nia_loader.cpp
 * Project: Nia
 * File created on April-29-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

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
// #ifdef _WIN32
    niaTextureStreaming::initializeStream();
// #endif
}