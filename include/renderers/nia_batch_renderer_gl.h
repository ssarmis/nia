#ifdef NIA_RENDERER_GL

#ifndef _NIA_BATCH_RENDERER_H_
#define _NIA_BATCH_RENDERER_H_

#include "nia_general.h"

#include "nia_sprite.h"
#include "nia_rectangle.h"
#include "nia_vector.h"
#include "renderers/nia_renderer_gl.h"

NIA_CLASS niaBatchRenderer : public niaRenderer {
public:
    niaBatchRenderer();
    ~niaBatchRenderer();

    void renderRectangle(r32 x, r32 y, r32 w, r32 h, r32 colors[3]);
    void renderRectangle(r32 x, r32 y, r32 w, r32 h, const niaVec3f& colors);
    void renderRectangle(r32 x, r32 y, r32 w, r32 h);
    void renderSprite(const niaSprite& sprite);

    void executeRender();
};


#endif //_NIA_BATCH_RENDERER_H_
#endif //NIA_RENDERER_GL