/*
 * File: nia_event.h
 * Project: Nia
 * File created on October-08-2018
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifdef _WIN32

#ifndef _NIA_EVENT_WIN32_H_
#define _NIA_EVENT_WIN32_H_

#include <windows.h>

#include "nia_general.h"

NIA_CLASS niaEvent {
public:
    NIA_CALL niaEvent();
    NIA_CALL ~niaEvent();

    MSG msg;
};

#endif //_NIA_EVENT_WIN32_H_

#endif