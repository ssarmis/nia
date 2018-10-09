#ifndef _NIA_SPRITE_H_
#define _NIA_SPRITE_H_

#include "nia_general.h"
#include "nia_texture.h"

NIA_CLASS niaSprite {
public:
    int width;
    int height;
    int x;
    int y;

    niaTexture texture;

    NIA_CALL niaSprite();
    NIA_CALL ~niaSprite();
};

#endif //_NIA_SPRITE_H_