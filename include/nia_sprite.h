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

/** @file nia_sprite.h
  * @author Streanga Sarmis-Stefan
  * @brief
  * 
  */
#ifndef _NIA_SPRITE_H_
#define _NIA_SPRITE_H_

#include "nia_general.h"
#include "nia_texture.h"
#include "nia_rectangle.h"
#include "nia_transform.h"
#include "nia_texture_structures.h"

/**
 *  @class niaSprite
 *  @author Streanga Sarmis-Stefan
 *  @brief 
 * 
 *  Sprite class for storing details about sprites
 */
// TODO add loading for already existing textures
NIA_CLASS niaSprite {
private:
    niaGlRectangle rectangle;
    u32 vbo;
    u32 veo;

    niaTexture texture;
    niaTransform transform;

public:
    NIA_CALL niaSprite();

    /**
     * @brief Sprite constructor
     * @param bitmap A pointer to where the data is stored in memory for the texture
     * @param textureWidth The width of the texture, must be a multiple of 4
     * @param textureHeight The height of the texture
     * @param details The format in which the texture is presented
     * @param rect A rectangle that defines the origin and the size of the sprite
     */
    NIA_CALL niaSprite(u8* bitmap, u32 textureWidth, u32 textureHeight, const textureFormatDetails& details, const niaRectangle& rect);

    /**
     * @brief Sprite constructor
     * @param filename Path to the texture that must be loaded for the sprite
     * @param rect A rectangle that defines the origin and the size of the sprite
     * @param flags Flags that define how the sprite should load the texture
     */
    NIA_CALL niaSprite(const char* filename, const niaRectangle& rect, u32 flags = NIA_TEXTURE_NOT_ANIMATED);
    NIA_CALL ~niaSprite();

    /**
     * @brief Loading a different texture for the sprite
     * @param filename Path to the image file the texture should have
     * @param flag Optinal Parameter, a collection of flags that define how the texture should be used
     */
    void NIA_CALL loadTexture(const char* filename, u32 flags);

    /**
     * @brief Loading a different texture for the sprite
     * @param data Pointer to the bitmap
     * @param width The width of the image, should be a multiple of 4
     * @param height The height of the image
     * @param details Represents the format of the image and how it should be represented
     */
    void NIA_CALL loadTexture(u8* bitmap, u32 width, u32 height, const textureFormatDetails& details);
    void NIA_CALL setBounds(const niaRectangle& rect);

    // TODO make more versions of update texture, maybe we won't need to specify the texture details next time
    //      just the new data would be enough assuming the width and height are the same, the same goes
    //      for the textureFormatDetails.

    /**
     * @brief Update an already imported texture
     * @param data Pointer to the bitmap
     * @param width The width of the image, should be a multiple of 4
     * @param height The height of the image
     * @param details Represents the format of the image and how it should be represented
     */
    void NIA_CALL updateTexture(u8* bitmap, u32 width, u32 height, const textureFormatDetails& details);

    // TODO add scaling on every axis, not all at once
    void NIA_CALL scale(r32 amount);
    
    void NIA_CALL translate(r32 x, r32 y, r32 z);
    void NIA_CALL translate(const niaVector3<r32>& position);

    // TODO add rotation on multiple axis at once
    void NIA_CALL rotate(r32 amount, niaAxis axis);

    void NIA_CALL clearTransformations();

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

    NIA_INLINE niaTransform getTransformation() const {
        return transform;
    }

};

#endif //_NIA_SPRITE_H_