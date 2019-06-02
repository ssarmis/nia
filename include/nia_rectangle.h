/*
 * File: nia_rectangle.h
 * Project: Nia
 * File created on February-25-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef _NIA_RECTANGLE_H_
#define _NIA_RECTANGLE_H_

#include "nia_general.h"

NIA_STRUCT niaGlRectangle {
    u32 vao;
    r32 x;
    r32 y;
    r32 w;
    r32 h;
    union {
        r32 c[3];
        struct {
            r32 r;
            r32 g;
            r32 b;
        };
    };
} niaGlRectangle;

NIA_STRUCT niaRectangle {
    r32 x;
    r32 y;
    r32 w;
    r32 h;
    union {
        r32 c[3];
        struct {
            r32 r;
            r32 g;
            r32 b;
        };
    };
} niaRectangle;

#endif