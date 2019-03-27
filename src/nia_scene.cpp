#include "nia_scene.h"
#include "nia_renderer.h"

NIA_CALL void niaScene::setAttributeVec2(u32 attribute, const niaVector2<r32>& vector){
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

NIA_CALL void niaScene::setAttributeVec3(u32 attribute, const niaVector3<r32>& vector){
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

NIA_CALL void niaScene::setAttributeMat4(u32 attribute, const niaMatrix4& matrix){
    switch(attribute){
        case NIA_VIEW :{
                view = matrix;
            };
            break;

        default :{
            };
            break;
    }
}

NIA_CALL niaScene::niaScene(){
    diffuseLightColor = niaVector3<r32>();
    diffuseLightPosition = niaVector3<r32>();
}

NIA_CALL niaScene::~niaScene(){
}

NIA_CALL void niaScene::bind(niaRenderer* renderer){
    // niaMatrix4::printMat4(view);

    renderer->submitView(view);

    renderer->defaultShader.useShader();

    renderer->defaultShader.setUniformVec3(NIA_UNIFORM_DIFFUSE_LIGHT_POSITION, diffuseLightPosition);
    renderer->defaultShader.setUniformVec3(NIA_UNIFORM_DIFFUSE_LIGHT_COLOR, diffuseLightColor);

    renderer->defaultShader.setUniformVec3(NIA_UNIFORM_SPECULAR_LIGHT_POSITION, specularLightPosition);
    renderer->defaultShader.setUniformVec3(NIA_UNIFORM_SPECULAR_LIGHT_COLOR, specularLightColor);
}

// NIA_CALL void niaScene::updateView(niaRenderer* renderer){
    
// }

NIA_CALL void niaScene::unbind(niaRenderer* renderer){
    renderer->defaultShader.unuseShader();
}
