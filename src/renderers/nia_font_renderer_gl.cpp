#include "renderers/nia_font_renderer_gl.h"

#include "nia_gl.h"
#include "nia_vertex.h"
#include "nia_constants.h"
#include "nia_rectangle.h"

niaFontRenderer::niaFontRenderer(){
}

niaFontRenderer::niaFontRenderer(const char* filename, u16 fontSize){
    niaFontRenderer();
    fontParser = new niaTTFParser(filename, fontSize);
}

niaFontRenderer::~niaFontRenderer(){
}

// void niaFontRenderer::renderString(r32 x, r32 y, r32 size, const char* string, r32 color[])

void niaFontRenderer::renderString(r32 x, r32 y, r32 size, const char* string, const niaVector3<r32>& color){
    u32 vertex = 0;
    r32 horizontalCursor = 0;
#if 1
    while(*string) {
        niaGlyph* glyph = fontParser->getGlyphChar(*string);
        if(*string == ' '){
            horizontalCursor += glyph->metrics.cursorAdvance;
            string++;
            continue;
        }
        niaTransform transform;

        r32 xx = x + horizontalCursor;
        // r32 yy = y + fontParser->getFontAscent() * fontParser->getGlyphScale(fontParser->getGlyphIndex(*string)) +
        //          glyph->metrics.verticalAdvance + glyph->bounds.ymax + glyph->bounds.ymin;
        r32 ascent = fontParser->getFontAscent() * fontParser->getGlyphScale(fontParser->getGlyphIndex(*string));
        r32 yy = y + ascent - glyph->bounds.ymax;
        // if(glyph->bounds.ymin < 0){
        //     yy += glyph->sprite.getHeight() - glyph->bounds.ymin;
        // }

        transform.translate(niaVector3<r32>(xx, yy, 0));

        horizontalCursor += glyph->metrics.cursorAdvance; // TODO make all the division already precomputed in parsing, not at rendering

        submitTransformation(transform);
        renderGlyph(glyph, color);

        string++;
    }
#endif

}

void niaFontRenderer::render(r32 x, r32 y, r32 size, const char* string){
}
