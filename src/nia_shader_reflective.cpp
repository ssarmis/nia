#include "nia_shader_reflective.h"

#include <stdio.h>
#include <malloc.h>

NIA_STATIC char* basicReflectiveVertexShader = ""
"#version 440 core\n"
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
"   reflectedVector.y *= -1;\n"
"   o_uv = reflectedVector;\n"
"   o_color = color;\n"
"}\n"
"";

NIA_STATIC char* basicReflectiveFragmentShader = ""
"#version 440 core\n"

"out vec4 finalColor;\n"

"in vec3 o_uv;\n"
"in vec3 o_color;\n"

"uniform samplerCube tex;\n"

"void main(){\n"
"   finalColor = texture(tex, o_uv) * vec4(o_color, 1.0);\n"
"}\n"
"";

NIA_CALL niaShaderReflective::niaShaderReflective(){
    vertexShader = loadBufferToShader(basicReflectiveVertexShader, GL_VERTEX_SHADER);
    fragmentShader = loadBufferToShader(basicReflectiveFragmentShader, GL_FRAGMENT_SHADER);

    compileShader(vertexShader);
    compileShader(fragmentShader);
    buildProgram(vertexShader, fragmentShader);
}

NIA_CALL niaShaderReflective::~niaShaderReflective(){
    // TODO free source files loaded in memory after using them
    NIA_GL_CALL(glDeleteShader(vertexShader));
    NIA_GL_CALL(glDeleteShader(fragmentShader));
    NIA_GL_CALL(glDeleteProgram(program));
}
