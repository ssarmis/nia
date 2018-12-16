#include "nia_shader.h"

#include <stdio.h>
#include <malloc.h>

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
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(program);
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
    glShaderSource(id, 1, &source, &size);
    return id;
}

NIA_INTERNAL void niaShader::compileShader(GLuint shader){
    glCompileShader(shader);

    GLint status;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status != GL_TRUE){
        GLsizei logLength = 0;
        GLchar log[1024] = {0};
        glGetShaderInfoLog(shader, 1024, &logLength, log);
        NIA_ERROR("%s\n", log);
    }
}

NIA_INTERNAL void niaShader::buildProgram(GLuint vertexShader, GLuint fragmentShader){
    program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glValidateProgram(program);

    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE){
        GLsizei logLength = 0;
        GLchar log[1024] = {0};
        glGetProgramInfoLog(program, 1024, &logLength, log);
        NIA_ERROR("%s\n", log);
    }

    glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
    if (status != GL_TRUE){
        GLsizei logLength = 0;
        GLchar log[1024] = {0};
        glGetProgramInfoLog(program, 1024, &logLength, log);
        NIA_ERROR("%s\n", log);
    }
}

NIA_CALL void niaShader::useShader(){
    glUseProgram(program);
}

NIA_CALL void niaShader::unuseShader(){
    glUseProgram(0);
}
