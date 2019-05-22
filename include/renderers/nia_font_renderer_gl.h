#ifdef NIA_RENDERER_GL

#ifndef _NIA_FONT_RENDERER_GL_H_
#define _NIA_FONT_RENDERER_GL_H_

#include "nia_general.h"
#include "renderers/nia_renderer_gl.h"
#include "renderers/nia_batch_renderer_gl.h"
#include "parsers/nia_ttf_parser.h"

NIA_CLASS niaFontRenderer : public niaBatchRenderer {
private:
    niaTTFParser* fontParser;

public:
    niaFontRenderer();
    niaFontRenderer(const char* filename);
    ~niaFontRenderer();

    void renderString(r32 x, r32 y, r32 size, const char* string); // <   these 2 are the same
    void render(r32 x, r32 y, r32 size, const char* string);       // <
};

#endif //_NIA_FONT_RENDERER_GL_H_
#endif //NIA_RENDERER_GL
