#ifndef _NIA_FILTER_H_
#define _NIA_FILTER_H_

#include "nia_mesh.h"
#include "nia_general.h"
#include "nia_renderer.h"
#include "nia_shader_quad.h"
#include "nia_frame_buffer.h"
#include "nia_shader_filter.h"

#define NIA_FILTER_GRAYSCALE

NIA_CLASS niaFilter {
protected:
    niaShaderFilter shader; 
    niaMesh quad;

public:
    NIA_CALL niaFilter();
    NIA_CALL niaFilter(const char* sourceVertex, const char* sourceFragment);
    NIA_CALL ~niaFilter();

    NIA_CALL virtual void process(niaRenderer* renderer, niaFrameBuffer& in, niaFrameBuffer& out) = 0;
    NIA_CALL void renderFrameBuffer(niaRenderer* renderer, niaFrameBuffer& in);
};

NIA_CLASS niaFilterGreyScale : public niaFilter{
public:
    NIA_CALL niaFilterGreyScale();
    NIA_CALL ~niaFilterGreyScale();

    NIA_CALL void process(niaRenderer* renderer, niaFrameBuffer& in, niaFrameBuffer& out);
};

NIA_CLASS niaFilterInvert : public niaFilter{
public:
    NIA_CALL niaFilterInvert();
    NIA_CALL ~niaFilterInvert();

    NIA_CALL void process(niaRenderer* renderer, niaFrameBuffer& in, niaFrameBuffer& out);
};

NIA_CLASS niaFilterBoxBlur : public niaFilter{
public:
    NIA_CALL niaFilterBoxBlur();
    NIA_CALL ~niaFilterBoxBlur();

    NIA_CALL void process(niaRenderer* renderer, niaFrameBuffer& in, niaFrameBuffer& out);
};


#endif //_NIA_FILTER_H_