#ifndef _NIA_FRAME_BUFFER_H_
#define _NIA_FRAME_BUFFER_H_

#include "nia_gl.h"
#include "nia_general.h"

NIA_CLASS niaFrameBuffer {
private:
    GLuint frameBufferId;
    GLuint textureId;

public:
    NIA_CALL niaFrameBuffer(r32 width, r32 height, GLenum attachment);
    NIA_CALL niaFrameBuffer();
    NIA_CALL ~niaFrameBuffer();
    
    void NIA_CALL bind();
    void NIA_CALL unbind();

    static void NIA_CALL bindFrameBuffer(GLuint frameBufferId);
    static void NIA_CALL unbindFrameBuffer();

    GLuint NIA_CALL getTextureId();
};

#endif // _NIA_FRAME_BUFFER_H_