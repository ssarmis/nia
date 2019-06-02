/*
 * File: nia_scene.cpp
 * Project: Nia
 * File created on March-25-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "nia_scene.h"
#include "nia_renderer.h"

void niaScene::setAttributeVec2(u32 attribute, const niaVector2<r32>& vector){
    switch(attribute){
        case NIA_DIFFUSE_LIGHT_POSITION :{
                diffuseLightPosition = niaVector3<r32>(vector.x, vector.y, 0);
            };
            break;

        case NIA_DIFFUSE_LIGHT_COLOR :{
                diffuseLightColor = niaVector3<r32>(vector.x, vector.y, 0);
            };
            break;

        default :{
            };
            break;
    }
}

void niaScene::setAttributeVec3(u32 attribute, const niaVector3<r32>& vector){
    switch(attribute){
        case NIA_DIFFUSE_LIGHT_POSITION :{
                diffuseLightPosition = vector;
            };
            break;

        case NIA_DIFFUSE_LIGHT_COLOR :{
                diffuseLightColor = vector;
            };
            break;

        default :{
            };
            break;
    }
}

void niaScene::setAttributeMat4(u32 attribute, const niaMatrix4& matrix){
    switch(attribute){
        case NIA_VIEW :{
            };
            break;

        default :{
            };
            break;
    }
}

void niaScene::setSkyBoxTextures(const char* textures[]){
    cubeTexture = niaCubeTexture(textures);
}

niaScene::niaScene(){
    diffuseLightColor = niaVector3<r32>();
    diffuseLightPosition = niaVector3<r32>();
}

niaScene::~niaScene(){
}

void niaScene::bind(niaRenderer* renderer){
    renderer->defaultShader.useShader();

    renderer->defaultShader.setUniformVec3(NIA_UNIFORM_DIFFUSE_LIGHT_POSITION, diffuseLightPosition);
    renderer->defaultShader.setUniformVec3(NIA_UNIFORM_DIFFUSE_LIGHT_COLOR, diffuseLightColor);

    renderer->defaultShader.setUniformVec3(NIA_UNIFORM_SPECULAR_LIGHT_POSITION, specularLightPosition);
    renderer->defaultShader.setUniformVec3(NIA_UNIFORM_SPECULAR_LIGHT_COLOR, specularLightColor);
    
    renderer->defaultShader.unuseShader();

    renderer->renderSkyBox(cubeTexture);
}

// void niaScene::updateView(niaRenderer* renderer){
    
// }

void niaScene::unbind(niaRenderer* renderer){
    // renderer->defaultShader.unuseShader();
}

niaFrameBuffer niaScene::getFrameBuffer() const {
    return frameBuffer;
}