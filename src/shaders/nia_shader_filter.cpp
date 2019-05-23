#include "shaders/nia_shader_filter.h"

niaShaderFilter::niaShaderFilter(){
}

niaShaderFilter::niaShaderFilter(const char* sourceCodeVertex, const char* sourceCodeFragment){
    vertexShader = loadBufferToShader(sourceCodeVertex, GL_VERTEX_SHADER);
    fragmentShader = loadBufferToShader(sourceCodeFragment, GL_FRAGMENT_SHADER);

    compileShader(vertexShader);
    compileShader(fragmentShader);
    buildProgram(vertexShader, fragmentShader);
}

niaShaderFilter::~niaShaderFilter(){
}
