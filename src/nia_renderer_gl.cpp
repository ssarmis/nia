#ifdef NIA_RENDERER_GL

#include "nia_renderer_gl.h"

#include "nia_constants.h"
#include "nia_general.h"
#include "nia_vector.h"
#include "nia_vertex.h"
#include "nia_gl.h"

//  0  +---+  3      order to render: 0 1 2   0 3 1
//     |\  |         so 6 indices per quad
//     |  \|
//  2  +---+  1

#define createAndBufferVertexies(_x, _y, _z, _w, _h, _colors) {\
    niaVertex v0 = {_x     , _y     , _z, _colors[0], _colors[1], _colors[2]};\
    niaVertex v1 = {_x + _w, _y + _h, _z, _colors[0], _colors[1], _colors[2]};\
    niaVertex v2 = {_x     , _y + _h, _z, _colors[0], _colors[1], _colors[2]};\
    niaVertex v3 = {_x + _w, _y     , _z, _colors[0], _colors[1], _colors[2]};\
    NIA_GL_CALL(glNamedBufferSubData(rectVbo, 0                    , sizeof(niaVertex), &v0));\
    NIA_GL_CALL(glNamedBufferSubData(rectVbo, sizeof(niaVertex) * 1, sizeof(niaVertex), &v1));\
    NIA_GL_CALL(glNamedBufferSubData(rectVbo, sizeof(niaVertex) * 2, sizeof(niaVertex), &v2));\
    NIA_GL_CALL(glNamedBufferSubData(rectVbo, sizeof(niaVertex) * 3, sizeof(niaVertex), &v3));\
}

u32 rectVao = 0;
u32 rectVeo = 0;
u32 rectVbo = 0;

NIA_CALL niaRenderer::niaRenderer(){
    NIA_GL_CALL(glGenVertexArrays(1, &rectVao));
    NIA_GL_CALL(glBindVertexArray(rectVao));

    NIA_GL_CALL(glGenBuffers(1, &rectVbo));
    NIA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, rectVbo));

    NIA_GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(niaVertex) * 4, 0, GL_DYNAMIC_DRAW));
    
    NIA_GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_VERTEX_STRIDE));
    NIA_GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_COLOR_STRIDE));
    
    NIA_GL_CALL(glEnableVertexAttribArray(0));
    NIA_GL_CALL(glEnableVertexAttribArray(1));

    NIA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

    u8 indices[] = {0, 1, 2,
                 0, 3, 1};

    NIA_GL_CALL(glGenBuffers(1, &rectVeo));

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectVeo));
    NIA_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    NIA_GL_CALL(glBindVertexArray(0));
}

NIA_CALL niaRenderer::~niaRenderer(){
}

NIA_CALL void niaRenderer::enableDepthTest(){
    NIA_GL_CALL(glEnable(GL_DEPTH_TEST));
}

NIA_CALL void niaRenderer::pushOrthographicView(r32 left, r32 right, r32 top, r32 bottom, r32 n, r32 f){
    shader.useShader();
    shader.setUniformMat4("mP", niaMatrix4::orthographic(left, right, top, bottom, n, f));
    shader.setUniformMat4("mT", niaMatrix4::identity());
    shader.unuseShader();
}

NIA_CALL void niaRenderer::pushPerspectiveView(r32 fov, r32 aspectRatio, r32 n, r32 f){
    shader.useShader();
    shader.setUniformMat4("mP", niaMatrix4::perspective(fov, aspectRatio, n, f));
    shader.setUniformMat4("mT", niaMatrix4::identity());
    shader.unuseShader();
}

NIA_CALL void niaRenderer::submitTransformation(const niaMatrix4& transformation, bool transpose){
    shader.useShader();
    shader.setUniformMat4("mT", transformation, transpose);
    shader.unuseShader();
}

NIA_CALL void niaRenderer::renderRectangle(r32 x, r32 y, r32 w, r32 h){
    r32 colors[] = {1, 1, 1};
    renderRectangle(x, y, w, h, colors);
}

NIA_CALL void niaRenderer::renderRectangle(r32 x, r32 y, r32 w, r32 h, r32 colors[3]){
    renderRectangle(x, y, 0, w, h, colors);
}

NIA_CALL void niaRenderer::renderRectangle(r32 x, r32 y, r32 z, r32 w, r32 h, r32 colors[3]){
    createAndBufferVertexies(x, y, z, w, h, colors);
    NIA_GL_CALL(glBindVertexArray(rectVao));
    shader.useShader();
    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectVeo));
    NIA_GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
    shader.unuseShader();
}

NIA_CALL void niaRenderer::renderMesh(const niaMesh& mesh){
    NIA_GL_CALL(glBindVertexArray(mesh.vao));
    shader.useShader();
    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.veo));
    NIA_GL_CALL(glDrawElements(GL_TRIANGLES, mesh.verts, GL_UNSIGNED_SHORT, 0));
    shader.unuseShader();
}

#endif
