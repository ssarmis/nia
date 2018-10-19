#ifdef NIA_RENDERER_GL

#ifndef _NIA_BATCH_RENDERER_H_
#define _NIA_BATCH_RENDERER_H_

#include "nia_general.h"
#include "nia_sprite.h"

NIA_CLASS niaBatchRenderer{
public:
    NIA_CALL niaBatchRenderer();
    NIA_CALL ~niaBatchRenderer();

    NIA_CALL void renderRectangle(r32 x, r32 y, r32 w, r32 h);
    NIA_CALL void renderSprite(const niaSprite& sprite);
    NIA_CALL void renderTexture();

    NIA_CALL void executeRender();
};


#endif //_NIA_BATCH_RENDERER_H_
#endif //NIA_RENDERER_GL