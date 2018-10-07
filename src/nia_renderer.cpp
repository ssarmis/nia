#include "nia_renderer.h"

NIA_CALL niaRenderer::niaRenderer(){
}

NIA_CALL niaRenderer::~niaRenderer(){
}

NIA_CALL void niaRenderer::swapBuffers(const niaWindow& window){
#ifdef _WIN32
    SwapBuffers(window.deviceContext);
#endif
}
