#ifdef __unix__

#include "arch/linux/nia_window.h"

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
XWindowAttributes       niaWindow::windowAttributes;

NIA_CALL niaWindow::niaWindow(){
}

NIA_CALL niaWindow::~niaWindow(){
}

NIA_CALL void niaWindow::createWindow(u32 width, u32 height, const char* title){
    display = XOpenDisplay(NULL);
    
    if(!display) {
        printf("\n\tcannot connect to X server\n\n");
    }

    rootWindow = DefaultRootWindow(display);

    visualInfo = glXChooseVisual(display, 0, attributes);

    if(visualInfo == NULL) {
        printf("\n\tno appropriate visual found\n\n");
    } else {
        printf("\n\tvisual %p selected\n", (void *)visualInfo->visualid);
    }

    colorMap = XCreateColormap(display, rootWindow, visualInfo->visual, AllocNone);

    setWindowAttributes.colormap = colorMap;
    setWindowAttributes.event_mask = ExposureMask | KeyPressMask;

    window = XCreateWindow(display, rootWindow, 0, 0, 600, 600, 0, visualInfo->depth, InputOutput,
        visualInfo->visual, CWColormap | CWEventMask, &setWindowAttributes);

    XMapWindow(display, window);

    XStoreName(display, window, title);

    openglContext = glXCreateContext(display, visualInfo, NULL, GL_TRUE);
    glXMakeCurrent(display, window, openglContext);

    closed = false;
}

NIA_CALL void niaWindow::handleEvents(niaEvent& event){
    XGetWindowAttributes(display, window, &windowAttributes);
    XNextEvent(display, &event.xEvent);
}

NIA_CALL bool niaWindow::isClosed() const {
    return closed;
}

#endif