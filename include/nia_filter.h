#ifndef _NIA_FILTER_H_
#define _NIA_FILTER_H_

#include "nia_mesh.h"
#include "nia_general.h"
#include "nia_renderer.h"
#include "shaders/nia_shader_quad.h"
#include "nia_frame_buffer.h"
#include "shaders/nia_shader_filter.h"

#define NIA_FILTER_GRAYSCALE

NIA_CLASS niaFilter {
protected:
    niaShaderFilter shader; 
    niaMesh quad;

public:
    niaFilter();
    niaFilter(const char* sourceVertex, const char* sourceFragment);
    ~niaFilter();

    virtual void process(niaRenderer* renderer, niaFrameBuffer& in, niaFrameBuffer& out) = 0;
    void renderFrameBuffer(niaRenderer* renderer, niaFrameBuffer& in);
};

NIA_CLASS niaFilterGreyScale : public niaFilter{
public:
    niaFilterGreyScale();
    ~niaFilterGreyScale();

    void process(niaRenderer* renderer, niaFrameBuffer& in, niaFrameBuffer& out);
};

NIA_CLASS niaFilterInvert : public niaFilter{
public:
    niaFilterInvert();
    ~niaFilterInvert();

    void process(niaRenderer* renderer, niaFrameBuffer& in, niaFrameBuffer& out);
};

NIA_CLASS niaFilterBoxBlur : public niaFilter{
public:
    niaFilterBoxBlur();
    ~niaFilterBoxBlur();

    void process(niaRenderer* renderer, niaFrameBuffer& in, niaFrameBuffer& out);
};


#endif //_NIA_FILTER_H_