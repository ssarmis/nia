/*
 * File: nia_renderer_gl.cpp
 * Project: Nia
 * File created on May-18-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

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
    niaVertex v[4] ={ \
                    {_x     , _y     , _z, _colors[0], _colors[1], _colors[2]},\
                    {_x + _w, _y + _h, _z, _colors[0], _colors[1], _colors[2]},\
                    {_x     , _y + _h, _z, _colors[0], _colors[1], _colors[2]},\
                    {_x + _w, _y     , _z, _colors[0], _colors[1], _colors[2]}\
    };\
    NIA_GL_CALL(glNamedBufferSubData(rectVbo, 0, sizeof(niaVertex) * 4, v));\
}

u32 rectVao = 0;
u32 rectVeo = 0;
u32 rectVbo = 0;

#include "nia_texture_gl.h"

typedef struct NIA_INTERNAL renderCache {
    u32 cachedHash;
    u32 cachedVaoId;
    u32 cachedTextureId;
    u32 cachedIndicesId;
} renderCache;

renderCache localRenderCache;

#define FNV_PRIME 16777619
#define FNV_OFFSET_BASIS 2166136261

typedef union NIA_INTERNAL r32_u32 {
    r32 r32value;
    u32 u32value;
} r32_u32;

#define HASH_ROUND(_hash, _byte) {\
    _hash ^= _byte;\
    _hash *= FNV_PRIME;\
}

NIA_INLINE u32 NIA_INTERNAL computeHash(r32 x, r32 y,
                                        r32 z, r32 w,
                                        r32 h, r32 colors[3],
                                        u32 vaoId, u32 textureId, u32 veoId){ // FNV-1a hash
    u32 result = FNV_OFFSET_BASIS;

    {
        r32_u32 tmp;
        tmp.r32value = x;
        HASH_ROUND(result, tmp.u32value);

        tmp.r32value = y;
        HASH_ROUND(result, tmp.u32value);

        tmp.r32value = z;
        HASH_ROUND(result, tmp.u32value);

        tmp.r32value = w;
        HASH_ROUND(result, tmp.u32value);

        tmp.r32value = h;
        HASH_ROUND(result, tmp.u32value);

        tmp.r32value = colors[0];
        HASH_ROUND(result, tmp.u32value);

        tmp.r32value = colors[1];
        HASH_ROUND(result, tmp.u32value);

        tmp.r32value = colors[2];
        HASH_ROUND(result, tmp.u32value);
    }
    
    HASH_ROUND(result, vaoId);
    HASH_ROUND(result, textureId);
    HASH_ROUND(result, veoId);

    return result;
}

NIA_INLINE void NIA_INTERNAL cacheInit(renderCache* cache){
    cache->cachedHash = 0xdead;
    cache->cachedVaoId = 0xdead;
    cache->cachedTextureId = 0xdead;
    cache->cachedIndicesId = 0xdead;
}

// To be used later
NIA_INLINE void NIA_INTERNAL cacheSetHash(renderCache* cache, u32 hash){
    cache->cachedHash = hash;
}

NIA_INLINE u32 NIA_INTERNAL cacheGetHash(renderCache* cache){
    return cache->cachedHash;
}

NIA_INLINE void NIA_INTERNAL cacheStoreVaoId(renderCache* cache, u32 vaoId){
    cache->cachedVaoId = vaoId;
}

NIA_INLINE void NIA_INTERNAL cacheStoreTextureId(renderCache* cache, u32 textureId){
    cache->cachedTextureId = textureId;
}

NIA_INLINE void NIA_INTERNAL cacheStoreIndicesId(renderCache* cache, u32 indicesId){
    cache->cachedIndicesId = indicesId;
}
///////////////

#define CACHE_CHECK(_cache, _cached_variable, _variable, _call){\
    if(_cache._cached_variable != _variable){\
        (_call);\
        _cache._cached_variable = _variable;\
    }\
}

niaRenderer::niaRenderer(u32 flags){
    cacheInit(&localRenderCache);

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

    if(flags & NIA_RENDERER_ENABLE_LIGHT){
        defaultShader.useShader();
        defaultShader.setUniform1f("lightIsEnabled", 1);
        defaultShader.unuseShader();

        defaultShaderReflective.useShader();
        defaultShaderReflective.setUniform1f("lightIsEnabled", 1);
        defaultShaderReflective.unuseShader();
    }
}

niaRenderer::~niaRenderer(){
}

void niaRenderer::clear(){
    NIA_GL_CALL(glClearColor(0, 0, 0, 0));
    NIA_GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void niaRenderer::enableDepthTest(){
    NIA_GL_CALL(glEnable(GL_DEPTH_TEST));
}

void niaRenderer::pushOrthographicView(r32 left, r32 right, r32 top, r32 bottom, r32 n, r32 f){
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

void niaRenderer::pushPerspectiveView(r32 fov, r32 aspectRatio, r32 n, r32 f){
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

void niaRenderer::submitTransformation(const niaTransform& transformation, bool transpose){
    defaultShader.useShader();
    defaultShader.setUniformMat4(NIA_UNIFORM_TRANSFORM, transformation.getTransformation(), transpose);
    defaultShader.unuseShader();

    defaultShaderBatch.useShader();
    defaultShaderBatch.setUniformMat4(NIA_UNIFORM_TRANSFORM, transformation.getTransformation(), transpose);
    defaultShaderBatch.unuseShader();

    defaultShaderReflective.useShader();
    defaultShaderReflective.setUniformMat4(NIA_UNIFORM_TRANSFORM, transformation.getTransformation(), transpose);
    defaultShaderReflective.unuseShader();

    defaultShaderFont.useShader();
    defaultShaderFont.setUniformMat4(NIA_UNIFORM_TRANSFORM, transformation.getTransformation(), transpose);
    defaultShaderFont.unuseShader();
}

void niaRenderer::submitClearTransformation(){
    defaultShader.useShader();
    defaultShader.setUniformMat4(NIA_UNIFORM_TRANSFORM, niaMatrix4::identity());
    defaultShader.unuseShader();
}


void niaRenderer::submitView(const niaMatrix4& view, bool transpose){
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

void niaRenderer::renderRectangle(r32 x, r32 y, r32 w, r32 h){
    r32 colors[] = {1, 1, 1};
    renderRectangle(x, y, w, h, colors);
}
// TODO add vector alternative for colors array
void niaRenderer::renderRectangle(r32 x, r32 y, r32 w, r32 h, r32 colors[3]){
    renderRectangle(x, y, 0, w, h, colors);
}

void niaRenderer::renderRectangle(r32 x, r32 y, r32 z, r32 w, r32 h, r32 colors[3]){
    u32 hash = computeHash(x, y, z, w, h, colors, rectVao, defaultTexture.textureId, rectVeo);

    defaultShader.useShader();
    // TODO fix the caching for rectangles
    // CACHE_CHECK(localRenderCache, cachedHash, hash, {
        createAndBufferVertexies(x, y, z, w, h, colors);
        NIA_GL_CALL(glBindVertexArray(rectVao));
        bindTexture(0, defaultTexture.textureId);
        
        NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectVeo));
    // });

    NIA_GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
    defaultShader.unuseShader();
}

void niaRenderer::renderMesh(const niaMesh& mesh){
    NIA_GL_CALL(glBindVertexArray(mesh.vao.id));
    defaultShader.useShader();
    bindTexture(0, defaultTexture.textureId);
    
    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vao.veoId));
    NIA_GL_CALL(glDrawElements(GL_TRIANGLES, mesh.verts, GL_UNSIGNED_SHORT, 0));
    defaultShader.unuseShader();
}

void niaRenderer::renderMesh(const niaMesh& mesh, niaTexture& texture){
    NIA_GL_CALL(glBindVertexArray(mesh.vao.id));
    defaultShader.useShader();
    bindTexture(0, texture.getTextureId());

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vao.veoId));
    NIA_GL_CALL(glDrawElements(GL_TRIANGLES, mesh.verts, GL_UNSIGNED_SHORT, 0));
    defaultShader.unuseShader();
}

void niaRenderer::renderMesh(const niaMesh& mesh, GLuint textureId){
    NIA_GL_CALL(glBindVertexArray(mesh.vao.id));
    defaultShader.useShader();
    bindTexture(0, textureId);

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vao.veoId));
    NIA_GL_CALL(glDrawElements(GL_TRIANGLES, mesh.verts, GL_UNSIGNED_SHORT, 0));
    defaultShader.unuseShader();
}

// The NO-raw functions will NOT call the RAW fuction because we don't know if the compiler will optimize the calls
NIA_CALL void niaRenderer::renderRectangleRaw(r32 x, r32 y, r32 w, r32 h, niaTexture& texture){
    r32 colors[] = {1, 1, 1};
    r32 z = 0;
    createAndBufferVertexies(x, y, z, w, h, colors);

    NIA_GL_CALL(glBindVertexArray(rectVao));

    bindTexture(0, texture.getTextureId());

    NIA_GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
}

void niaRenderer::renderMeshRaw(const niaMesh& mesh){
    NIA_GL_CALL(glBindVertexArray(mesh.vao.id));

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vao.veoId));
    NIA_GL_CALL(glDrawElements(GL_TRIANGLES, mesh.verts, GL_UNSIGNED_SHORT, 0));
}

void niaRenderer::renderMeshRaw(const niaMesh& mesh, const niaTexture& texture){
    NIA_GL_CALL(glBindVertexArray(mesh.vao.id));
    bindTexture(0, texture.textureId);

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vao.veoId));
    NIA_GL_CALL(glDrawElements(GL_TRIANGLES, mesh.verts, GL_UNSIGNED_SHORT, 0));
}
void niaRenderer::renderMeshRaw(const niaMesh& mesh, GLuint textureId){
    NIA_GL_CALL(glBindVertexArray(mesh.vao.id));
    bindTexture(0, textureId);

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vao.veoId));
    NIA_GL_CALL(glDrawElements(GL_TRIANGLES, mesh.verts, GL_UNSIGNED_SHORT, 0));
}

void niaRenderer::renderSkyBox(const niaCubeTexture& texture){
    NIA_GL_CALL(glDepthFunc(GL_LEQUAL));
    NIA_GL_CALL(glBindVertexArray(cubeMesh.vao.id));
    defaultShaderCubeMap.useShader();
    bindTexture(0, texture.textureId, GL_TEXTURE_CUBE_MAP);

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeMesh.vao.veoId));
    NIA_GL_CALL(glDrawElements(GL_TRIANGLES, cubeMesh.verts, GL_UNSIGNED_SHORT, 0));
    defaultShaderCubeMap.unuseShader();
    NIA_GL_CALL(glDepthFunc(GL_LESS));
}

void niaRenderer::renderReflectiveMesh(const niaMesh& mesh, const niaCubeTexture& texture){ // TODO make the cubeTexture not needed
    NIA_GL_CALL(glBindVertexArray(mesh.vao.id));
    defaultShaderReflective.useShader();
    bindTexture(0, texture.textureId, GL_TEXTURE_CUBE_MAP);

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.vao.veoId));
    NIA_GL_CALL(glDrawElements(GL_TRIANGLES, mesh.verts, GL_UNSIGNED_SHORT, 0));
    defaultShaderReflective.unuseShader();
}

void niaRenderer::renderSprite(niaSprite& sprite){
    submitTransformation(sprite.getTransformation());
    
    defaultShader.useShader();

    NIA_GL_CALL(glBindVertexArray(sprite.getVao()));
    bindTexture(0, sprite.getTextureId());
    
    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite.getVeo()));

    NIA_GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
    defaultShader.unuseShader();

    submitClearTransformation();
}

void niaRenderer::renderGlyph(niaGlyph* glyph, const niaVector3<r32>& color){
    // TODO this could be a renderSpriteRAW with the correct shader...
    //      just sayin...
    defaultShaderFont.useShader();
    defaultShaderFont.setUniformVec3("fontColor", color);
    NIA_GL_CALL(glBindVertexArray(glyph->sprite.getVao()));
    bindTexture(0, glyph->sprite.getTextureId());
    
    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glyph->sprite.getVeo()));

    NIA_GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0));
    defaultShaderFont.unuseShader();
}

void niaRenderer::submitDiffuseLightProperties(const niaVector3<r32>& position, const niaVector3<r32>& color){
    defaultShader.useShader();

    defaultShader.setUniformVec3(NIA_UNIFORM_DIFFUSE_LIGHT_POSITION, position);
    defaultShader.setUniformVec3(NIA_UNIFORM_DIFFUSE_LIGHT_COLOR, color);
    
    defaultShader.unuseShader();

    defaultShaderReflective.useShader();

    defaultShaderReflective.setUniformVec3(NIA_UNIFORM_DIFFUSE_LIGHT_POSITION, position);
    defaultShaderReflective.setUniformVec3(NIA_UNIFORM_DIFFUSE_LIGHT_COLOR, color);

    defaultShaderReflective.unuseShader();
}

void niaRenderer::submitSpecularLightProperties(const niaVector3<r32>& position, const niaVector3<r32>& color){
    defaultShader.useShader();

    defaultShader.setUniformVec3(NIA_UNIFORM_SPECULAR_LIGHT_POSITION, position);
    defaultShader.setUniformVec3(NIA_UNIFORM_SPECULAR_LIGHT_COLOR, color);

    defaultShader.unuseShader();

    defaultShaderReflective.useShader();

    defaultShaderReflective.setUniformVec3(NIA_UNIFORM_SPECULAR_LIGHT_POSITION, position);
    defaultShaderReflective.setUniformVec3(NIA_UNIFORM_SPECULAR_LIGHT_COLOR, color);

    defaultShaderReflective.unuseShader();
}

// TODO add case for textures and not just ids because this way animated textures won't work.
void niaRenderer::bindTexture(u32 activeNumber, u32 textureId, GLenum type){ 
    NIA_GL_CALL(glActiveTexture(GL_TEXTURE0 + activeNumber));
    NIA_GL_CALL(glBindTexture(type, textureId));
}

#endif
