#ifdef NIA_RENDERER_GL

#ifndef _NIA_RENDERER_GL_WIN32_H_
#define _NIA_RENDERER_GL_WIN32_H_

#include <windows.h>

#include "nia_mesh.h"
#include "nia_shader.h"
#include "nia_sprite.h"
#include "nia_matrix.h"
#include "nia_general.h"
#include "nia_transform.h"

NIA_CLASS niaRenderer {
protected:
    niaShader defaultShader;
    niaTexture defaultTexture;

public:
    NIA_CALL niaRenderer();
    NIA_CALL ~niaRenderer();

    NIA_CALL void enableDepthTest();

    NIA_CALL void renderRectangle(r32 x, r32 y, r32 w, r32 h); // TODO maybe make this virtual
    NIA_CALL void renderRectangle(r32 x, r32 y, r32 w, r32 h, r32 colors[3]);
    NIA_CALL void renderRectangle(r32 x, r32 y, r32 z, r32 w, r32 h, r32 colors[3]);

    NIA_CALL void submitTransformation(const niaTransform& transformation, bool transpose = true);
    NIA_CALL void submitView(const niaMatrix4& view, bool transpose = true);

    NIA_CALL void pushOrthographicView(r32 left, r32 right, r32 top, r32 bottom, r32 near, r32 far); // future, custom matrix
    NIA_CALL void pushPerspectiveView(r32 fov, r32 aspectRatio, r32 near, r32 far);
    // TODO add texture support for rectangles as well, maybe they will be sprites
    NIA_CALL void renderMesh(const niaMesh& mesh);
    NIA_CALL void renderMesh(const niaMesh& mesh, const niaTexture& texture);

    NIA_CALL void renderSprite(const niaSprite& sprite);
    
    friend class niaScene;
};


#endif //_NIA_RENDERER_GL_WIN32_H_
#endif //NIA_RENDERER_GL