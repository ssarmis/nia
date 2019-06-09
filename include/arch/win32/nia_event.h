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
#include "nia_vector.h"

NIA_STRUCT keyboardDetails {
    u32 activeKey;
    u32 oldActiveKey;
    u32 lastKeyboardEvent;
} keyboardDetails;

NIA_STRUCT mouseDetails {
    bool leftMouseClicked;
    bool rightMouseClicked;
    i32 x;
    i32 y;
} mouseDetails;

NIA_CLASS niaEvent {
private:
    // TODO implement this array into detecting when a key was pressd nicely
    bool keyCompletePress[256];
    u32 eventType;
    keyboardDetails keyboard;
    mouseDetails mouse;

public:
    NIA_CALL niaEvent();
    NIA_CALL ~niaEvent();

    NIA_INLINE u32 NIA_CALL getEventType() const {
        return eventType;
    }

    NIA_INLINE u32 NIA_CALL getActiveKey(){
        return keyboard.activeKey;
    }

    // returns the last keyt that was pressed completely
    NIA_INLINE bool NIA_CALL getCompleteKeyPress(u32 key){
        return keyCompletePress[key];
    }

    NIA_INLINE i32 NIA_CALL getMouseX(){
        return mouse.x;
    }

    NIA_INLINE i32 NIA_CALL getMouseY(){
        return mouse.y;
    }

    NIA_INLINE bool NIA_CALL getMouseLeftButton(){
        return mouse.leftMouseClicked;
    }

    NIA_INLINE bool NIA_CALL getMouseRightButton(){
        return mouse.rightMouseClicked;
    }

    bool NIA_CALL processMessageReceived();

    MSG msg;
};

#endif //_NIA_EVENT_WIN32_H_

#endif