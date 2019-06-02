/*
 * File: nia_shader_cubemap.h
 * Project: Nia
 * File created on April-29-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef _NIA_SHADER_CUBE_MAP_H_
#define _NIA_SHADER_CUBE_MAP_H_

#include "nia_general.h"

#include "shaders/nia_shader.h"

NIA_CLASS niaShaderCubeMap : public niaShader{
public:
    NIA_CALL niaShaderCubeMap();
    NIA_CALL ~niaShaderCubeMap();
};

#endif //_NIA_SHADER_CUBE_MAP_H_
