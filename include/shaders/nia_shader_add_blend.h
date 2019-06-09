/*
 * File: nia_shader_add_blend.h
 * Project: Nia
 * File created on June-06-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef _NIA_SHADER_ADD_BLEND_H_
#define _NIA_SHADER_ADD_BLEND_H_

#include "shaders/nia_shader.h"
#include "nia_general.h"

NIA_CLASS niaShaderAddBlend : public niaShader{
public:
    NIA_CALL niaShaderAddBlend();
    NIA_CALL ~niaShaderAddBlend();
};

#endif //_NIA_SHADER_ADD_BLEND_H_