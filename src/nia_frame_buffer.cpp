/*
 * File: nia_frame_buffer.cpp
 * Project: Nia
 * File created on March-30-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "nia_frame_buffer.h"

NIA_INLINE u32 createColorAttachmentTexture(u32 width, u32 height){
    u32 result;
    NIA_GL_CALL(glGenTextures(1, &result));
    NIA_GL_CALL(glBindTexture(GL_TEXTURE_2D, result));
    NIA_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    NIA_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    NIA_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    NIA_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    NIA_GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_INT, NULL));
    NIA_GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
    return result;
}

NIA_INLINE u32 createDepthAttachmentTexture(u32 width, u32 height){
    u32 result;
    NIA_GL_CALL(glGenTextures(1, &result));
    NIA_GL_CALL(glBindTexture(GL_TEXTURE_2D, result));
    NIA_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    NIA_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    NIA_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    NIA_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    NIA_GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
    NIA_GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
    return result;
}

niaFrameBuffer::niaFrameBuffer(r32 width, r32 height):
width(width), height(height){
    textureId = createColorAttachmentTexture(width, height);
    depthTextureId = createDepthAttachmentTexture(width, height);

    NIA_GL_CALL(glGenFramebuffers(1, &frameBufferId));
    NIA_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId));

    NIA_GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0));
    NIA_GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureId, 0));

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE){
        NIA_ERROR("Frame buffer could not be made, check previous errors %d.\n", status);
    }

    NIA_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

niaFrameBuffer::niaFrameBuffer(){
    frameBufferId = 0;
    textureId = 0;
    depthTextureId = 0;
}

niaFrameBuffer::~niaFrameBuffer(){
    // TODO remove frame buffer
}

void niaFrameBuffer::copyData(niaFrameBuffer& other){
    width = other.getWidth();
    height = other.getHeight();
    // TODO if framebuffer was previously used, delete the old one(or not), make a new one
    //      and copy the data of the other in this one
    //      kinda fing ugly, should've tought of this before
    if(!frameBufferId){
        // create it
        textureId = createColorAttachmentTexture(width, height);
        depthTextureId = createDepthAttachmentTexture(width, height);

        NIA_GL_CALL(glGenFramebuffers(1, &frameBufferId));
        NIA_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId));

        NIA_GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0));
        NIA_GL_CALL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureId, 0));

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

        if (status != GL_FRAMEBUFFER_COMPLETE){
            NIA_ERROR("Frame buffer could not be made, check previous errors %d.\n", status);
        }

        NIA_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }

    other.bindRead();
    bindWrite();

    NIA_GL_CALL(glBlitFramebuffer(0, 0, width, height, 0, 0, width, height,
                                  GL_COLOR_BUFFER_BIT, GL_LINEAR));

    NIA_GL_CALL(glBlitFramebuffer(0, 0, width, height, 0, 0, width, height,
                                  GL_DEPTH_BUFFER_BIT, GL_LINEAR));

    NIA_GL_CALL(glBindFramebuffer(GL_READ_FRAMEBUFFER, 0));
    NIA_GL_CALL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
}

void niaFrameBuffer::bind(){
    NIA_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId));
}

void niaFrameBuffer::bindRead(){
    NIA_GL_CALL(glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBufferId));
}

void niaFrameBuffer::bindWrite(){
    NIA_GL_CALL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBufferId));
}

void niaFrameBuffer::unbind(){
    NIA_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void niaFrameBuffer::bindFrameBuffer(GLuint frameBufferId){
    NIA_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId));
}

void niaFrameBuffer::unbindFrameBuffer(){
    NIA_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

GLuint niaFrameBuffer::getFrameBufferId() const {
    return frameBufferId;
}

GLuint niaFrameBuffer::getTextureId() const {
    return textureId;
}

GLuint  niaFrameBuffer::getDepthTextureId() const {
    return depthTextureId;
}

u32 niaFrameBuffer::getWidth() const {
    return width;
}

u32 niaFrameBuffer::getHeight() const {
    return height;
}
