/*
 * File: nia_cube_texture.h
 * Project: Nia
 * File created on March-27-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef _NIA_CUBE_TEXTURE_GL_H_
#define _NIA_CUBE_TEXTURE_GL_H_

#include "nia_gl.h"
#include "nia_general.h"

NIA_CLASS niaCubeTexture {
private:
    NIA_INTERNAL void allocateTexture(u32 width, u32 height, u8* data, u32 side);

public:
    GLuint textureId;
    
    NIA_CALL niaCubeTexture();
    NIA_CALL niaCubeTexture(const char* textures[6]);
    NIA_CALL ~niaCubeTexture();
};

#endif //_NIA_CUBE_TEXTURE_GL_H_