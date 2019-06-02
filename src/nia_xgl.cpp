/*
 * File: nia_xgl.cpp
 * Project: Nia
 * File created on December-15-2018
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifdef __unix__

#include "nia_xgl.h"

PFNGLXSWAPINTERVALEXTPROC _nia_glXSwapIntervalEXT = NULL;

NIA_CALL bool niaInitXGL(){
    bool extensionLoadCheck = true; 

    extensionLoadCheck = ((glXSwapIntervalEXT = (PFNGLXSWAPINTERVALEXTPROC)glXGetProcAddress((const GLubyte*)"glXSwapIntervalEXT")) && extensionLoadCheck);

    return extensionLoadCheck;
}

#endif
