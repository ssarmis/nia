#ifndef _NIA_WINDOW_H_
#define _NIA_WINDOW_H_

#include <windows.h>

#include "nia_event.h"
#include "nia_general.h"

NIA_CLASS niaWindow{
private:
    static bool closed;
#ifdef _WIN32
    static HDC deviceContext;
    static HGLRC glRenderContext; 
    static NIA_CALL LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 
#endif
    
public:
    NIA_CALL niaWindow();
    NIA_CALL ~niaWindow();

    NIA_CALL void createWindow(u32 width, u32 height, const char* title); // additional flags later on
    NIA_CALL void handleEvents(niaEvent& event);
    NIA_CALL bool isClosed() const;

    friend class niaRenderer;
};

#endif //_NIA_WINDOW_H_