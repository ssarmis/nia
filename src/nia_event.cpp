/*
 * File: nia_event.cpp
 * Project: Nia
 * File created on October-07-2018
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

// TODO put in some win32 arch file
#ifdef _WIN32

#define WM_MOUSEHWHEEL                  0x020E

#include "nia_event.h"

niaEvent::niaEvent(){
}

niaEvent::~niaEvent(){
}

#include <stdio.h>//remove
bool niaEvent::processMessageReceived(){
    switch (msg.message) { 
        // TODO split this maybe in 2 cases, not just one
        case WM_KEYDOWN: case WM_KEYUP: case WM_CHAR: {
                keyboard.activeKey = msg.wParam;
                if(keyboard.lastKeyboardEvent == WM_KEYDOWN && msg.message == WM_KEYUP){
                    if(keyboard.oldActiveKey == keyboard.activeKey){
                        keyCompletePress[keyboard.activeKey] = true;
                    }
                } else {
                    if(keyboard.lastKeyboardEvent == WM_KEYDOWN){
                        eventType = NIA_KEY_PRESSED;
                        keyCompletePress[keyboard.oldActiveKey] = false;
                    }

                }

                if(msg.message == WM_KEYDOWN){
                    eventType = NIA_KEY_PRESSED;
                }else if(msg.message == WM_KEYUP){
                    eventType = NIA_KEY_RELEASED;
                }

                keyboard.oldActiveKey = msg.wParam;
                keyboard.lastKeyboardEvent = msg.message;
            }
            break;

        case WM_MOUSEMOVE: {
                mouse.x = (msg.lParam & 0x0000FFFF);
                mouse.y = ((msg.lParam & 0xFFFF0000) >> 16);
            }
            break;

        case WM_LBUTTONDOWN: case WM_LBUTTONUP: {
                mouse.leftMouseClicked = (msg.message == WM_LBUTTONDOWN);
            }
            break;

        case WM_RBUTTONDOWN: case WM_RBUTTONUP: {
                mouse.rightMouseClicked = (msg.message == WM_RBUTTONDOWN);
            }
            break;

        case WM_MOUSEHWHEEL: {
                mouse.scrollX = (msg.lParam & 0x0000FFFF);
                mouse.scrollY = ((msg.lParam & 0xFFFF0000) >> 16);
            }
            break;

        default:{
                eventType = NIA_EVENT_NONE;
                return false;
            }
            break;
    }
    return true;
}

#endif