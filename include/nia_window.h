/*
 * File: nia_window.h
 * Project: Nia
 * File created on October-07-2018
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef _NIA_WINDOW_H_
#define _NIA_WINDOW_H_

#include "nia_event.h"
#include "nia_general.h"

#ifdef _WIN32
#   ifdef NIA_RENDERER_GL
#       include "arch/win32/nia_window_gl.h"
#   endif
#elif defined __unix__
#   ifdef NIA_RENDERER_GL
#       include "arch/linux/nia_window.h"
#   endif
#endif

#endif //_NIA_WINDOW_H_