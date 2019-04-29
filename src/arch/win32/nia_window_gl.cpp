#ifdef _WIN32

#ifdef NIA_RENDERER_GL

#ifndef UNICODE
#define UNICODE
#endif

#include "arch/win32/nia_window_gl.h"

#include "nia_gl.h"
#include "nia_wgl.h"

#include <tchar.h>

bool niaWindow::closed;
HDC niaWindow::deviceContext;
HGLRC niaWindow::glRenderContext;
HGLRC niaWindow::glRenderContextSecond;

NIA_CALL niaWindow::niaWindow(){
}

NIA_CALL niaWindow::~niaWindow(){
}

NIA_CALL void niaWindow::createWindow(u32 width, u32 height, const char* title){
    HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASS wc      = {0}; 
	wc.lpfnWndProc   = WndProc;
	wc.hInstance     = hInstance;
	wc.lpszClassName = L"Nia window";
	wc.style = CS_OWNDC;

	if( !RegisterClass(&wc) ){
        printf("Oopsie\n");
    }

	CreateWindowW(wc.lpszClassName, L"Nia window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, width, height, 0, 0, hInstance, 0);
    closed = false;
}

NIA_CALL void niaWindow::handleEvents(niaEvent& event){
    while (PeekMessageW(&event.msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&event.msg);
        DispatchMessageW(&event.msg);
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
                glRenderContextSecond = wglCreateContext(deviceContext);

                BOOL error = wglShareLists(glRenderContext, glRenderContextSecond);
                // if(error == FALSE){
                //     DWORD errorCode = GetLastError();
                //     LPVOID lpMsgBuf;
                //     FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                //         NULL, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR) &lpMsgBuf, 0, NULL);
                //     MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
                //     LocalFree(lpMsgBuf);
                //     wglDeleteContext(glRenderContextSecond);
                // }

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

NIA_CALL void niaWindow::swapBuffers(){
    SwapBuffers(deviceContext);
}

#endif //NIA_RENDERER_GL
#endif //_WIN32
