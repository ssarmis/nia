#ifdef NIA_RENDERER_GL

#include "renderers/nia_renderer_gl.h"

#include "nia_gl.h"
#include "nia_vector.h"
#include "nia_vertex.h"
#include "nia_constants.h"

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

#include "nia_texture_gl.h"

NIA_CALL niaRenderer::niaRenderer(){
    cubeMesh = niaMesh::cube(100);
    
    NIA_GL_CALL(glGenVertexArrays(1, &rectVao)); // put this inside niaMesh
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

NIA_CALL void niaRenderer::clear(){
    NIA_GL_CALL(glClearColor(0, 0, 0, 0));
    NIA_GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

NIA_CALL void niaRenderer::enableDepthTest(){
    NIA_GL_CALL(glEnable(GL_DEPTH_TEST));
}

NIA_CALL void niaRenderer::pushOrthographicView(r32 left, r32 right, r32 top, r32 bottom, r32 n, r32 f){
    defaultShader.useShader();
    defaultShader.setUniformMat4(NIA_UNIFORM_PROJECTION, niaMatrix4::orthographic(left, right, top, bottom, n, f));
    defaultShader.setUniformMat4(NIA_UNIFORM_TRANSFORM, niaMatrix4::identity());
    defaultShader.setUniformMat4(NIA_UNIFORM_VIEW, niaMatrix4::identity());
    defaultShader.unuseShader();

    defaultShaderBatch.useShader();
    defaultShaderBatch.setUniformMat4(NIA_UNIFORM_PROJECTION, niaMatrix4::orthographic(left, right, top, bottom, n, f));
    defaultShaderBatch.setUniformMat4(NIA_UNIFORM_TRANSFORM, niaMatrix4::identity());
    defaultShaderBatch.setUniformMat4(NIA_UNIFORM_VIEW, niaMatrix4::identity());
    defaultShaderBatch.unuseShader();

    defaultShaderFont.useShader();
    defaultShaderFont.setUniformMat4(NIA_UNIFORM_PROJECTION, niaMatrix4::orthographic(left, right, top, bottom, n, f));
    defaultShaderFont.setUniformMat4(NIA_UNIFORM_TRANSFORM, niaMatrix4::identity());
    defaultShaderFont.setUniformMat4(NIA_UNIFORM_VIEW, niaMatrix4::identity());
    defaultShaderFont.unuseShader();
}

NIA_CALL void niaRenderer::pushPerspectiveView(r32 fov, r32 aspectRatio, r32 n, r32 f){
    defaultShader.useShader();
    defaultShader.setUniformMat4(NIA_UNIFORM_PROJECTION, niaMatrix4::perspective(fov, aspectRatio, n, f));
    defaultShader.setUniformMat4(NIA_UNIFORM_TRANSFORM, niaMatrix4::identity());
    defaultShader.setUniformMat4(NIA_UNIFORM_VIEW, niaMatrix4::identity());
    defaultShader.unuseShader();

    defaultShaderBatch.useShader();
    defaultShaderBatch.setUniformMat4(NIA_UNIFORM_PROJECTION, niaMatrix4::perspective(fov, aspectRatio, n, f));
    defaultShaderBatch.setUniformMat4(NIA_UNIFORM_TRANSFORM, niaMatrix4::identity());
    defaultShaderBatch.setUniformMat4(NIA_UNIFORM_VIEW, niaMatrix4::identity());
    defaultShaderBatch.unuseShader();

    defaultShaderFont.useShader();
    defaultShaderFont.setUniformMat4(NIA_UNIFORM_PROJECTION, niaMatrix4::perspective(fov, aspectRatio, n, f));
    defaultShaderFont.setUniformMat4(NIA_UNIFORM_TRANSFORM, niaMatrix4::identity());
    defaultShaderFont.setUniformMat4(NIA_UNIFORM_VIEW, niaMatrix4::identity());
    defaultShaderFont.unuseShader();

    defaultShaderCubeMap.useShader();
    defaultShaderCubeMap.setUniformMat4(NIA_UNIFORM_PROJECTION, niaMatrix4::perspective(fov, aspectRatio, n, f));
    defaultShaderCubeMap.unuseShader();

    defaultShaderReflective.useShader();
    defaultShaderReflective.setUniformMat4(NIA_UNIFORM_PROJECTION, niaMatrix4::perspective(fov, aspectRatio, n, f));
    defaultShaderReflective.unuseShader();
}

NIA_CALL void niaRenderer::submitTransformation(const niaTransform& transformation, bool transpose){
    defaultShader.useShader();
    defaultShader.setUniformMat4(NIA_UNIFORM_TRANSFORM, transformation.getTransformation(), transpose);
    defaultShader.unuseShader();

    defaultShaderBatch.useShader();
    defaultShaderBatch.setUniformMat4(NIA_UNIFORM_TRANSFORM, transformation.getTransformation(), transpose);
    defaultShaderBatch.unuseShader();

    defaultShaderReflective.useShader();
    defaultShaderReflective.setUniformMat4(NIA_UNIFORM_TRANSFORM, transformation.getTransformation(), transpose);
    defaultShaderReflective.unuseShader();
}

NIA_CALL void niaRenderer::submitView(const niaMatrix4& view, bool transpose){
    defaultShader.useShader();
    defaultShader.setUniformMat4(NIA_UNIFORM_VIEW, view, transpose);
    defaultShader.unuseShader();

    defaultShaderBatch.useShader();
    defaultShaderBatch.setUniformMat4(NIA_UNIFORM_VIEW, view, transpose);
    defaultShaderBatch.unuseShader();

    defaultShaderCubeMap.useShader();
    defaultShaderCubeMap.setUniformMat4(NIA_UNIFORM_VIEW, view, transpose);
    defaultShaderCubeMap.unuseShader();

    defaultShaderReflective.useShader();
    defaultShaderReflective.setUniformMat4(NIA_UNIFORM_VIEW, view, transpose);
    defaultShaderReflective.unuseShader();
}

NIA_CALL void niaRenderer::renderRectangle(r32 x, r32 y, r32 w, r32 h){
    r32 colors[] = {1, 1, 1};
    renderRectangle(x, y, w, h, colors);
}

NIA_CALL void niaRenderer::renderRectangle(r32 x, r32 y, r32 w, r32 h, r32 colors[3]){
    renderRectangle(x, y, 0, w, h, colors);
}

NIA_CALL void niaRenderer::renderRectangle(r32 x, r32 y, r32 z, r32 w, r32 h, r32 colors[3]){ // TODO add unbinding for vaos
    createAndBufferVertexies(x, y, z, w, h, colors);
    NIA_GL_CALL(glBindVertexArray(rectVao));
    defaultShader.useShader();
    NIA_GL_CALL(glBindTexture(GL_TEXTURE_2D, defaultTexture.textureId));

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectVeo));
    NIA_GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
    defaultShader.unuseShader();
}

