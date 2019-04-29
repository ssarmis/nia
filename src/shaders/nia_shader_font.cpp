#include "shaders/nia_shader_font.h"

#include <stdio.h>
#include <malloc.h>

NIA_STATIC char* basicVertexShaderFont = ""
"#version 440 core\n"
"layout(location = 0) in vec3 pos;\n"
"layout(location = 1) in vec3 color;\n"
"layout(location = 2) in vec2 uv;\n"

"uniform mat4 mP;\n"
"uniform mat4 mT;\n"
"uniform mat4 mV;\n"

"out vec4 o_color;\n"

"out vec2 o_uv;\n"

"void main(){\n"
"   vec4 transformedPosition = mT * vec4(pos, 1.0);\n"
"   gl_Position = mP * mV * transformedPosition;\n"
"   o_color = vec4(color, 1.0);\n"
"   o_uv = uv;\n"
"}\n"
"";

NIA_STATIC char* basicFragmentShaderFont = ""
"#version 440 core\n"

"out vec4 finalColor;\n"

"in vec4 o_color;\n"

"in vec2 o_uv;\n"

"uniform sampler2D tex;\n"

"void main(){\n"
"   finalColor = texture(tex, o_uv);\n"
"   if (finalColor.x != 1){\n"
"       discard;\n"
"   }\n"
"   finalColor *=  o_color;"
"}\n"
"";


NIA_CALL niaShaderFont::niaShaderFont(){
    vertexShader = loadBufferToShader(basicVertexShaderFont, GL_VERTEX_SHADER);
    fragmentShader = loadBufferToShader(basicFragmentShaderFont, GL_FRAGMENT_SHADER);

    compileShader(vertexShader);
    compileShader(fragmentShader);
    buildProgram(vertexShader, fragmentShader);
}

NIA_CALL niaShaderFont::~niaShaderFont(){
}
