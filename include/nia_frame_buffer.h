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
    
    NIA_CALL void bind();
    NIA_CALL void unbind();

    NIA_CALL static void bindFrameBuffer(GLuint frameBufferId);
    NIA_CALL static void unbindFrameBuffer();

    NIA_CALL GLuint getTextureId();
};

#endif // _NIA_FRAME_BUFFER_H_