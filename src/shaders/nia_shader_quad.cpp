/*
 * File: nia_shader_quad.cpp
 * Project: Nia
 * File created on April-29-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "shaders/nia_shader_quad.h"


NIA_STATIC char* basicQaudVertex = ""
NIA_GLSL_VERSION" \n""precision "
NIA_GLSL_PRECISION" float;\n"
"layout(location = 0) in vec3 pos;\n"
"layout(location = 1) in vec3 color;\n"
"layout(location = 2) in vec3 normal;\n"
"layout(location = 3) in vec2 uv;\n"

"uniform mat4 mP;\n"
"uniform mat4 mT;\n"
"uniform mat4 mV;\n"

"out vec4 o_color;\n"

"out vec2 o_uv;\n"

"void main(){\n"
"   vec4 position = mP * mV * mT * vec4(pos, 1.0);\n" // why the hell didn't I use this? Can't remember now
"   gl_Position = vec4(pos, 1.0);\n"
"   o_color = vec4(color, 1.0);\n"
"   o_uv = uv;\n"
"}\n"
"";

NIA_STATIC char* basicQaudFragment = ""
NIA_GLSL_VERSION" \n""precision "
NIA_GLSL_PRECISION" float;\n"

"out vec4 finalColor;\n"

"in vec4 o_color;\n"

"in vec2 o_uv;\n"

"uniform sampler2D tex;\n"

"void main(){\n"
// "   finalColor = texture(tex, o_uv) * o_color;\n"
"   finalColor = texture(tex, o_uv);\n"
"}\n"
"";


niaShaderQuad::niaShaderQuad(){
    vertexShader = loadBufferToShader(basicQaudVertex, GL_VERTEX_SHADER);
    fragmentShader = loadBufferToShader(basicQaudFragment, GL_FRAGMENT_SHADER);

    compileShader(vertexShader);
    compileShader(fragmentShader);
    buildProgram(vertexShader, fragmentShader);
}

niaShaderQuad::~niaShaderQuad(){
}
