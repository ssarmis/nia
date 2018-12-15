#ifndef _NIA_TEXTURE_GL_H_
#define _NIA_TEXTURE_GL_H_

#include "nia_gl.h"
#include "nia_general.h"

NIA_CLASS niaTexture {
public:
    NIA_CALL niaTexture();
    NIA_CALL ~niaTexture();

    GLuint textureId;
};

#endif //_NIA_TEXTURE_GL_H_