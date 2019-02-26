#ifdef _WIN32

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
#endif