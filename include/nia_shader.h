#ifndef _NIA_SHADER_H_
#define _NIA_SHADER_H_

#include "nia_gl.h"

#include "nia_general.h"

NIA_STATIC char* basicVertexShader = ""
"#version 440 core\n"
"layout(location = 0) in vec3 pos;\n"
"layout(location = 1) in vec3 color;\n"
"out vec4 o_color;\n"
"void main(){\n"
"   gl_Position = vec4(pos, 1.0);\n"
"   o_color = vec4(pos, 1.0);\n"
"}\n"
"";

NIA_STATIC char* basicFragmentShader = ""
"#version 440 core\n"
"out vec4 finalColor;\n"
"in vec4 o_color;\n"
"void main(){\n"
"  finalColor = o_color;\n"
"}\n"
"";

NIA_CLASS niaShader {
private:
    GLuint vertexShader; 
    GLuint fragmentShader;
    GLuint program;

    NIA_INTERNAL GLuint loadBufferToShader(const char* source, GLenum type);
    NIA_INTERNAL GLuint loadSourceToShader(const char* filename, GLenum type);
    NIA_INTERNAL void compileShader(GLuint shader);
    NIA_INTERNAL void buildProgram(GLuint vertexShader, GLuint fragmentShader);

public:
    NIA_CALL niaShader(const char* vertexShaderFilename, const char* fragmentShaderFilename);
    NIA_CALL niaShader();
    NIA_CALL ~niaShader();

    NIA_CALL void useShader();
    NIA_CALL void unuseShader();
};

#endif // _NIA_BATCH_RENDERER_H_