#ifndef _NIA_TEXTURE_GL_H_
#define _NIA_TEXTURE_GL_H_

#include "nia_gl.h"
#include "nia_general.h"
#include "nia_texture_structures.h"

#define NIA_TEXTURE_NO_FLAG         0x00000000
#define NIA_TEXTURE_NOT_ANIMATED    0x00000002
#define NIA_TEXTURE_ANIMATED        0x00000001

NIA_STRUCT niaAnimatedTexture {
    class niaTexture* texture;
    struct niaAnimatedTexture* next;
} niaAnimatedTexture;

NIA_STRUCT niaAnimatedTextureChain {
    niaAnimatedTexture* current;
    niaAnimatedTexture* head;
    niaAnimatedTexture* tail;
} niaAnimatedTextureChain;

NIA_CLASS niaTexture {
private:
    bool isAnimated;
    niaAnimatedTextureChain textureChain;

    NIA_INTERNAL void allocateTexture(u32 width, u32 height, u8* data, const textureFormatDetails& details = NIA_TEXTURE_FORMAT_RGB_BGR_UBYTE);

public:
    GLuint textureId;
    NIA_CALL niaTexture();
    NIA_CALL niaTexture(const char* filename, u32 flag = NIA_TEXTURE_NO_FLAG, u16 frameNumber = 0);
    NIA_CALL niaTexture(u8* data, u32 width, u32 height, const textureFormatDetails& details);
    NIA_CALL ~niaTexture();

    u32 NIA_CALL getTextureId();
};

#endif //_NIA_TEXTURE_GL_H_