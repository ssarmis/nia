/*
 * File: nia_frame_buffer.h
 * Project: Nia
 * File created on March-30-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef _NIA_FRAME_BUFFER_H_
#define _NIA_FRAME_BUFFER_H_

#include "nia_gl.h"
#include "nia_general.h"

NIA_CLASS niaFrameBuffer {
private:
    u32 width;
    u32 height;

    GLuint frameBufferId;
    GLuint textureId;
    GLuint depthTextureId;

public:
    NIA_CALL niaFrameBuffer(r32 width, r32 height);
    NIA_CALL niaFrameBuffer();
    NIA_CALL ~niaFrameBuffer();
    
    void NIA_CALL copyData(niaFrameBuffer& other);

    void NIA_CALL bind();
    void NIA_CALL unbind();
    void NIA_CALL bindRead();
    void NIA_CALL bindWrite();
    
    static void NIA_CALL bindFrameBuffer(GLuint frameBufferId);
    static void NIA_CALL unbindFrameBuffer();

    GLuint NIA_CALL getFrameBufferId() const;
    GLuint NIA_CALL getTextureId() const;
    GLuint NIA_CALL getDepthTextureId() const;

    u32 NIA_CALL getWidth() const;
    u32 NIA_CALL getHeight() const;
};

#endif // _NIA_FRAME_BUFFER_H_