NIA_CALL void niaRenderer::renderMesh(const niaMesh& mesh){
    NIA_GL_CALL(glBindVertexArray(mesh.vao.id));
    defaultShader.useShader();
    NIA_GL_CALL(glBindTexture(GL_TEXTURE_2D, defaultTexture.textureId));
    
    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vao.veoId));
    NIA_GL_CALL(glDrawElements(GL_TRIANGLES, mesh.verts, GL_UNSIGNED_SHORT, 0));
    defaultShader.unuseShader();
}

NIA_CALL void niaRenderer::renderMesh(const niaMesh& mesh, niaTexture& texture){
    NIA_GL_CALL(glBindVertexArray(mesh.vao.id));
    defaultShader.useShader();
    NIA_GL_CALL(glBindTexture(GL_TEXTURE_2D, texture.getTextureId()));

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vao.veoId));
    NIA_GL_CALL(glDrawElements(GL_TRIANGLES, mesh.verts, GL_UNSIGNED_SHORT, 0));
    defaultShader.unuseShader();
}

NIA_CALL void niaRenderer::renderMesh(const niaMesh& mesh, GLuint textureId){
    NIA_GL_CALL(glBindVertexArray(mesh.vao.id));
    defaultShader.useShader();
    NIA_GL_CALL(glBindTexture(GL_TEXTURE_2D, textureId));

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vao.veoId));
    NIA_GL_CALL(glDrawElements(GL_TRIANGLES, mesh.verts, GL_UNSIGNED_SHORT, 0));
    defaultShader.unuseShader();
}

// The NO-raw functions will NOT call the RAW fuction because we don't know if the compiler will optimize the calls

NIA_CALL void niaRenderer::renderMeshRaw(const niaMesh& mesh){
    NIA_GL_CALL(glBindVertexArray(mesh.vao.id));

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vao.veoId));
    NIA_GL_CALL(glDrawElements(GL_TRIANGLES, mesh.verts, GL_UNSIGNED_SHORT, 0));
}

NIA_CALL void niaRenderer::renderMeshRaw(const niaMesh& mesh, const niaTexture& texture){
    NIA_GL_CALL(glBindVertexArray(mesh.vao.id));
    NIA_GL_CALL(glBindTexture(GL_TEXTURE_2D, texture.textureId));

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vao.veoId));
    NIA_GL_CALL(glDrawElements(GL_TRIANGLES, mesh.verts, GL_UNSIGNED_SHORT, 0));
}
NIA_CALL void niaRenderer::renderMeshRaw(const niaMesh& mesh, GLuint textureId){
    NIA_GL_CALL(glBindVertexArray(mesh.vao.id));
    NIA_GL_CALL(glBindTexture(GL_TEXTURE_2D, textureId));

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vao.veoId));
    NIA_GL_CALL(glDrawElements(GL_TRIANGLES, mesh.verts, GL_UNSIGNED_SHORT, 0));
}

NIA_CALL void niaRenderer::renderSkyBox(const niaCubeTexture& texture){
    NIA_GL_CALL(glDepthFunc(GL_LEQUAL));
    NIA_GL_CALL(glBindVertexArray(cubeMesh.vao.id));
    defaultShaderCubeMap.useShader();
    NIA_GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, texture.textureId));

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeMesh.vao.veoId));
    NIA_GL_CALL(glDrawElements(GL_TRIANGLES, cubeMesh.verts, GL_UNSIGNED_SHORT, 0));
    defaultShaderCubeMap.unuseShader();
    NIA_GL_CALL(glDepthFunc(GL_LESS));
}

NIA_CALL void niaRenderer::renderReflectiveMesh(const niaMesh& mesh, const niaCubeTexture& texture){ // TODO make the cubeTexture not needed
    NIA_GL_CALL(glBindVertexArray(mesh.vao.id));
    defaultShaderReflective.useShader();
    NIA_GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, texture.textureId));

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vao.veoId));
    NIA_GL_CALL(glDrawElements(GL_TRIANGLES, mesh.verts, GL_UNSIGNED_SHORT, 0));
    defaultShaderReflective.unuseShader();
}

#endif
