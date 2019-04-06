#include "nia_shader_filter.h"

NIA_CALL niaShaderFilter::niaShaderFilter(){
}

NIA_CALL niaShaderFilter::niaShaderFilter(const char* sourceCodeVertex, const char* sourceCodeFragment){
    vertexShader = loadBufferToShader(sourceCodeVertex, GL_VERTEX_SHADER);
    fragmentShader = loadBufferToShader(sourceCodeFragment, GL_FRAGMENT_SHADER);

    compileShader(vertexShader);
    compileShader(fragmentShader);
    buildProgram(vertexShader, fragmentShader);
}

NIA_CALL niaShaderFilter::~niaShaderFilter(){
}
