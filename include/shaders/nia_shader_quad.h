/*
 * File: nia_shader_quad.h
 * Project: Nia
 * File created on April-29-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef _NIA_SHADER_QUAD_H_
#define _NIA_SHADER_QUAD_H_

#include "shaders/nia_shader.h"
#include "nia_general.h"

NIA_CLASS niaShaderQuad : public niaShader{
public:
    NIA_CALL niaShaderQuad();
    NIA_CALL ~niaShaderQuad();
};

#endif //_NIA_SHADER_QUAD_H_