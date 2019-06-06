/*
 * File: nia_sprite.cpp
 * Project: Nia
 * File created on May-28-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "nia_sprite.h"

#include "nia_gl.h"
#include "nia_vertex.h"
#include "nia_constants.h"

niaSprite::niaSprite(){
}

niaSprite::niaSprite(u8* bitmap, u32 textureWidth, u32 textureHeight, const textureFormatDetails& details, const niaRectangle& rect):
niaSprite(NULL, rect){
    loadTexture(bitmap, textureWidth, textureHeight, details);
}

niaSprite::niaSprite(const char* filename, const niaRectangle& rect, u32 flags){
    if(filename){
        loadTexture(filename, flags);
    }

    setBounds(rect);

    NIA_GL_CALL(glGenVertexArrays(1, &rectangle.vao)); // put this inside niaMesh
    NIA_GL_CALL(glBindVertexArray(rectangle.vao));

    NIA_GL_CALL(glGenBuffers(1, &vbo));
    NIA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));

    NIA_GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(niaVertex) * 4, 0, GL_DYNAMIC_DRAW));

    u32 customUVTStride = NIA_VERTEX_STRIDE + NIA_COLOR_STRIDE;

    NIA_GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_VERTEX_STRIDE));
    NIA_GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_COLOR_STRIDE));
    NIA_GL_CALL(glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_NORMALS_STRIDE));
    NIA_GL_CALL(glVertexAttribPointer(3, 2, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_UV_STRIDE));
    
    NIA_GL_CALL(glEnableVertexAttribArray(0));
    NIA_GL_CALL(glEnableVertexAttribArray(1));
    NIA_GL_CALL(glEnableVertexAttribArray(2));
    NIA_GL_CALL(glEnableVertexAttribArray(3));


    // 0, 0         1, 0
    //
    //
    // 0, 1         1, 1



    niaVertex v[4] ={
                    {rectangle.x              , rectangle.y              , 0,        1, 1, 1,     1, 1, 1,      0, 1},
                    {rectangle.x + rectangle.w, rectangle.y + rectangle.h, 0,        1, 1, 1,     1, 1, 1,      1, 0},
                    {rectangle.x              , rectangle.y + rectangle.h, 0,        1, 1, 1,     1, 1, 1,      0, 0},
                    {rectangle.x + rectangle.w, rectangle.y              , 0,        1, 1, 1,     1, 1, 1,      1, 1}
    };

    NIA_GL_CALL(glNamedBufferSubData(vbo, 0, sizeof(niaVertex) * 4, v));

    NIA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

    u8 indices[] = {0, 1, 2,
                 0, 3, 1};

    NIA_GL_CALL(glGenBuffers(1, &veo));

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, veo));
    NIA_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

niaSprite::~niaSprite(){
}

void niaSprite::scale(r32 amount){
    transform.scale(amount);
}
    
void niaSprite::translate(r32 x, r32 y, r32 z){
    transform.translate(x, y, z);
}

void niaSprite::translate(const niaVector3<r32>& position){
    transform.translate(position);
}

void niaSprite::rotate(r32 amount, niaAxis axis){
    transform.rotate(amount, axis);
}

void niaSprite::loadTexture(const char* filename, u32 flags){
    // TODO make this actually work normally, pull of the test case that exists now below,
    //      niaTexture(filenmae) is the only one that is needed, the flag for animated or not
    //      + frame count if is the case will come in the destructor
    texture = niaTexture(filename, flags);
}

void niaSprite::loadTexture(u8* bitmap, u32 width, u32 height, const textureFormatDetails& details){
    texture = niaTexture(bitmap, width, height, details);
}

void NIA_CALL niaSprite::setBounds(const niaRectangle& rect){
    rectangle.w = rect.w;
    rectangle.h = rect.h;

    rectangle.x = rect.x;
    rectangle.y = rect.y;
}

void niaSprite::updateTexture(u8* bitmap, u32 width, u32 height, const textureFormatDetails& details){
    texture.updateTexture(bitmap, width, height, details);
}



