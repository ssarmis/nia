/*
 * File: nia_sprite.h
 * Project: Nia
 * File created on October-09-2018
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef _NIA_SPRITE_H_
#define _NIA_SPRITE_H_

#include "nia_general.h"
#include "nia_texture.h"
#include "nia_rectangle.h"
#include "nia_texture_structures.h"

NIA_CLASS niaSprite {
private:
    niaGlRectangle rectangle;
    u32 vbo;
    u32 veo;

    niaTexture texture;

public:
    NIA_CALL niaSprite();
    NIA_CALL niaSprite(u8* bitmap, u32 textureWidth, u32 textureHeight, const textureFormatDetails& details, const niaRectangle& rect);
    NIA_CALL niaSprite(const char* filename, const niaRectangle& rect);
    NIA_CALL ~niaSprite();

    void NIA_CALL loadTexture(const char* filename);
    void NIA_CALL loadTexture(u8* bitmap, u32 width, u32 height, const textureFormatDetails& details);
    void NIA_CALL setBounds(const niaRectangle& rect);
    // TODO make more versions of update texture, maybe we won't need to specify the texture details next time
    //      just the new data would be enough assuming the width and height are the same, the same goes
    //      for the textureFormatDetails.
    void NIA_CALL updateTexture(u8* bitmap, u32 width, u32 height, const textureFormatDetails& details);

    NIA_INLINE u32 NIA_CALL getWidth() const {
        return rectangle.w;
    }

    NIA_INLINE u32 NIA_CALL getHeight() const {
        return rectangle.h;
    }

    NIA_INLINE u32 NIA_CALL getVao() const { 
        return rectangle.vao;
    }

    NIA_INLINE u32 NIA_CALL getVeo() const {
        return veo;
    }
    
    NIA_INLINE u32 NIA_CALL getTextureId() {
        return texture.getTextureId();
    }

};

#endif //_NIA_SPRITE_H_