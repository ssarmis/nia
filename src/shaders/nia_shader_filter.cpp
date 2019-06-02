/*
 * File: nia_shader_filter.cpp
 * Project: Nia
 * File created on April-29-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

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
