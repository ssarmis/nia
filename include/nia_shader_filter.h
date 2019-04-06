#ifndef _NIA_FILTERS_H_
#define _NIA_FILTERS_H_

#include "nia_shader.h"
#include "nia_general.h"

NIA_CLASS niaShaderFilter : public niaShader {
public:
    int a;
    int b;
    int c;

    NIA_CALL niaShaderFilter();
    NIA_CALL niaShaderFilter(const char* sourceCodeVertex, const char* sourceCodeFragment);
    NIA_CALL ~niaShaderFilter();
};


#endif // _NIA_FILTERS_H_