/*
 * File: nia_window_gl.cpp
 * Project: Nia
 * File created on October-09-2018
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

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

niaWindow::niaWindow(){
}

niaWindow::~niaWindow(){
}

wchar_t* char2wchar(const char* string){
    wchar_t* result = new wchar_t[nia_strlen(string) + 1];
    wchar_t* tmp = result;

    while(*string){
        *(tmp++) = *(string++);
    }

    result[nia_strlen(string)] = 0;
    return result;
}

void niaWindow::createWindow(u32 width, u32 height, const char* title){
    HINSTANCE hInstance = GetModuleHandle(NULL);
	WNDCLASS wc      = {0}; 
	wc.lpfnWndProc   = WndProc;
	wc.hInstance     = hInstance;
	wc.lpszClassName = L"Something";
	wc.style = CS_OWNDC;

	if(!RegisterClass(&wc)){
        NIA_TRACE("Oopsie\n");
    }
// (const wchar_t*)char2wchar(title)
	CreateWindowW(wc.lpszClassName, L"Something", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, width, height, 0, 0, hInstance, 0);
    closed = false;
}

void niaWindow::handleEvents(niaEvent& event){
    while (PeekMessageW(&event.msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&event.msg);
        DispatchMessageW(&event.msg);
    }
}

void niaWindow::enableAdaptiveVsync(){
    if(wglSwapIntervalEXT){
        wglSwapIntervalEXT(-1);
    } else {
        NIA_TRACE("Vsync is not supported!\n");
    }
}

void niaWindow::enableVsync(){
    if(wglSwapIntervalEXT){
        wglSwapIntervalEXT(1);
    } else {
        NIA_TRACE("Vsync is not supported!\n");
    }
}

bool niaWindow::isClosed() const {
    return closed;
}

LRESULT CALLBACK niaWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
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
                
				if (!wglMakeCurrent(deviceContext, glRenderContext)){
                    NIA_ERROR("Failed to create context, return with error code %d\n", GetLastError());
                }
            }
            break;
        case WM_DESTROY:{
                wglMakeCurrent(NULL, NULL); 
                wglDeleteContext(glRenderContext);
                PostQuitMessage(0);
                closed = true;
            }
            break;  
    }  
    return DefWindowProc(hWnd, message, wParam, lParam);  
}  

void niaWindow::swapBuffers(){
    SwapBuffers(deviceContext);
}

#endif //NIA_RENDERER_GL
#endif //_WIN32
