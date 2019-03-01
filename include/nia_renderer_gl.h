#ifdef NIA_RENDERER_GL

#ifndef _NIA_RENDERER_GL_WIN32_H_
#define _NIA_RENDERER_GL_WIN32_H_

#include <windows.h>

#include "nia_general.h"

#include "nia_mesh.h"
#include "nia_shader.h"
#include "nia_sprite.h"
#include "nia_rectangle.h"


NIA_CLASS niaRenderer{
public:
    niaShader shader;
    NIA_CALL niaRenderer();
    NIA_CALL ~niaRenderer();

    NIA_CALL void renderRectangle(r32 x, r32 y, r32 w, r32 h);
    NIA_CALL void renderRectangle(r32 x, r32 y, r32 w, r32 h, r32 colors[3]);

    NIA_CALL void renderMesh(const niaMesh& mesh);

    NIA_CALL void renderSprite(const niaSprite& sprite);
};

#endif //_NIA_RENDERER_GL_WIN32_H_
#endif //NIA_RENDERER_GL