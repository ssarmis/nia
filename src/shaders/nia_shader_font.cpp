/*
 * File: nia_shader_font.cpp
 * Project: Nia
 * File created on April-29-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "shaders/nia_shader_font.h"

#include <stdio.h>
#include <malloc.h>

NIA_STATIC char* basicVertexShaderFont = ""
NIA_GLSL_VERSION" \n""precision "
NIA_GLSL_PRECISION" float;\n"
"layout(location = 0) in vec3 pos;\n"
"layout(location = 1) in vec3 color;\n"
"layout(location = 2) in vec3 normals;\n"
"layout(location = 3) in vec2 uv;\n"

"uniform mat4 mP;\n"
"uniform mat4 mT;\n"
"uniform mat4 mV;\n"

"out vec4 o_color;\n"

"uniform vec3 fontColor;\n"

"out vec2 o_uv;\n"

"out vec3 o_fontColor;\n"

"void main(){\n"
"   vec4 transformedPosition = mT * vec4(pos, 1.0);\n"
"   gl_Position = mP * mV * transformedPosition;\n"
"   o_color = vec4(color, 1.0);\n"
"   o_uv = uv;\n"
"   o_fontColor = fontColor;\n"
"}\n"
"";

NIA_STATIC char* basicFragmentShaderFont = ""
NIA_GLSL_VERSION" \n""precision "
NIA_GLSL_PRECISION" float;\n"

"out vec4 finalColor;\n"

"in vec4 o_color;\n"
"in vec3 o_fontColor;\n"

"in vec2 o_uv;\n"

"uniform sampler2D tex;\n"

"void main(){\n"
// "   vec2 copy = o_uv;\n"
// "   copy.y = 1 - copy.y;\n"
"   finalColor = texture(tex, o_uv);\n"
"   if (finalColor.x != 1.0){\n"
"       discard;\n"
"   }\n"
"   finalColor = vec4(1.0, 1.0, 1.0, 1.0);\n" // reset the color here, so its not red, ask the opengl people why they don't have a greyscale color for one byte only...
"   finalColor *= vec4(o_fontColor, 1.0);\n"
"}\n"
"";

niaShaderFont::niaShaderFont(){
    vertexShader = loadBufferToShader(basicVertexShaderFont, GL_VERTEX_SHADER);
    fragmentShader = loadBufferToShader(basicFragmentShaderFont, GL_FRAGMENT_SHADER);

    compileShader(vertexShader);
    compileShader(fragmentShader);
    buildProgram(vertexShader, fragmentShader);
}

niaShaderFont::~niaShaderFont(){
}
