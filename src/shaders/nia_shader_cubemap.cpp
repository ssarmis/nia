#include "shaders/nia_shader_cubemap.h"

#include <stdio.h>
#include <malloc.h>

NIA_STATIC char* basicCubeMapVertexShader = ""
NIA_GLSL_VERSION" \n""precision "NIA_GLSL_PRECISION" float;\n"
"layout(location = 0) in vec3 pos;\n"

"out "NIA_GLSL_PRECISION" vec3 o_uv;\n"

"uniform "NIA_GLSL_PRECISION" mat4 mP;\n"
"uniform "NIA_GLSL_PRECISION" mat4 mV;\n"

"void main(){\n"
"   gl_Position = mP * mV * vec4(pos, 1.0);\n"
"   o_uv = pos;\n"
"}\n"
"";

NIA_STATIC char* basicCubeMapFragmentShader = ""
NIA_GLSL_VERSION" \n""precision "NIA_GLSL_PRECISION" float;\n"

"out "NIA_GLSL_PRECISION" vec4 finalColor;\n"

"in "NIA_GLSL_PRECISION" vec3 o_uv;\n"

"uniform "NIA_GLSL_PRECISION" samplerCube tex;\n"

"void main(){\n"
"   finalColor = texture(tex, o_uv);\n"
"}\n"
"";

NIA_CALL niaShaderCubeMap::niaShaderCubeMap(){
    vertexShader = loadBufferToShader(basicCubeMapVertexShader, GL_VERTEX_SHADER);
    fragmentShader = loadBufferToShader(basicCubeMapFragmentShader, GL_FRAGMENT_SHADER);

    compileShader(vertexShader);
    compileShader(fragmentShader);
    buildProgram(vertexShader, fragmentShader);
}

NIA_CALL niaShaderCubeMap::~niaShaderCubeMap(){
    // TODO free source files loaded in memory after using them
    NIA_GL_CALL(glDeleteShader(vertexShader));
    NIA_GL_CALL(glDeleteShader(fragmentShader));
    NIA_GL_CALL(glDeleteProgram(program));
}
