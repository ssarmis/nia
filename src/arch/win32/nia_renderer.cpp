#ifdef _WIN32
#include "nia_renderer.h"

NIA_CALL niaRenderer::niaRenderer(){
}

NIA_CALL niaRenderer::~niaRenderer(){
}

NIA_CALL void niaRenderer::swapBuffers(const niaWindow& window){
    SwapBuffers(window.deviceContext);
}
#endif
