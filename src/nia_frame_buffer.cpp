#include "nia_frame_buffer.h"

NIA_CALL niaFrameBuffer::niaFrameBuffer(r32 width, r32 height, GLenum attachment){
    NIA_GL_CALL(glGenFramebuffers(1, &frameBufferId));
    NIA_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId));

    NIA_GL_CALL(glGenTextures(1, &textureId));

    NIA_GL_CALL(glBindTexture(GL_TEXTURE_2D, textureId));

    NIA_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    NIA_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    NIA_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    NIA_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    switch(attachment){
        case GL_DEPTH_ATTACHMENT: {
                NIA_GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
                NIA_GL_CALL(glDrawBuffer(GL_NONE));
            }
            break;
        case GL_COLOR_ATTACHMENT0: {
                NIA_GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_INT, NULL));
            }
            break;
    }

    NIA_GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
    NIA_GL_CALL(glFramebufferTexture(GL_FRAMEBUFFER, attachment, textureId, 0));

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE){
        NIA_ERROR("Frame buffer could not be made, check previous errors.\n");
    }

    NIA_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

NIA_CALL niaFrameBuffer::niaFrameBuffer(){
}

NIA_CALL niaFrameBuffer::~niaFrameBuffer(){
    // TODO remove frame buffer
}

NIA_CALL void niaFrameBuffer::bind(){
    NIA_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId));
}

NIA_CALL void niaFrameBuffer::unbind(){
    NIA_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

NIA_CALL void niaFrameBuffer::bindFrameBuffer(GLuint frameBufferId){
    NIA_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId));
}

NIA_CALL void niaFrameBuffer::unbindFrameBuffer(){
    NIA_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

NIA_CALL GLuint niaFrameBuffer::getTextureId(){
    return textureId;
}
