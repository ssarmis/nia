/*
 * File: nia_shader_reflective.cpp
 * Project: Nia
 * File created on April-29-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "shaders/nia_shader_reflective.h"

#include <stdio.h>
#include <malloc.h>

NIA_STATIC char* basicReflectiveVertexShader = ""
NIA_GLSL_VERSION" \n""precision "
NIA_GLSL_PRECISION" float;\n"
"layout(location = 0) in vec3 pos;\n"
"layout(location = 1) in vec3 color;\n"
"layout(location = 2) in vec3 normal;\n"
"layout(location = 3) in vec2 uv;\n"

"uniform mat4 mP;\n"
"uniform mat4 mT;\n"
"uniform mat4 mV;\n"

"uniform vec3 dlP;\n"
"uniform vec3 dlC;\n"

"uniform vec3 slP;\n"
"uniform vec3 slC;\n"

"uniform float lightIsEnabled;\n"

"varying float o_lightIsEnabled;\n"

"out vec3 o_dlP;\n"
"out vec3 o_dlC;\n"
"out vec3 o_slP;\n"
"out vec3 o_slC;\n"

"out float o_lightFactor;\n"
"out float o_lightSpecularFactor;\n"

"out vec3 o_uv;\n"
"out vec3 o_color;\n"

"void main(){\n"
"   vec4 transformedPosition = mT * vec4(pos, 1.0);"
"   vec4 viewVector = inverse(mV)[3] - transformedPosition;\n"
"   viewVector = normalize(viewVector);\n"
    
"   gl_Position = mP * mV * transformedPosition;\n"

"   vec3 normalTransformed = transformedPosition.xyz * normal;\n"
"   normalTransformed = normalize(normalTransformed);\n"

"   //viewVector.y *= -1;\n"

"   vec3 reflectedVector = reflect(viewVector.xyz, normalTransformed);\n"
"   reflectedVector.y *= -1.0;\n"
"   o_uv = reflectedVector;\n"
"   o_color = color;\n"
// light same as in default shader
"   o_dlP = dlP;\n"
"   o_dlC = dlC;\n"
"   o_slP = slP;\n"
"   o_slC = slC;\n"
"   o_lightIsEnabled = lightIsEnabled;\n"
"   if(lightIsEnabled > 0){\n"
"      vec3 lightVector = (dlP - transformedPosition.xyz);\n"
"      vec3 normalizedLightVector = normalize(lightVector);\n"

"      mat3 normalMatrix = transpose(inverse(mat3(mT)));\n"
"      vec3 transformedNormal = (normalMatrix * normal);\n"
"      vec3 normlizedTransformedNormal = normalize(transformedNormal);\n"

"      o_lightFactor = dot(normalizedLightVector, normlizedTransformedNormal);\n"
"      o_lightFactor = max(o_lightFactor, 0.5);\n"

"      vec3 specularLightVector = (slP - transformedPosition.xyz);\n"
"      vec3 reflectedCameraVector = specularLightVector - 2 * (dot(specularLightVector, normlizedTransformedNormal) * normlizedTransformedNormal);\n"
"      reflectedCameraVector = normalize(reflectedCameraVector);\n"
"      vec4 normalizedViewVector = normalize(inverse(mV)[3] - transformedPosition);\n"
"      o_lightSpecularFactor = dot(-reflectedCameraVector, normalizedViewVector.xyz);\n"
"      o_lightSpecularFactor = pow(max(o_lightSpecularFactor, 0.0), 8);\n"
"   }\n"
"}\n"
"";

NIA_STATIC char* basicReflectiveFragmentShader = ""
NIA_GLSL_VERSION" \n""precision "
NIA_GLSL_PRECISION" float;\n"

"out vec4 finalColor;\n"

"in vec3 o_uv;\n"
"in vec3 o_color;\n"

"varying float o_lightIsEnabled;\n"

"in vec3 o_dlP;\n"
"in vec3 o_dlC;\n"
"in vec3 o_slP;\n"
"in vec3 o_slC;\n"
"in float o_lightFactor;\n"
"in float o_lightSpecularFactor;\n"

"uniform samplerCube tex;\n"

"void main(){\n"
"   if(o_lightIsEnabled > 0){\n"
"      vec4 diffuse = (o_lightFactor * vec4(o_dlC, 1.0));\n"
"      vec4 specular = (1 * o_lightSpecularFactor * vec4(o_slC, 1.0));\n"
"      finalColor = (specular + diffuse) * (texture(tex, -o_uv) * vec4(o_color, 1.0));\n"
"   } else {\n"
"      finalColor = (texture(tex, -o_uv) * vec4(o_color, 1.0));\n"
"   }"
"}\n"
"";

niaShaderReflective::niaShaderReflective(){
    vertexShader = loadBufferToShader(basicReflectiveVertexShader, GL_VERTEX_SHADER);
    fragmentShader = loadBufferToShader(basicReflectiveFragmentShader, GL_FRAGMENT_SHADER);

    compileShader(vertexShader);
    compileShader(fragmentShader);
    buildProgram(vertexShader, fragmentShader);
}

niaShaderReflective::~niaShaderReflective(){
}
