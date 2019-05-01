#include "shaders/nia_shader.h"

#include <stdio.h>
#include <malloc.h>

NIA_STATIC char* basicVertexShader = ""
NIA_GLSL_VERSION" \n""precision "NIA_GLSL_PRECISION" float;\n"
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

"out vec4 o_color;\n"

"out vec2 o_uv;\n"

"out vec3 o_dlP;\n"
"out vec3 o_dlC;\n"
"out vec3 o_slP;\n"
"out vec3 o_slC;\n"

"out float o_lightFactor;\n"

"void main(){\n"
"   vec4 transformedPosition = mT * vec4(pos, 1.0);\n"
"   gl_Position = mP * mV * transformedPosition;\n"
"   o_color = vec4(color, 1.0);\n"
"   o_uv = uv;\n"
"   o_dlP = dlP;\n"
"   o_dlC = dlC;\n"
"   o_slP = slP;\n"
"   o_slC = slC;\n"

"   vec3 lightVector = (transformedPosition.xyz - dlP);\n"
"   lightVector = normalize(lightVector);\n"
"   vec3 transformedNormal = (transformedPosition.xyz * normal);\n"
"   transformedNormal = normalize(transformedNormal);\n"
"   o_lightFactor = dot(lightVector, transformedNormal);\n"

"}\n"
"";

NIA_STATIC char* basicFragmentShader = ""
NIA_GLSL_VERSION" \n""precision "NIA_GLSL_PRECISION" float;\n"

"out vec4 finalColor;\n"

"in vec4 o_color;\n"

"in vec2 o_uv;\n"

"in vec3 o_dlP;\n"
"in vec3 o_dlC;\n"
"in vec3 o_slP;\n"
"in vec3 o_slC;\n"
"in float o_lightFactor;\n"

"uniform sampler2D tex;\n"

"void main(){\n"
"   float finalDiffuseFactor = max(o_lightFactor, 0.3);\n"
"   finalColor = finalDiffuseFactor * texture(tex, o_uv) * o_color;\n"
// "   finalColor = vec4(1.0, 0.0, 1.0, 1.0);\n"
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
    GLint id = glGetUniformLocation(program, name); // cache these
    NIA_GL_CALL(glUniformMatrix4fv(id, 1, transpose, mat.m));
}

NIA_CALL void niaShader::setUniformVec3(const char* name, const niaVector3<r32>& vec){
    GLint id = glGetUniformLocation(program, name);
    NIA_GL_CALL(glUniform3f(id, vec.x, vec.y, vec.z));
}
