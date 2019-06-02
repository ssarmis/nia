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
"}\n"
"";

NIA_STATIC char* basicReflectiveFragmentShader = ""
NIA_GLSL_VERSION" \n""precision "
NIA_GLSL_PRECISION" float;\n"

"out vec4 finalColor;\n"

"in  vec3 o_uv;\n"
"in  vec3 o_color;\n"

"uniform samplerCube tex;\n"

"void main(){\n"
"   finalColor = texture(tex, o_uv) * vec4(o_color, 1.0);\n"
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
