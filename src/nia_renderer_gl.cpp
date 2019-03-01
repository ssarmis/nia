#ifdef NIA_RENDERER_GL

#include "nia_renderer_gl.h"

#include "nia_constants.h"
#include "nia_general.h"
#include "nia_vector.h"
#include "nia_vertex.h"
#include "nia_gl.h"

#define createAndBufferVertexies(_x, _y, _w, _h, _colors) {\
    niaVertex v0 = {_x     , _y     , 0, _colors[0], _colors[1], _colors[2]};\
    niaVertex v1 = {_x + _w, _y + _h, 0, _colors[0], _colors[1], _colors[2]};\
    niaVertex v2 = {_x     , _y + _h, 0, _colors[0], _colors[1], _colors[2]};\
    niaVertex v3 = {_x + _w, _y     , 0, _colors[0], _colors[1], _colors[2]};\
    glNamedBufferSubData(rectVbo, 0                    , sizeof(niaVertex), &v0);\
    glNamedBufferSubData(rectVbo, sizeof(niaVertex) * 1, sizeof(niaVertex), &v1);\
    glNamedBufferSubData(rectVbo, sizeof(niaVertex) * 2, sizeof(niaVertex), &v2);\
    glNamedBufferSubData(rectVbo, sizeof(niaVertex) * 3, sizeof(niaVertex), &v3);\
}

u32 rectVao = 0;
u32 rectVeo = 0;
u32 rectVbo = 0;

NIA_CALL niaRenderer::niaRenderer(){
    glGenVertexArrays(1, &rectVao);
    glBindVertexArray(rectVao);

    glGenBuffers(1, &rectVbo);
    glBindBuffer(GL_ARRAY_BUFFER, rectVbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(niaVertex) * 4, 0, GL_DYNAMIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_VERTEX_STRIDE);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_COLOR_STRIDE);
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    u8 indices[] = {0, 1, 2,
                 0, 3, 1};

    glGenBuffers(1, &rectVeo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectVeo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

NIA_CALL niaRenderer::~niaRenderer(){
}

NIA_CALL void niaRenderer::renderRectangle(r32 x, r32 y, r32 w, r32 h){

}

NIA_CALL void niaRenderer::renderRectangle(r32 x, r32 y, r32 w, r32 h, r32 colors[3]){
    //  0  +---+  3      order to render: 0 1 2   0 3 1
    //     |\  |         so 6 indices per quad
    //     |  \|
    //  2  +---+  1
    createAndBufferVertexies(x, y, w, h, colors);
    glBindVertexArray(rectVao);
    shader.useShader();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectVeo);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);
    shader.unuseShader();
}

NIA_CALL void niaRenderer::renderMesh(const niaMesh& mesh){
    glBindVertexArray(mesh.vao);
    shader.useShader();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.veo);
    glDrawElements(GL_TRIANGLES, mesh.verts, GL_UNSIGNED_SHORT, 0);
    shader.unuseShader();
}

#endif
