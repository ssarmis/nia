#ifndef _NIA_TEXTURE_GL_H_
#define _NIA_TEXTURE_GL_H_

#include "nia_gl.h"
#include "nia_general.h"

NIA_STRUCT textureFormatDetails{
    GLenum storeFormat;
    GLenum readFormat;
    GLenum type;
} textureFormatDetails;

NIA_STATIC textureFormatDetails NIA_TEXTURE_FORMAT_RGB_BGR_UBYTE = {
    GL_RGB,
    GL_BGR,
    GL_UNSIGNED_BYTE
};

NIA_STATIC textureFormatDetails NIA_TEXTURE_FORMAT_RGBA_RGBA_UBYTE = {
    GL_RGBA,
    GL_RGBA,
    GL_UNSIGNED_BYTE
};

NIA_STATIC textureFormatDetails NIA_TEXTURE_FORMAT_R8_RED_UBYTE = {
    GL_R8,
    GL_RED,
    GL_UNSIGNED_BYTE
};

NIA_CLASS niaTexture {
private:
    NIA_INTERNAL void allocateTexture(u32 width, u32 height, u8* data, const textureFormatDetails& details = NIA_TEXTURE_FORMAT_RGB_BGR_UBYTE);

public:
    GLuint textureId;
    NIA_CALL niaTexture();
    NIA_CALL niaTexture(const char* filename);
    NIA_CALL niaTexture(u8* data, u32 width, u32 height, const textureFormatDetails& details);
    NIA_CALL ~niaTexture();
};

#endif //_NIA_TEXTURE_GL_H_