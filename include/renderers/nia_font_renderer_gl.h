/*
 * File: nia_font_renderer_gl.h
 * Project: Nia
 * File created on May-18-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifdef NIA_RENDERER_GL

#ifndef _NIA_FONT_RENDERER_GL_H_
#define _NIA_FONT_RENDERER_GL_H_

#include "nia_general.h"
#include "renderers/nia_renderer_gl.h"
#include "renderers/nia_batch_renderer_gl.h"
#include "parsers/nia_ttf_parser.h"

NIA_CLASS niaFontRenderer : public niaRenderer {
private:
    niaTTFParser* fontParser;

public:
    NIA_CALL niaFontRenderer();
    NIA_CALL niaFontRenderer(const char* filename, u16 fontSize);
    NIA_CALL ~niaFontRenderer();

    void NIA_CALL renderString(r32 x, r32 y, r32 size, const char* string, const niaVector3<r32>& color); // <   these 2 are the same
    void NIA_CALL render(r32 x, r32 y, r32 size, const char* string);       // <
};

#endif //_NIA_FONT_RENDERER_GL_H_
#endif //NIA_RENDERER_GL
