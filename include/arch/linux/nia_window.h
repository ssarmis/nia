/*
 * File: nia_window.h
 * Project: Nia
 * File created on October-08-2018
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifdef __unix__

#ifndef _NIA_WINDOW_LINUX_H_
#define _NIA_WINDOW_LINUX_H_

#include <X11/X.h>
#include <X11/Xlib.h>

#include <GL/gl.h>
#include <GL/glx.h>
#include <GL/glu.h>

#include "nia_event.h"
#include "nia_general.h"

NIA_CLASS niaWindow{
private:
    static GLint                   attributes[5];
    static XVisualInfo*            visualInfo;
    static Colormap                colorMap;
    static XSetWindowAttributes    setWindowAttributes;
    static Window                  rootWindow;
    static XWindowAttributes       windowAttributes;
    static XEvent                  event;

public:
    static bool closed;
    static Window                  window;
    static Display*                display;
    static GLXContext              openglContext;
    static GLXContext              openglContextSecond;

    NIA_CALL niaWindow();
    NIA_CALL ~niaWindow();

    NIA_CALL void createWindow(u32 width, u32 height, const char* title); // additional flags later on
    NIA_CALL void handleEvents(niaEvent& event);
    NIA_CALL void enableAdaptiveVsync();
    NIA_CALL void enableVsync();
    NIA_CALL void swapBuffers();
    NIA_CALL bool isClosed() const;

    friend class niaRenderer;
};

#endif //_NIA_WINDOW_LINUX_H_

#endif