#include "nia_shader.h"

#include <stdio.h>
#include <malloc.h>

NIA_STATIC char* basicVertexShader = ""
"#version 440 core\n"
"layout(location = 0) in vec3 pos;\n"
"layout(location = 1) in vec3 color;\n"
"layout(location = 2) in vec3 normal;\n"
"layout(location = 3) in vec2 uv;\n"
"uniform mat4 mP;\n"
"uniform mat4 mT;\n"
"out vec4 o_color;\n"
"void main(){\n"
"   gl_Position = mP * mT * vec4(pos, 1.0);\n"
"   o_color = vec4(color, 1.0);\n"
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

NIA_CALL niaShader::niaShader(const char* vertexShaderFilename, const char* fragmentShaderFilename){
    vertexShader = loadSourceToShader(vertexShaderFilename, GL_VERTEX_SHADER);
    fragmentShader = loadSourceToShader(fragmentShaderFilename, GL_FRAGMENT_SHADER);

    compileShader(vertexShader);
    compileShader(fragmentShader);
    buildProgram(vertexShader, fragmentShader);
}

NIA_CALL niaShader::niaShader(){
    vertexShader = loadBufferToShader(basicVertexShader, GL_VERTEX_SHADER);
    fragmentShader = loadBufferToShader(basicFragmentShader, GL_FRAGMENT_SHADER);

    compileShader(vertexShader);
    compileShader(fragmentShader);
    buildProgram(vertexShader, fragmentShader);
}

NIA_CALL niaShader::~niaShader(){
    // TODO free source files loaded in memory after using them
    NIA_GL_CALL(glDeleteShader(vertexShader));
    NIA_GL_CALL(glDeleteShader(fragmentShader));
    NIA_GL_CALL(glDeleteProgram(program));
}

NIA_INTERNAL GLuint niaShader::loadBufferToShader(const char* source, GLenum type){
    GLuint id = glCreateShader(type);
    GLint size = nia_strlen(source);
    glShaderSource(id, 1, &source, &size);
    return id;
}

NIA_INTERNAL GLuint niaShader::loadSourceToShader(const char* filename, GLenum type){
    FILE* sourceFile = fopen(filename, "rb");

    fseek(sourceFile, 0, SEEK_END);
    GLint size = ftell(sourceFile);
    fseek(sourceFile, 0, SEEK_SET);

    GLchar* source = (GLchar*)malloc((size + 1) * sizeof(GLchar));
    fread(source, sizeof(GLchar), size, sourceFile);
    source[size] = 0;
    fclose(sourceFile);

    GLuint id = glCreateShader(type);
    NIA_GL_CALL(glShaderSource(id, 1, &source, &size));
    return id;
}

NIA_INTERNAL void niaShader::compileShader(GLuint shader){
    NIA_GL_CALL(glCompileShader(shader));

    GLint status;

    NIA_GL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &status));

    if (status != GL_TRUE){
        GLsizei logLength = 0;
        GLchar log[1024] = {0};
        NIA_GL_CALL(glGetShaderInfoLog(shader, 1024, &logLength, log));
        NIA_ERROR("%s\n", log);
    }
}

NIA_INTERNAL void niaShader::buildProgram(GLuint vertexShader, GLuint fragmentShader){
    program = glCreateProgram();

    NIA_GL_CALL(glAttachShader(program, vertexShader));
    NIA_GL_CALL(glAttachShader(program, fragmentShader));
    NIA_GL_CALL(glLinkProgram(program));
    NIA_GL_CALL(glValidateProgram(program));

    GLint status;
    NIA_GL_CALL(glGetProgramiv(program, GL_LINK_STATUS, &status));
    if (status != GL_TRUE){
        GLsizei logLength = 0;
        GLchar log[1024] = {0};
        NIA_GL_CALL(glGetProgramInfoLog(program, 1024, &logLength, log));
        NIA_ERROR("%s\n", log);
    }

    NIA_GL_CALL(glGetProgramiv(program, GL_VALIDATE_STATUS, &status));
    if (status != GL_TRUE){
        GLsizei logLength = 0;
        GLchar log[1024] = {0};
        NIA_GL_CALL(glGetProgramInfoLog(program, 1024, &logLength, log));
        NIA_ERROR("%s\n", log);
    }
}

NIA_CALL GLuint niaShader::getProgram() const {
    return program;
}

NIA_CALL GLuint niaShader::getVertexShader() const {
    return vertexShader;
}

NIA_CALL GLuint niaShader::getFragmentShader() const {
    return fragmentShader;
}


NIA_CALL void niaShader::useShader(){
    NIA_GL_CALL(glUseProgram(program));
}

NIA_CALL void niaShader::unuseShader(){
    NIA_GL_CALL(glUseProgram(0));
}

NIA_CALL void niaShader::setUniformMat4(const char* name, const niaMatrix4& mat, bool transpose){
    GLint id = glGetUniformLocation(program, name);
    NIA_GL_CALL(glUniformMatrix4fv(id, 1, transpose, mat.m));
}