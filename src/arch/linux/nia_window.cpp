#ifdef __unix__

#include "arch/linux/nia_window.h"

#include "nia_xgl.h"

#include <stdio.h>

bool                    niaWindow::closed;
Display*                niaWindow::display;
GLint                   niaWindow::attributes[5] = {GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};
XVisualInfo*            niaWindow::visualInfo;
Colormap                niaWindow::colorMap;
XSetWindowAttributes    niaWindow::setWindowAttributes;
Window                  niaWindow::rootWindow;
Window                  niaWindow::window;
GLXContext              niaWindow::openglContext;
GLXContext              niaWindow::openglContextSecond;
XWindowAttributes       niaWindow::windowAttributes;

NIA_CALL niaWindow::niaWindow(){
}

NIA_CALL niaWindow::~niaWindow(){
}

NIA_CALL void niaWindow::createWindow(u32 width, u32 height, const char* title){
    display = XOpenDisplay(NULL);
    
    if(!display) {
        printf("Cannot connect to X server\n");
    }

    rootWindow = DefaultRootWindow(display);

    visualInfo = glXChooseVisual(display, 0, attributes);

    if(visualInfo == NULL) {
        printf("No appropriate visual found\n");
    }

    colorMap = XCreateColormap(display, rootWindow, visualInfo->visual, AllocNone);

    setWindowAttributes.colormap = colorMap;
    setWindowAttributes.event_mask = ExposureMask | KeyPressMask;

    window = XCreateWindow(display, rootWindow, 0, 0, width, height, 0, visualInfo->depth, InputOutput,
        visualInfo->visual, CWColormap | CWEventMask, &setWindowAttributes);

    XMapWindow(display, window);

    XStoreName(display, window, title);

    openglContext = glXCreateContext(display, visualInfo, NULL, GL_TRUE);
    openglContextSecond = glXCreateContext(display, visualInfo, NULL, GL_TRUE);

    glXMakeCurrent(display, window, openglContext);

    closed = false;
}

NIA_CALL void niaWindow::enableAdaptiveVsync(){
    if(glXSwapIntervalEXT){
        glXSwapIntervalEXT(display, window, -1);
    }
}

NIA_CALL void niaWindow::enableVsync(){
    if(glXSwapIntervalEXT){
        glXSwapIntervalEXT(display, window, 1);
    }
}

NIA_CALL void niaWindow::handleEvents(niaEvent& event){
    XGetWindowAttributes(display, window, &windowAttributes);
    // XNextEvent(display, &event.xEvent);
    XPeekEvent(display, &event.xEvent);
}

NIA_CALL bool niaWindow::isClosed() const {
    return closed;
}

NIA_CALL void niaWindow::swapBuffers(){
    glXSwapBuffers(display, window);
}

#endif
