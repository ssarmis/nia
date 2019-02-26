#ifdef NIA_RENDERER_GL

#ifndef _NIA_RENDERER_GL_WIN32_H_
#define _NIA_RENDERER_GL_WIN32_H_

#include <windows.h>

#include "nia_general.h"

#include "nia_shader.h"
#include "nia_sprite.h"
#include "nia_rectangle.h"

#define NIA_VERTEX_STRIDE   0
#define NIA_COLOR_STRIDE    (3 * sizeof(r32))

NIA_STRUCT niaVertex{
    union {
        r32 attrs[6];
        struct {
            union {
                r32 vert[3];
                struct {
                    r32 x;
                    r32 y;
                    r32 z;
                };
            };

            union {
                r32 cols[3];
                struct {
                    r32 r;
                    r32 g;
                    r32 b;
                };
            };
        };
    };
} niaVertex;

NIA_CLASS niaRenderer{
public:
    niaShader shader;
    NIA_CALL niaRenderer();
    NIA_CALL ~niaRenderer();

    NIA_CALL void renderRectangle(r32 x, r32 y, r32 w, r32 h);
    NIA_CALL void renderRectangle(r32 x, r32 y, r32 w, r32 h, r32 colors[3]);
    NIA_CALL void renderSprite(const niaSprite& sprite);
};

#endif //_NIA_RENDERER_GL_WIN32_H_
#endif //NIA_RENDERER_GL