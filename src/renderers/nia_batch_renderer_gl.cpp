/*
 * File: nia_batch_renderer_gl.cpp
 * Project: Nia
 * File created on May-18-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifdef NIA_RENDERER_GL

#include "renderers/nia_batch_renderer_gl.h"

#include "nia_gl.h"
#include "nia_vertex.h"
#include "nia_constants.h"

#define NIA_SIMPLE_RENDERER         0x00000001
#define NIA_BATCH_RENDERER          0x00000002

#define NIA_BATCH_MAXIMUM_QUADS     1000000
#define NIA_BATCH_VERTICES_COUNT    (NIA_BATCH_MAXIMUM_QUADS * sizeof(niaBasicVertex))
#define NIA_BATCH_INDICES_COUNT     (NIA_BATCH_VERTICES_COUNT * 6)

//  0  +---+  3      order to render: 0 1 2   0 3 1
//     |\  |         so 6 indices per quad
//     |  \|
//  2  +---+  1

// niaBasicVertex
//      float x;
//      float y;
//      float z;
//      float r;
//      float g;
//      float b;

NIA_STATIC u32 batchVao = 0;
NIA_STATIC u32 batchVbo = 0;
NIA_STATIC u32 batchVeo = 0;
NIA_STATIC u32 batchUsedIndices = 0;
NIA_STATIC u16 batchIndices[NIA_BATCH_INDICES_COUNT] = {};

NIA_STATIC u32 usedRectangles = 0;

niaRectangle rectangleArray[NIA_BATCH_MAXIMUM_QUADS] = {};

NIA_STATIC void setupBatchBuffers(){
    NIA_GL_CALL(glGenVertexArrays(1, &batchVao));
    NIA_GL_CALL(glBindVertexArray(batchVao));

    NIA_GL_CALL(glGenBuffers(1, &batchVbo));
    NIA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, batchVbo));

    NIA_GL_CALL(glBufferData(GL_ARRAY_BUFFER, NIA_BATCH_VERTICES_COUNT, 0, GL_DYNAMIC_DRAW));
    
    NIA_GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(niaBasicVertex), (GLvoid*)NIA_VERTEX_STRIDE));
    NIA_GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(niaBasicVertex), (GLvoid*)NIA_COLOR_STRIDE));
    
    NIA_GL_CALL(glEnableVertexAttribArray(0));
    NIA_GL_CALL(glEnableVertexAttribArray(1));

    NIA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

    u32 indicesOffset = 0;

    for(int i = 0; i < NIA_BATCH_INDICES_COUNT; i += 6){
        // 0 1 2   0 3 1
        batchIndices[i + 0] = indicesOffset + 0;
        batchIndices[i + 1] = indicesOffset + 1;
        batchIndices[i + 2] = indicesOffset + 2;

        batchIndices[i + 3] = indicesOffset + 0;
        batchIndices[i + 4] = indicesOffset + 3;
        batchIndices[i + 5] = indicesOffset + 1;

        indicesOffset += 4;
    }

    NIA_GL_CALL(glGenBuffers(1, &batchVeo));

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batchVeo));
    NIA_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(batchIndices), batchIndices, GL_STATIC_DRAW));

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    NIA_GL_CALL(glBindVertexArray(0));
}

niaBatchRenderer::niaBatchRenderer(){
    setupBatchBuffers();
}

niaBatchRenderer::~niaBatchRenderer(){
}

void niaBatchRenderer::renderRectangle(r32 x, r32 y, r32 w, r32 h, r32 colors[3]){
    if (usedRectangles == NIA_BATCH_MAXIMUM_QUADS){
        NIA_ERROR("The amount of rectangles requested to be drawn is too much(over %d)\n", NIA_BATCH_MAXIMUM_QUADS);
    } else {
        rectangleArray[usedRectangles].x = x;
        rectangleArray[usedRectangles].y = y;

        rectangleArray[usedRectangles].w = w;
        rectangleArray[usedRectangles].h = h;

        rectangleArray[usedRectangles].r = colors[0];
        rectangleArray[usedRectangles].g = colors[1];
        rectangleArray[usedRectangles].b = colors[2];

        ++usedRectangles;
        batchUsedIndices += 6;
    }
}

void niaBatchRenderer::renderRectangle(r32 x, r32 y, r32 w, r32 h, const niaVec3f& colors){
    if (usedRectangles == NIA_BATCH_MAXIMUM_QUADS){
        NIA_ERROR("The amount of rectangles requested to be drawn is too much(over %d)\n", NIA_BATCH_MAXIMUM_QUADS);
    } else {
        rectangleArray[usedRectangles].x = x;
        rectangleArray[usedRectangles].y = y;

        rectangleArray[usedRectangles].w = w;
        rectangleArray[usedRectangles].h = h;

        rectangleArray[usedRectangles].r = colors.x;
        rectangleArray[usedRectangles].g = colors.y;
        rectangleArray[usedRectangles].b = colors.z;

        ++usedRectangles;
        batchUsedIndices += 6;
    }
}

void niaBatchRenderer::renderRectangle(r32 x, r32 y, r32 w, r32 h){
    r32 colors[3] = {0, 1, 0};
    renderRectangle(x, y, w, h, colors);
}

NIA_INTERNAL void niaBuildVertex(niaBasicVertex* source, const niaRectangle& rectangle, u32* vertex){
    for (u32 i = 0; i < 4; ++i){
        source[*vertex + i].r = rectangle.r;
        source[*vertex + i].g = rectangle.g;
        source[*vertex + i].b = rectangle.b;
    }

    source[*vertex + 0].x = rectangle.x;
    source[*vertex + 0].y = rectangle.y;
    source[*vertex + 0].z = 0;
    
    source[*vertex + 1].x = rectangle.x + rectangle.w;
    source[*vertex + 1].y = rectangle.y + rectangle.h;
    source[*vertex + 1].z = 0;

    source[*vertex + 2].x = rectangle.x;
    source[*vertex + 2].y = rectangle.y + rectangle.h;
    source[*vertex + 2].z = 0;

    source[*vertex + 3].x = rectangle.x + rectangle.w;
    source[*vertex + 3].y = rectangle.y;
    source[*vertex + 3].z = 0;

    *vertex += 4;
}

void niaBatchRenderer::executeRender(){
    NIA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, batchVbo));
    niaBasicVertex* source = (niaBasicVertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    
    if(source){
        u32 vertex = 0;
        for(u32 rect = 0; rect < usedRectangles; ++rect){
            niaBuildVertex(source, rectangleArray[rect], &vertex);
        }
    }

    nia_memset((u8*)rectangleArray, 0, usedRectangles * sizeof(niaRectangle));
    NIA_GL_CALL(glUnmapBuffer(GL_ARRAY_BUFFER));
    NIA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

    NIA_GL_CALL(glBindVertexArray(batchVao));
    defaultShaderBatch.useShader();
    
    NIA_GL_CALL(glBindTexture(GL_TEXTURE_2D, defaultTexture.textureId));

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batchVeo));
    NIA_GL_CALL(glDrawElements(GL_TRIANGLES, batchUsedIndices, GL_UNSIGNED_SHORT, 0));

    defaultShaderBatch.unuseShader();

    NIA_GL_CALL(glBindVertexArray(0));

    usedRectangles = 0;
    batchUsedIndices = 0;
}

#endif