#ifndef _NIA_FRAME_BUFFER_H_
#define _NIA_FRAME_BUFFER_H_

#include "nia_gl.h"
#include "nia_general.h"

NIA_CLASS niaFrameBuffer {
private:
    GLuint frameBufferId;
    GLuint textureId;

public:
    niaFrameBuffer(r32 width, r32 height, GLenum attachment);
    niaFrameBuffer();
    ~niaFrameBuffer();
    
    void bind();
    void unbind();

    static void bindFrameBuffer(GLuint frameBufferId);
    static void unbindFrameBuffer();

    GLuint getTextureId();
};

#endif // _NIA_FRAME_BUFFER_H_