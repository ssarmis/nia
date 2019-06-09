/*
 * File: nia_event.h
 * Project: Nia
 * File created on October-07-2018
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef _NIA_EVENT_H_
#define _NIA_EVENT_H_

#define NIA_EVENT_NONE   0xff

#define NIA_KEY_RELEASED 0
#define NIA_KEY_PRESSED  1

#ifdef _WIN32
#define NIA_SPECIAL_KEY(_key) VK_##_key
#define NIA_CHARACTER_KEY(_key) (#_key)[0]
#elif defined __unix__
#define NIA_KEY(_key) _key
#endif

#ifdef _WIN32
#   include "arch/win32/nia_event.h"
#elif defined __unix__
#   include "arch/linux/nia_event.h"
#endif

#include "nia_general.h"

#endif //_NIA_EVENT_H_