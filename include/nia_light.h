/*
 * File: nia_light.h
 * Project: Nia
 * File created on June-09-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef _NIA_LIGHT_H_
#define _NIA_LIGHT_H_

#include "nia_general.h"
#include "nia_vector.h"

NIA_STRUCT niaLight {
    niaVector3<r32> position;
    niaVector3<r32> color;
} niaLight;

#endif // _NIA_LIGHT_H_