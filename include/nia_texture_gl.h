/*
 * File: nia_texture_gl.h
 * Project: Nia
 * File created on October-09-2018
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

/** @file nia_texture_gl.h
  * @author Streanga Sarmis-Stefan
  * @brief
  * 
  */
#ifndef _NIA_TEXTURE_GL_H_
#define _NIA_TEXTURE_GL_H_

#include "nia_gl.h"
#include "nia_general.h"
#include "nia_texture_structures.h"

#define NIA_TEXTURE_NO_FLAG         0x00000000
#define NIA_TEXTURE_ANIMATED        0x00000001
#define NIA_TEXTURE_NOT_ANIMATED    0x00000002

NIA_STRUCT niaAnimatedTexture {
    class niaTexture* texture;
    struct niaAnimatedTexture* next;
} niaAnimatedTexture;

NIA_STRUCT niaAnimatedTextureChain {
    niaAnimatedTexture* current;
    niaAnimatedTexture* head;
    niaAnimatedTexture* tail;
} niaAnimatedTextureChain;

/**
 *  @class niaTexture
 *  @author Streanga Sarmis-Stefan
 *  @brief 
 * 
 *  Texture class used to parse image files, and refer to internal textures
 */

NIA_CLASS niaTexture {
private:
    bool isAnimated;
    niaAnimatedTextureChain textureChain;

    NIA_INTERNAL void allocateTexture(u32 width, u32 height,
                                      u8* data, 
                                      const textureFormatDetails& details = NIA_TEXTURE_FORMAT_RGB_BGR_UBYTE);

public:
    GLuint textureId;
    NIA_CALL niaTexture();

    /**
     * @brief Texture constructor
     * @param filename Path to the image file the texture should have
     * @param flag Optinal Parameter, a collection of flags that define how the texture should be used
     * @param frameNumber Optinal Parameter, if a file with multiple frames is imported, this parameter represents what frame to be imported
     */
    NIA_CALL niaTexture(const char* filename, u32 flag = NIA_TEXTURE_NO_FLAG, u16 frameNumber = 0);

    /**
     * @brief Texture constructor
     * @param data Pointer to the bitmap
     * @param width The width of the image, should be a multiple of 4
     * @param height The height of the image
     * @param details Represents the format of the image and how it should be represented
     */
    NIA_CALL niaTexture(u8* data, u32 width, u32 height, const textureFormatDetails& details);
    NIA_CALL ~niaTexture();

    /**
     * @brief Update an already imported texture
     * @param data Pointer to the bitmap
     * @param width The width of the image, should be a multiple of 4
     * @param height The height of the image
     * @param details Represents the format of the image and how it should be represented
     */
    void NIA_CALL updateTexture(u8* data, u32 width, u32 height, const textureFormatDetails& details);

    u32 NIA_CALL getTextureId();
};

#endif //_NIA_TEXTURE_GL_H_