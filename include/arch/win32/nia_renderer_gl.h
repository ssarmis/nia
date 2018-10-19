#ifdef _WIN32

#ifdef NIA_RENDERER_GL

#ifndef _NIA_RENDERER_GL_WIN32_H_
#define _NIA_RENDERER_GL_WIN32_H_

#include <windows.h>

#include "nia_general.h"
#include "nia_sprite.h"

#include "arch/win32/nia_window_gl.h"

NIA_CLASS niaRenderable{
    union {
        float attrs[6];
        struct {
            union {
                float vert[3];
                struct {
                    float x;
                    float y;
                    float z;
                };
            };

            union {
                float cols[3];
                struct {
                    float r;
                    float g;
                    float b;
                };
            };
        };
    };
};

NIA_CLASS niaRenderer{
public:
    NIA_CALL niaRenderer();
    NIA_CALL ~niaRenderer();

    void renderSprite(const niaSprite& sprite);
    void renderTexture();

    NIA_CALL void swapBuffers(const niaWindow& window);
};

#endif //_NIA_RENDERER_GL_WIN32_H_
#endif //NIA_RENDERER_GL
#endif //_WIN32