/*
 * File: nia_wgl.cpp
 * Project: Nia
 * File created on October-20-2018
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifdef _WIN32

#include "nia_wgl.h"

PFNWGLSWAPINTERVALEXTPROC _nia_wglSwapIntervalEXT = NULL;

bool niaInitWGL(){
    bool extensionLoadCheck = true; 

    extensionLoadCheck = ((wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT")) && extensionLoadCheck);

    return extensionLoadCheck;
}

#endif