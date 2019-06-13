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

"uniform vec3 dlP["NIA_MAXIMUM_LIGHT_SOURCES"];\n"
"uniform vec3 dlC["NIA_MAXIMUM_LIGHT_SOURCES"];\n"

"uniform vec3 slP["NIA_MAXIMUM_LIGHT_SOURCES"];\n"
"uniform vec3 slC["NIA_MAXIMUM_LIGHT_SOURCES"];\n"

"uniform float lightIsEnabled;\n"

"varying float o_lightIsEnabled;\n"

"uniform int diffuseLightsUsed;\n"
"uniform int specularLightsUsed;\n"

"out int o_diffuseLightsUsed;\n"
"out int o_specularLightsUsed;\n"

"out vec3 o_dlC["NIA_MAXIMUM_LIGHT_SOURCES"];\n"
"out vec3 o_slC["NIA_MAXIMUM_LIGHT_SOURCES"];\n"

"out float o_lightFactor["NIA_MAXIMUM_LIGHT_SOURCES"];\n"
"out float o_lightSpecularFactor["NIA_MAXIMUM_LIGHT_SOURCES"];\n"

"out vec3 o_uv;\n"
"out vec3 o_color;\n"

"float computeDiffuseFactor(vec3 lightPosition, vec3 transformedVertPosition){\n"
"   float result = 0;\n"

"   vec3 lightVector = (lightPosition - transformedVertPosition);\n"
"   vec3 normalizedLightVector = normalize(lightVector);\n"

"   mat3 normalMatrix = transpose(inverse(mat3(mT)));\n"
"   vec3 transformedNormal = (normalMatrix * normal);\n"
"   vec3 normlizedTransformedNormal = normalize(transformedNormal);\n"

"   result = dot(normalizedLightVector, normlizedTransformedNormal);\n"
"   result = max(result, 0.7);\n"

"   return result;\n"
"}\n"

"float computeSpecularFactor(vec3 lightPosition, vec3 transformedVertPosition){"
"   vec4 transformedPosition = mT * vec4(pos, 1.0);\n" // replace later
"   float result = 0;\n"
"   vec3 specularLightVector = (lightPosition - transformedVertPosition);\n"
"   mat3 normalMatrix = transpose(inverse(mat3(mT)));\n"
"   vec3 transformedNormal = (normalMatrix * normal);\n"
"   vec3 normlizedTransformedNormal = normalize(transformedNormal);\n"
// TODO this should be reflect() but its probably just as fast
"   vec3 reflectedCameraVector = specularLightVector - 2 * (dot(specularLightVector, normal)"
"                                * normal);\n"
"   reflectedCameraVector = normalize(reflectedCameraVector);\n"
"   vec4 viewVector = normalize(inverse(mV)[3] - transformedPosition);\n"

"   result = dot(-reflectedCameraVector, viewVector.xyz);\n"
"   result = pow(max(result, 0.0), 32);\n"
"   return result;\n"
"}\n"

"void main(){\n"
"   vec4 transformedPosition = mT * vec4(pos, 1.0);"
"   vec4 viewVector = inverse(mV)[3] - transformedPosition;\n"
"   viewVector = normalize(viewVector);\n"
    
"   gl_Position = mP * mV * transformedPosition;\n"

"   vec3 normalTransformed = transformedPosition.xyz * normal;\n"
"   normalTransformed = normalize(normalTransformed);\n"

"   //viewVector.y *= -1;\n"

"   vec3 reflectedVector = reflect(viewVector.xyz, normalTransformed);\n"
"   o_uv = reflectedVector;\n"
"   o_color = color;\n"
// light same as in default shader
"   o_lightIsEnabled = lightIsEnabled;\n"

"   for(int i = 0; i < diffuseLightsUsed; ++i){\n"
"       o_dlC[i] = dlC[i];\n"
"   }\n"
"   for(int i = 0; i < specularLightsUsed; ++i){\n"
"       o_slC[i] = slC[i];\n"
"   }\n"
"   o_lightIsEnabled = lightIsEnabled;\n"
"   o_diffuseLightsUsed = diffuseLightsUsed;\n"
"   o_specularLightsUsed = specularLightsUsed;\n"

"   if(lightIsEnabled > 0){\n"
"        for(int i = 0; i < diffuseLightsUsed; ++i){\n"
"            o_lightFactor[i] = computeDiffuseFactor(dlP[i], transformedPosition.xyz);\n"
"        }\n"
"        for(int i = 0; i < specularLightsUsed; ++i){\n"
"            o_lightSpecularFactor[i] = computeSpecularFactor(slP[i], transformedPosition.xyz);\n"
"        }\n"
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

"flat in int o_diffuseLightsUsed;\n"
"flat in int o_specularLightsUsed;\n"

"in vec3 o_dlC["NIA_MAXIMUM_LIGHT_SOURCES"];\n"
"in vec3 o_slC["NIA_MAXIMUM_LIGHT_SOURCES"];\n"

"in float o_lightFactor["NIA_MAXIMUM_LIGHT_SOURCES"];\n"
"in float o_lightSpecularFactor["NIA_MAXIMUM_LIGHT_SOURCES"];\n"

"vec4 computeDiffuseOutput(float diffuseLightFactor, vec3 diffuseColor){\n"
"   return (diffuseLightFactor * vec4(diffuseColor, 1.0));\n"
"}\n"

"vec4 computeSpecularOutput(float specularLightFactor, vec3 specularColor){"
"   return (1 * specularLightFactor * vec4(specularColor, 1.0));\n"
"}"

"uniform samplerCube tex;\n"

"void main(){\n"
"   vec3 finalUvs = o_uv;\n"
"   finalUvs.y = -1 * finalUvs.y;\n"
"   if(o_lightIsEnabled > 0){\n"
"      finalColor = vec4(0, 0, 0, 0);\n"
"      for(int i = 0; i < o_diffuseLightsUsed; ++i){\n"
"          finalColor += computeDiffuseOutput(o_lightFactor[i], o_dlC[i]);\n"
"      }\n"
"      for(int i = 0; i < o_specularLightsUsed; ++i){\n"
"          finalColor += computeSpecularOutput(o_lightSpecularFactor[i], o_slC[i]);\n"
"      }\n"
"      finalColor *= (texture(tex, -finalUvs) * vec4(o_color, 1.0));\n"
"   } else {\n"
"      finalColor = (texture(tex, -finalUvs) * vec4(o_color, 1.0));\n"
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
