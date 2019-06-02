/*
 * File: nia_shader_filter.h
 * Project: Nia
 * File created on April-29-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef _NIA_FILTERS_H_
#define _NIA_FILTERS_H_

#include "shaders/nia_shader.h"
#include "nia_general.h"

NIA_CLASS niaShaderFilter : public niaShader {
public:
    NIA_CALL niaShaderFilter();
    NIA_CALL niaShaderFilter(const char* sourceCodeVertex, const char* sourceCodeFragment);
    NIA_CALL ~niaShaderFilter();
};


#endif // _NIA_FILTERS_H_