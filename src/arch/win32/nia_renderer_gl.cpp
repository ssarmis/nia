#ifdef _WIN32

#ifdef NIA_RENDERER_GL

#include "arch/win32/nia_renderer_gl.h"

NIA_CALL niaRenderer::niaRenderer(){
}

NIA_CALL niaRenderer::~niaRenderer(){
}

NIA_CALL void niaRenderer::swapBuffers(const niaWindow& window){
    SwapBuffers(window.deviceContext);
}

#endif
#endif
