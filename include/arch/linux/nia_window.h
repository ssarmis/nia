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
    static bool closed;
    static Display*                display;
    static GLint                   attributes[5];
    static XVisualInfo*            visualInfo;
    static Colormap                colorMap;
    static XSetWindowAttributes    setWindowAttributes;
    static Window                  rootWindow;
    static Window                  window;
    static GLXContext              openglContext;
    static XWindowAttributes       windowAttributes;
    static XEvent                  event;

public:
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