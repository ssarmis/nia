#ifndef _NIA_SPRITE_H_
#define _NIA_SPRITE_H_

#include "nia_general.h"
#include "nia_texture.h"
#include "nia_rectangle.h"

NIA_CLASS niaSprite {
private:
    niaGlRectangle rectangle;
    u32 vbo;
    u32 veo;

    niaTexture texture;

public:
    NIA_CALL niaSprite();
    NIA_CALL niaSprite(const char* filename, const niaRectangle& rect);
    NIA_CALL ~niaSprite();

    void NIA_CALL loadTexture(const char* filename);
    void NIA_CALL setBounds(const niaRectangle& rect);

    NIA_INLINE u32 NIA_CALL getVao() { 
        return rectangle.vao;
    }

    NIA_INLINE u32 NIA_CALL getVeo() {
        return veo;
    }
    
    NIA_INLINE u32 NIA_CALL getTextureId() {
        return texture.getTextureId();
    }
};

#endif //_NIA_SPRITE_H_