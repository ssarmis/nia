#ifdef _WIN32

#ifdef NIA_RENDERER_GL

#ifndef _NIA_RENDERER_GL_WIN32_H_
#define _NIA_RENDERER_GL_WIN32_H_

#include <windows.h>

#include "nia_general.h"
#include "nia_sprite.h"

NIA_CLASS niaRenderer{
public:
    NIA_CALL niaRenderer();
    NIA_CALL ~niaRenderer();

    NIA_CALL void renderRectangle(r32 x, r32 y, r32 w, r32 h);
    NIA_CALL void renderSprite(const niaSprite& sprite);
    NIA_CALL void renderTexture();

    NIA_CALL void executeRender();
};

#endif //_NIA_RENDERER_GL_WIN32_H_
#endif //NIA_RENDERER_GL
#endif //_WIN32