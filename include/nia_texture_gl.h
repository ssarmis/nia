#ifndef _NIA_TEXTURE_GL_H_
#define _NIA_TEXTURE_GL_H_

#include "nia_gl.h"
#include "nia_general.h"

NIA_CLASS niaTexture {
private:

    NIA_INTERNAL void allocateTexture(u32 width, u32 height, u8* data);

public:
    GLuint textureId;
    NIA_CALL niaTexture();
    NIA_CALL niaTexture(const char* filename);
    NIA_CALL ~niaTexture();
};

#endif //_NIA_TEXTURE_GL_H_