/*
 * File: nia_scene.h
 * Project: Nia
 * File created on March-23-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef _NIA_SCENE_H_
#define _NIA_SCENE_H_

#include "nia_general.h"

#define NIA_DIFFUSE_LIGHT_POSITION  0x00000000
#define NIA_DIFFUSE_LIGHT_COLOR     0x00000001

#define NIA_SPECULAR_LIGHT_POSITION  0x00000002
#define NIA_SPECULAR_LIGHT_COLOR     0x00000004

#define NIA_VIEW    0x00000001

#include "nia_vector.h"
#include "nia_matrix.h"
#include "shaders/nia_shader.h"
#include "nia_renderer.h"
#include "nia_constants.h"
#include "nia_frame_buffer.h"
#include "nia_cube_texture.h"

NIA_CLASS niaScene {
protected:

    niaVector3<r32> diffuseLightColor;
    niaVector3<r32> diffuseLightPosition;

    niaVector3<r32> specularLightPosition;
    niaVector3<r32> specularLightColor;

    niaMatrix4 view;

    niaCubeTexture cubeTexture;

    void setSkyBoxTextures(const char* textures[6]);

    void setAttributeVec2(u32 attribute, const niaVector2<r32>& vector);
    void setAttributeVec3(u32 attribute, const niaVector3<r32>& vector);
    void setAttributeMat4(u32 attribute, const niaMatrix4& matrix);

public:
    niaFrameBuffer frameBuffer;
    niaScene();
    ~niaScene();

    void NIA_CALL bind(niaRenderer* renderer);
    void NIA_CALL unbind(niaRenderer* renderer);

    niaFrameBuffer NIA_CALL getFrameBuffer() const; // consider using references
};


#endif // _NIA_SCENE_H_