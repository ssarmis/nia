/*
 * File: nia_renderer_gl.h
 * Project: Nia
 * File created on May-18-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

/** @file nia_renderer_gl.h
  * @author Streanga Sarmis-Stefan
  * @brief
  * 
  */
#ifdef NIA_RENDERER_GL

#ifndef _NIA_RENDERER_GL_WIN32_H_
#define _NIA_RENDERER_GL_WIN32_H_

#include "nia_mesh.h"
#include "nia_light.h"
#include "nia_sprite.h"
#include "nia_matrix.h"
#include "nia_general.h"
#include "nia_transform.h"
#include "nia_cube_texture.h"
#include "shaders/nia_shader.h"
#include "shaders/nia_shader_cubemap.h"
#include "shaders/nia_shader_quad.h"
#include "shaders/nia_shader_batch.h"
#include "shaders/nia_shader_font.h"
#include "shaders/nia_shader_reflective.h"
#include "parsers/nia_ttf_parser.h"

#define NIA_RENDERER_NO_FLAGS     0x00000000
#define NIA_RENDERER_ENABLE_LIGHT 0x00000001

/**
 *  @class niaRenderer
 *  @author Streanga Sarmis-Stefan
 *  @brief 
 * 
 *  Main Nia renderer used to render primiteves to the screen
 */

NIA_STRUCT lightArray {
  u32 usedLights;
  niaLight lights[NIA_MAXIMUM_LIGHT_SOURCES_I];
} lightArray;

