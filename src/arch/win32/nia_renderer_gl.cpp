#ifdef _WIN32

#ifdef NIA_RENDERER_GL

#include "arch/win32/nia_renderer_gl.h"

#include "nia_gl.h"

#define NIA_BATCH_MAXIMUM_QUADS     3000
#define NIA_BATCH_VERTICES_COUNT    (NIA_BATCH_MAXIMUM_QUADS * sizeof(niaRenderable))
#define NIA_BATCH_INDICES_COUNT     (NIA_BATCH_VERTICES_COUNT * 6)

//  0  +---+  3      order to render: 0 1 2   0 3 1
//     |\  |         so 6 indices per quad
//     |  \|
//  2  +---+  1

#define NIA_VERTEX_STRIDE   0
#define NIA_COLOR_STRIDE    (3 * sizeof(float))

// niaRenderable
//      float x;
//      float y;
//      float z;
//      float r;
//      float g;
//      float b;

u32 batchVao;
u32 batchVbo;

void setupBatchBuffers(){
    glGenVertexArrays(1, &batchVao);
    glBindVertexArray(batchVao);
    glGenBuffers(1, &batchVbo);
    glBindBuffer(GL_ARRAY_BUFFER, batchVbo);
    glBufferData(GL_ARRAY_BUFFER, NIA_BATCH_VERTICES_COUNT, 0, GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(niaRenderable), (GLvoid*)NIA_VERTEX_STRIDE);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(niaRenderable), (GLvoid*)NIA_COLOR_STRIDE);
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glGenBuffers(0, &batchVbo);
    
    glBindVertexArray(0);
}

NIA_CALL niaRenderer::niaRenderer(){
    setupBatchBuffers();
}

NIA_CALL niaRenderer::~niaRenderer(){
}


NIA_CALL void niaRenderer::swapBuffers(const niaWindow& window){
    SwapBuffers(window.deviceContext);
}

#endif
#endif
