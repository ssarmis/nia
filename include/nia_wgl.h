/*
 * File: nia_wgl.h
 * Project: Nia
 * File created on October-17-2018
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifdef _WIN32

#ifndef _NIA_WGL_H_
#define _NIA_WGL_H_

#include <windows.h>

#include "nia_gl.h"
#include "arch/win32/khronos/wglext.h"

#include "nia_general.h"

NIA_EXTERN PFNWGLSWAPINTERVALEXTPROC _nia_wglSwapIntervalEXT;
#define wglSwapIntervalEXT _nia_wglSwapIntervalEXT

bool NIA_CALL niaInitWGL();

#endif // _NIA_WGL_H_
#endif