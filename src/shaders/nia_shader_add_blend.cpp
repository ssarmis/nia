/*
 * File: nia_shader_add_blend.cpp
 * Project: Nia
 * File created on June-06-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

/*
 * File: nia_shader.cpp
 * Project: Nia
 * File created on April-29-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "shaders/nia_shader_add_blend.h"

NIA_STATIC char* blendAddVertex = ""
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
"   vec4 position = mP * mV * mT * vec4(pos, 1.0);\n" // why the hell didn't I use this? Can't remember now, me from the future, its because the matrixes are not set
"   gl_Position = vec4(pos, 1.0);\n"
"   o_color = vec4(color, 1.0);\n"
"   o_uv = uv;\n"
"}\n"
"";

NIA_STATIC char* blendAddFragment = ""
NIA_GLSL_VERSION" \n""precision "
NIA_GLSL_PRECISION" float;\n"

"out vec4 finalColor;\n"

"in vec4 o_color;\n"

"in vec2 o_uv;\n"

"uniform sampler2D tex;\n"
"uniform sampler2D tex2;\n"

"void main(){\n"
"   finalColor = texture(tex, o_uv) + texture(tex2, o_uv);\n"
"}\n"
"";

niaShaderAddBlend::niaShaderAddBlend(){
    vertexShader = loadBufferToShader(blendAddVertex, GL_VERTEX_SHADER);
    fragmentShader = loadBufferToShader(blendAddFragment, GL_FRAGMENT_SHADER);

    compileShader(vertexShader);
    compileShader(fragmentShader);
    buildProgram(vertexShader, fragmentShader);
}

niaShaderAddBlend::~niaShaderAddBlend(){
}
