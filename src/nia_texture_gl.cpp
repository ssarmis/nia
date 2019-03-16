#include "nia_texture_gl.h"

NIA_CALL niaTexture::niaTexture(){
    float pixels[] = {
        0.0f, 0.0f, 0.0f,   1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f
    };

    allocateTexture(2, 2, pixels);
}

NIA_CALL niaTexture::~niaTexture(){
}

NIA_INTERNAL void niaTexture::allocateTexture(u32 width, u32 height, r32* data){
    NIA_GL_CALL(glGenTextures(1, &textureId));

    NIA_GL_CALL(glBindTexture(GL_TEXTURE_2D, textureId));

    NIA_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    NIA_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    NIA_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    NIA_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

    NIA_GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
    NIA_GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, data));

    NIA_GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}