#ifndef UNICODE
#define UNICODE
#endif
#include "nia_window.h"

#include <stdio.h>
#include <tchar.h>

bool niaWindow::closed;

#ifdef _WIN32
    HDC niaWindow::deviceContext;
    HGLRC niaWindow::glRenderContext; 
#endif

NIA_CALL niaWindow::niaWindow(){
}

NIA_CALL niaWindow::~niaWindow(){
}

NIA_CALL void niaWindow::createWindow(u32 width, u32 height, const char* title){
#ifdef _WIN32
    HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASS wc      = {0}; 
	wc.lpfnWndProc   = WndProc;
	wc.hInstance     = hInstance;
	wc.lpszClassName = L"oglversionchecksample";
	wc.style = CS_OWNDC;

	if( !RegisterClass(&wc) ){
        printf("Oopsie\n");
    }

	CreateWindowW(wc.lpszClassName, L"openglversioncheck", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 640, 480, 0, 0, hInstance, 0);
    closed = false;
    // while(GetMessage( &msg, NULL, 0, 0 ) > 0){
    // }
#endif
}

NIA_CALL void niaWindow::handleEvents(niaEvent& event){
#ifdef _WIN32
    if(GetMessage(&event.msg, NULL, 0, 0) > 0){
        DispatchMessage(&event.msg);
    }
#endif
}

NIA_CALL bool niaWindow::isClosed() const {
    return closed;
}

#ifdef _WIN32
NIA_CALL LRESULT CALLBACK niaWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {  
    switch (message) { 
        case WM_CREATE:{
                PIXELFORMATDESCRIPTOR pfd = {
                    sizeof(PIXELFORMATDESCRIPTOR),
                    1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
                    PFD_TYPE_RGBA, 32,                   
                    0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0,
                    24, 8, 0, PFD_MAIN_PLANE,
                    0, 0, 0, 0
                };

                deviceContext = GetDC(hWnd);
                int PixelFormat = ChoosePixelFormat(deviceContext, &pfd); 
                
                SetPixelFormat(deviceContext, PixelFormat, &pfd);

                glRenderContext = wglCreateContext(deviceContext);

                if (!wglMakeCurrent(deviceContext, glRenderContext)){
                    fprintf(stderr, "Failed to create context, return with error code %d\n", GetLastError());
                }
            }
            break;
        case WM_DESTROY:{
                printf("Exiting...\n");
                wglMakeCurrent(NULL, NULL); 
                wglDeleteContext(glRenderContext);
                PostQuitMessage(0);
                closed = true;
            }
            break;  
    }  
    return DefWindowProc(hWnd, message, wParam, lParam);  
}  
#endif
