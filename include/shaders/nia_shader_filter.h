#ifndef _NIA_FILTERS_H_
#define _NIA_FILTERS_H_

#include "shaders/nia_shader.h"
#include "nia_general.h"

NIA_CLASS niaShaderFilter : public niaShader {
public:
    int a;
    int b;
    int c;

    niaShaderFilter();
    niaShaderFilter(const char* sourceCodeVertex, const char* sourceCodeFragment);
    ~niaShaderFilter();
};


#endif // _NIA_FILTERS_H_