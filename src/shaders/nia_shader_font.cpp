#include "shaders/nia_shader_font.h"

#include <stdio.h>
#include <malloc.h>

NIA_STATIC char* basicVertexShaderFont = ""
NIA_GLSL_VERSION" \n""precision "NIA_GLSL_PRECISION" float;\n"
"layout(location = 0) in vec3 pos;\n"
"layout(location = 1) in vec3 color;\n"
"layout(location = 2) in vec2 uv;\n"

"uniform "NIA_GLSL_PRECISION" mat4 mP;\n"
"uniform "NIA_GLSL_PRECISION" mat4 mT;\n"
"uniform "NIA_GLSL_PRECISION" mat4 mV;\n"

"out "NIA_GLSL_PRECISION" vec4 o_color;\n"

"out "NIA_GLSL_PRECISION" vec2 o_uv;\n"

"void main(){\n"
"   vec4 transformedPosition = mT * vec4(pos, 1.0);\n"
"   gl_Position = mP * mV * transformedPosition;\n"
"   o_color = vec4(color, 1.0);\n"
"   o_uv = uv;\n"
"}\n"
"";

NIA_STATIC char* basicFragmentShaderFont = ""
NIA_GLSL_VERSION" \n""precision "NIA_GLSL_PRECISION" float;\n"

"out "NIA_GLSL_PRECISION" vec4 finalColor;\n"

"in "NIA_GLSL_PRECISION" vec4 o_color;\n"

"in "NIA_GLSL_PRECISION" vec2 o_uv;\n"

"uniform "NIA_GLSL_PRECISION" sampler2D tex;\n"

"void main(){\n"
"   finalColor = texture(tex, o_uv);\n"
"   if (finalColor.x != 1.0){\n"
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
