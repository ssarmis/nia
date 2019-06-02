/*
 * File: nia_filter.h
 * Project: Nia
 * File created on March-30-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

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
    NIA_CALL niaFilter();
    NIA_CALL niaFilter(const char* sourceVertex, const char* sourceFragment);
    NIA_CALL ~niaFilter();

    virtual void NIA_CALL process(niaRenderer* renderer, niaFrameBuffer& in, niaFrameBuffer& out) = 0;
    void NIA_CALL renderFrameBuffer(niaRenderer* renderer, niaFrameBuffer& in);
};

NIA_CLASS niaFilterGreyScale : public niaFilter{
public:
    NIA_CALL niaFilterGreyScale();
    NIA_CALL ~niaFilterGreyScale();

    void NIA_CALL process(niaRenderer* renderer, niaFrameBuffer& in, niaFrameBuffer& out);
};

NIA_CLASS niaFilterInvert : public niaFilter{
public:
    NIA_CALL niaFilterInvert();
    NIA_CALL ~niaFilterInvert();

    void NIA_CALL process(niaRenderer* renderer, niaFrameBuffer& in, niaFrameBuffer& out);
};

NIA_CLASS niaFilterBoxBlur : public niaFilter{
public:
    NIA_CALL niaFilterBoxBlur();
    NIA_CALL ~niaFilterBoxBlur();

    void NIA_CALL process(niaRenderer* renderer, niaFrameBuffer& in, niaFrameBuffer& out);
};


#endif //_NIA_FILTER_H_