NIA_CLASS niaRenderer {
protected:

    lightArray diffuseLights;
    lightArray specularLights;

    niaMesh cubeMesh;

    niaShader defaultShader;
    niaShaderCubeMap defaultShaderCubeMap;
    niaShaderQuad defaultShaderQuad;
    niaShaderBatch defaultShaderBatch;
    niaShaderFont defaultShaderFont;

    niaShaderReflective defaultShaderReflective;

    niaTexture defaultTexture;
    niaCubeTexture defaultCubeTexture;

    niaMatrix4 viewMatrix;
    niaMatrix4 projectionMatrix;

public:
    NIA_CALL niaRenderer(u32 flags = NIA_RENDERER_NO_FLAGS);
    NIA_CALL ~niaRenderer();

    /**
     * @brief The function if for clearing the viewport 
     */
    void NIA_CALL clear();

    /**
     * @brief The function if for enabling depth test 
     */
     void NIA_CALL enableDepthTest();

    /**
     * @brief Function for rendering a simple rectangle on the screen 
     * @param x A floating point number for the x coordinate.
     * @param y A floating point number for the y coordinate.
     * @param w A floating point number for the width of the rectangle.
     * @param h A floating point number for the height of the rectangle.
     */
     void NIA_CALL renderRectangle(r32 x, r32 y, r32 w, r32 h);

    /**
     * @brief Function for rendering a simple rectangle on the screen 
     * @param x A floating point number for the x coordinate.
     * @param y A floating point number for the y coordinate.
     * @param w A floating point number for the width of the rectangle.
     * @param h A floating point number for the height of the rectangle.
     * @param colors A floating point array of 3 components (red, green, blue) all in a 0 - 1 range.
     */
     void NIA_CALL renderRectangle(r32 x, r32 y, r32 w, r32 h, r32 colors[3]);

    /**
     * @brief Function for rendering a simple rectangle on the screen 
     * @param x A floating point number for the x coordinate.
     * @param y A floating point number for the y coordinate.
     * @param z A floating point number for the z coordinate.
     * @param w A floating point number for the width of the rectangle.
     * @param h A floating point number for the height of the rectangle.
     * @param colors A floating point array of 3 components (red, green, blue) all in a 0 - 1 range.
     */
     void NIA_CALL renderRectangle(r32 x, r32 y, r32 z, r32 w, r32 h, r32 colors[3]);

    /**
     * @brief Function for submiting a transformation to the next mesh that will be drawn
     * @param transformation The transformation to be applied.
     * @param transposed Specifies if you have the transformations in a transposed form(this defaults to true).
     */
     void NIA_CALL submitTransformation(const niaTransform& transformation, bool transpose = true);
     void NIA_CALL submitClearTransformation();

    /**
     * @brief Function for submiting the view for the screen/scene
     * @param view A 4x4 matrix that is the view matrix
     * @param transposed Specifies if you have the matrix in a transposed form(this defaults to true).
     */
     void NIA_CALL submitView(const niaMatrix4& view, bool transpose = true);

    /**
     * @brief Function for enabling an orthographic view for the current renderer 
     * @param left A floating point indicating the left most limit
     * @param right A floating point indicating the right most limit
     * @param top A floating point indicating the top most limit
     * @param bottom A floating point indicating the bottom most limit
     * @param near A floating point indicating the near limit
     * @param far A floating point indicating the far limit
     */
     void NIA_CALL pushOrthographicView(r32 left, r32 right, r32 top, r32 bottom, r32 near, r32 far); // future, custom matrix

    /**
     * @brief Function for enabling the perspective view for the current renderer 
     * @param fov A floating point indicating field of view
     * @param aspectRation A floating point indicating the aspect ratio of the window
     * @param near A floating point indicating the near limit
     * @param far A floating point indicating the far limit
     */
     void NIA_CALL pushPerspectiveView(r32 fov, r32 aspectRatio, r32 near, r32 far);

    /**
     * @brief Function for rendering an already loaded cube texture 
     * @param texture A cube texture to be rendered
     */
     void NIA_CALL renderSkyBox(const niaCubeTexture& texture);

    /**
     * @brief Function for rendering an already loaded mesh
     * @param mesh The mesh to be rendered
     */
     void NIA_CALL renderMesh(const niaMesh& mesh);

    /**
     * @brief Function for rendering an already loaded mesh with some texture
     * @param mesh The mesh to be rendered
     * @param texture The texture to be applied
     */
     void NIA_CALL renderMesh(const niaMesh& mesh, niaTexture& texture);

    /**
     * @brief Function for rendering an already loaded mesh with some texture
     * @param mesh The mesh to be rendered
     * @param textureId An unsinged int number representing an opengl texture id
     */
     void NIA_CALL renderMesh(const niaMesh& mesh, GLuint textureId);

    NIA_CALL void renderRectangleRaw(r32 x, r32 y, r32 w, r32 h, niaTexture& texture);
    
    /**
     * @brief Function for rendering an already loaded mesh, but without any predefined shaders being used
     * @param mesh The mesh to be rendered
     */
     void NIA_CALL renderMeshRaw(const niaMesh& mesh); // for these we asuume the shader was already bounded

    /**
     * @brief Function for rendering an already loaded mesh with a texture, but without any predefined shaders being used
     * @param mesh The mesh to be rendered
     * @param texture The texture to be applied
     */
     void NIA_CALL renderMeshRaw(const niaMesh& mesh, const niaTexture& texture);
    
    /**
     * @brief Function for rendering an already loaded mesh with a texture, but without any predefined shaders being used
     * @param mesh The mesh to be rendered
     * @param textureId An unsinged int number representing an opengl texture id
     */
     void NIA_CALL renderMeshRaw(const niaMesh& mesh, GLuint textureId);

    /**
     * @brief Function for rendering a mesh with a reflective shader applied
     * @param mesh The mesh to be rendered
     * @param texture A cube texture to be rendered as a reflection onto the mesh
     */
    void NIA_CALL renderReflectiveMesh(const niaMesh& mesh, const niaCubeTexture& texture); // TODO make the cubeTexture not needed
    
    void NIA_CALL renderSprite(niaSprite& sprite);
    void NIA_CALL renderGlyph(niaGlyph* glyph, const niaVector3<r32>& color);

    void NIA_CALL updateDiffuseLight(u32 index, const niaVector3<r32>& position, const niaVector3<r32>& color);
    void NIA_CALL updateSpecularLight(u32 index, const niaVector3<r32>& position, const niaVector3<r32>& color);

    u32 NIA_CALL submitDiffuseLightProperties(const niaVector3<r32>& position, const niaVector3<r32>& color);
    u32 NIA_CALL submitDiffuseLightProperties(const niaLight& light);

    u32 NIA_CALL submitSpecularLightProperties(const niaVector3<r32>& position, const niaVector3<r32>& color);
    u32 NIA_CALL submitSpecularLightProperties(const niaLight& light);

    void NIA_CALL bindTexture(u32 activeNumber, u32 textureId, GLenum type = GL_TEXTURE_2D);

    niaMatrix4 NIA_CALL getViewMatrix() const;
    niaMatrix4 NIA_CALL getProjectionMatrix() const;

    friend class niaScene;
};


#endif //_NIA_RENDERER_GL_WIN32_H_
#endif //NIA_RENDERER_GL