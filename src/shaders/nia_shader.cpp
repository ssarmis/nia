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

#include "shaders/nia_shader.h"

#include <stdio.h>
#include <malloc.h>

NIA_STATIC char* basicVertexShader = ""
NIA_GLSL_VERSION" \n""precision "
NIA_GLSL_PRECISION" float;\n"
"layout(location = 0) in vec3 pos;\n"
"layout(location = 1) in vec3 color;\n"
"layout(location = 2) in vec3 normal;\n"
"layout(location = 3) in vec2 uv;\n"

"uniform vec3 dlP;\n"
"uniform vec3 dlC;\n"

"uniform vec3 slP;\n"
"uniform vec3 slC;\n"

"uniform mat4 mP;\n"
"uniform mat4 mT;\n"
"uniform mat4 mV;\n"

"uniform float lightIsEnabled;\n"

"varying float o_lightIsEnabled;\n"

"out vec4 o_color;\n"

"out vec2 o_uv;\n"

"out vec3 o_dlP;\n"
"out vec3 o_dlC;\n"
"out vec3 o_slP;\n"
"out vec3 o_slC;\n"

"out float o_lightFactor;\n"
"out float o_lightSpecularFactor;\n"

"void main(){\n"
"   vec4 transformedPosition = mT * vec4(pos, 1.0);\n"
"   gl_Position = mP * mV * transformedPosition;\n"
"   o_color = vec4(color, 1.0);\n"
"   o_uv = uv;\n"
"   o_dlP = dlP;\n"
"   o_dlC = dlC;\n"
"   o_slP = slP;\n"
"   o_slC = slC;\n"
"   o_lightIsEnabled = lightIsEnabled;\n"
"   if(lightIsEnabled > 0){\n"
"      vec3 lightVector = (dlP - transformedPosition.xyz);\n"
"      vec3 normalizedLightVector = normalize(lightVector);\n"

"      mat3 normalMatrix = transpose(inverse(mat3(mT)));\n"
"      vec3 transformedNormal = (normalMatrix * normal);\n"
"      vec3 normlizedTransformedNormal = normalize(transformedNormal);\n"

"      o_lightFactor = dot(normalizedLightVector, normlizedTransformedNormal);\n"
"      o_lightFactor = max(o_lightFactor, 0.7);\n"

"      vec3 specularLightVector = (slP - transformedPosition.xyz);\n"
"      vec3 reflectedCameraVector = specularLightVector - 2 * (dot(specularLightVector, normlizedTransformedNormal) * normlizedTransformedNormal);\n"
"      reflectedCameraVector = normalize(reflectedCameraVector);\n"
"      vec4 viewVector = normalize(inverse(mV)[3] - transformedPosition);\n"
"      o_lightSpecularFactor = dot(-reflectedCameraVector, viewVector.xyz);\n"
"      o_lightSpecularFactor = pow(max(o_lightSpecularFactor, 0.0), 32);\n"
"   }\n"
"}\n"
"";

NIA_STATIC char* basicFragmentShader = ""
NIA_GLSL_VERSION" \n""precision "
NIA_GLSL_PRECISION" float;\n"

"out vec4 finalColor;\n"

"varying float o_lightIsEnabled;\n"

"in vec4 o_color;\n"

"in vec2 o_uv;\n"

"in vec3 o_dlP;\n"
"in vec3 o_dlC;\n"
"in vec3 o_slP;\n"
"in vec3 o_slC;\n"
"in float o_lightFactor;\n"
"in float o_lightSpecularFactor;\n"

"uniform sampler2D tex;\n"

"void main(){\n"
"   if(o_lightIsEnabled > 0){\n"
"      vec4 diffuse = (o_lightFactor * vec4(o_dlC, 1.0));\n"
"      vec4 specular = (1 * o_lightSpecularFactor * vec4(o_slC, 1.0));\n"
"      finalColor = (specular + diffuse) * (texture(tex, o_uv) * o_color);\n"
"   } else {\n"
"      finalColor = (texture(tex, o_uv) * o_color);\n"
"   }"
"}\n"
"";

niaShader::niaShader(const char* vertexShaderFilename, const char* fragmentShaderFilename){
    vertexShader = loadSourceToShader(vertexShaderFilename, GL_VERTEX_SHADER);
    fragmentShader = loadSourceToShader(fragmentShaderFilename, GL_FRAGMENT_SHADER);

    compileShader(vertexShader);
    compileShader(fragmentShader);
    buildProgram(vertexShader, fragmentShader);
}

niaShader::niaShader(){
    vertexShader = loadBufferToShader(basicVertexShader, GL_VERTEX_SHADER);
    fragmentShader = loadBufferToShader(basicFragmentShader, GL_FRAGMENT_SHADER);

    compileShader(vertexShader);
    compileShader(fragmentShader);
    buildProgram(vertexShader, fragmentShader);
}

niaShader::~niaShader(){
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
        NIA_ERROR("From file "__FILE__"\n");
        NIA_ERROR("\t%s\n", log);
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

GLuint niaShader::getProgram() const {
    return program;
}

GLuint niaShader::getVertexShader() const {
    return vertexShader;
}

GLuint niaShader::getFragmentShader() const {
    return fragmentShader;
}


void niaShader::useShader(){
    NIA_GL_CALL(glUseProgram(program));
}

void niaShader::unuseShader(){
    NIA_GL_CALL(glUseProgram(0));
}

// TODO cache uniform ids
void niaShader::setUniform1f(const char* name, r32 value){
    GLint id = glGetUniformLocation(program, name); // cache these
    NIA_GL_CALL(glUniform1f(id, value));
}

void niaShader::setUniformU32(const char* name, u32 value){
    GLint id = glGetUniformLocation(program, name); // cache these
    NIA_GL_CALL(glUniform1ui(id, value));
}

void niaShader::setUniformMat4(const char* name, const niaMatrix4& mat, bool transpose){
    GLint id = glGetUniformLocation(program, name); // cache these
    NIA_GL_CALL(glUniformMatrix4fv(id, 1, transpose, mat.m));
}

void niaShader::setUniformVec3(const char* name, const niaVector3<r32>& vec){
    GLint id = glGetUniformLocation(program, name);
    NIA_GL_CALL(glUniform3f(id, vec.x, vec.y, vec.z));
}
