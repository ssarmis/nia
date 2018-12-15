#ifdef NIA_RENDERER_GL

#include "nia_batch_renderer_gl.h"

#include "nia_gl.h"

#define NIA_SIMPLE_RENDERER         0x00000001
#define NIA_BATCH_RENDERER          0x00000002

NIA_STRUCT niaVertex{
    union {
        float attrs[6];
        struct {
            union {
                float vert[3];
                struct {
                    float x;
                    float y;
                    float z;
                };
            };

            union {
                float cols[3];
                struct {
                    float r;
                    float g;
                    float b;
                };
            };
        };
    };
};

#define NIA_BATCH_MAXIMUM_QUADS     100000
#define NIA_BATCH_VERTICES_COUNT    (NIA_BATCH_MAXIMUM_QUADS * sizeof(niaVertex))
#define NIA_BATCH_INDICES_COUNT     (NIA_BATCH_VERTICES_COUNT * 6)

//  0  +---+  3      order to render: 0 1 2   0 3 1
//     |\  |         so 6 indices per quad
//     |  \|
//  2  +---+  1

#define NIA_VERTEX_STRIDE   0
#define NIA_COLOR_STRIDE    (3 * sizeof(float))

// niaVertex
//      float x;
//      float y;
//      float z;
//      float r;
//      float g;
//      float b;


NIA_INTERNAL u32 batchVao = 0;
NIA_INTERNAL u32 batchVbo = 0;
NIA_INTERNAL u32 batchVeo = 0;
NIA_INTERNAL u32 batchUsedIndices = 0;
NIA_INTERNAL u16 batchIndices[NIA_BATCH_INDICES_COUNT] = {};

NIA_INTERNAL u32 usedRectangles = 0;

typedef struct niaRectangle{
    r32 x;
    r32 y;
    r32 w;
    r32 h;
    union {
        r32 c[3];
        struct {
            r32 r;
            r32 g;
            r32 b;
        };
    };
} niaRectangle;

NIA_INTERNAL niaRectangle rectangleArray[NIA_BATCH_MAXIMUM_QUADS] = {};

NIA_INTERNAL void setupBatchBuffers(){
    glGenVertexArrays(1, &batchVao);
    glBindVertexArray(batchVao);

    glGenBuffers(1, &batchVbo);
    glBindBuffer(GL_ARRAY_BUFFER, batchVbo);

    glBufferData(GL_ARRAY_BUFFER, NIA_BATCH_VERTICES_COUNT, 0, GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_VERTEX_STRIDE);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_COLOR_STRIDE);
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

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

    glGenBuffers(1, &batchVeo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batchVeo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(batchIndices), batchIndices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

NIA_CALL niaBatchRenderer::niaBatchRenderer(){
    setupBatchBuffers();
}

NIA_CALL niaBatchRenderer::~niaBatchRenderer(){
}

NIA_CALL void niaBatchRenderer::renderRectangle(r32 x, r32 y, r32 w, r32 h, r32 colors[3]){
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

NIA_CALL void niaBatchRenderer::renderRectangle(r32 x, r32 y, r32 w, r32 h){
    r32 colors[3] = {0, 1, 0};
    renderRectangle(x, y, w, h, colors);
}

NIA_INTERNAL void niaBuildVertex(niaVertex* source, const niaRectangle& rectangle, u32* vertex){
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

NIA_CALL void niaBatchRenderer::executeRender(){
    glBindBuffer(GL_ARRAY_BUFFER, batchVbo);
    niaVertex* source = (niaVertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    
    u32 vertex = 0;
    for(u32 rect = 0; rect < usedRectangles; ++rect){
        niaBuildVertex(source, rectangleArray[rect], &vertex);
    }

    nia_memset((u8*)rectangleArray, 0, usedRectangles * sizeof(niaRectangle));
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(batchVao);
    shader.useShader();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batchVeo);
    glDrawElements(GL_TRIANGLES, batchUsedIndices, GL_UNSIGNED_SHORT, 0);
    shader.unuseShader();
    glBindVertexArray(0);

    usedRectangles = 0;
    batchUsedIndices = 0;
}

#endif