/** @file nia_window_gl.h
  * @author Streanga Sarmis-Stefan
  * @brief
  */
#ifdef _WIN32

#ifdef NIA_RENDERER_GL

#ifndef _NIA_WINDOW_GL_WIN32_H_
#define _NIA_WINDOW_GL_WIN32_H_

#include <windows.h>

#include "nia_event.h"
#include "nia_general.h"

/**
 *  @class niaWindow
 *  @author Streanga Sarmis-Stefan
 *  @brief 
 * 
 * Win32 Window class
 */
NIA_CLASS niaWindow{
private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
    static bool closed;
    static HDC deviceContext;
    static HGLRC glRenderContext; 
    static HGLRC glRenderContextSecond;
    /**
     * @brief Constructor
     */
    niaWindow();

    /**
     * @brief Destructor
     */
    ~niaWindow();

    /**
     * @brief The function for creating a window and the opengl context 
     * @param width An unsigned value representing the window width.
     * @param height An unsigned value representing the window height.
     * @param title The title of the window.
     */
    void createWindow(u32 width, u32 height, const char* title); // additional flags later on

    /**
     * @brief The function for creating a window and the opengl context 
     * @param event An niaEvent instance used to store the current event message.
     */
	void handleEvents(niaEvent& event);

    /**
     * @brief Function for enabling adaptive vsync(if supported)
     */
    void enableAdaptiveVsync();

    /**
     * @brief Function for enabling vsync(if supported)
     */
    void enableVsync();

    /**
     * @brief Function for swapping the window buffers
     */
    void swapBuffers();

    /**
     * @brief Function that returns if the window is closed or not
     * @return If the window is closed
     */
    bool isClosed() const;

    friend class niaRenderer;
};

#endif //NIA_RENDERER_GL
#endif //_NIA_WINDOW_GL_WIN32_H_
#endif //_WIN32