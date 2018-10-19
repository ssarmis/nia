#ifdef _WIN32

#ifdef NIA_RENDERER_GL

#ifndef UNICODE
#define UNICODE
#endif

#include "arch/win32/nia_window_gl.h"

#include <GL/gl.h>
#include "nia_wgl.h"

#include <tchar.h>

bool niaWindow::closed;
HDC niaWindow::deviceContext;
HGLRC niaWindow::glRenderContext; 

NIA_CALL niaWindow::niaWindow(){
}

NIA_CALL niaWindow::~niaWindow(){
}

NIA_CALL void niaWindow::createWindow(u32 width, u32 height, const char* title){
    HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASS wc      = {0}; 
	wc.lpfnWndProc   = WndProc;
	wc.hInstance     = hInstance;
	wc.lpszClassName = L"oglversionchecksample";
	wc.style = CS_OWNDC;

	if( !RegisterClass(&wc) ){
        printf("Oopsie\n");
    }

	CreateWindowW(wc.lpszClassName, L"openglversioncheck", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, width, height, 0, 0, hInstance, 0);
    closed = false;
}

NIA_CALL void niaWindow::handleEvents(niaEvent& event){
    if(GetMessage(&event.msg, NULL, 0, 0) > 0){
        DispatchMessage(&event.msg);
    }
}

NIA_CALL void niaWindow::enableAdaptiveVsync(){
    if(wglSwapIntervalEXT){
        wglSwapIntervalEXT(-1);
    } else {
        NIA_TRACE("Vsync is not supported!\n");
    }
}

NIA_CALL void niaWindow::enableVsync(){
    if(wglSwapIntervalEXT){
        wglSwapIntervalEXT(1);
    } else {
        NIA_TRACE("Vsync is not supported!\n");
    }
}

NIA_CALL bool niaWindow::isClosed() const {
    return closed;
}

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
                
                int arrtributes[] = {
                    WGL_ARB_extensions_string,
                    WGL_ARB_pixel_format,
                    WGL_ARB_make_current_read
                };
                
                glRenderContext = wglCreateContext(deviceContext);

                if (!wglMakeCurrent(deviceContext, glRenderContext)){
                    NIA_ERROR("Failed to create context, return with error code %d\n", GetLastError());
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

#endif //NIA_RENDERER_GL
#endif //_WIN32
