#include "renderers/nia_font_renderer_gl.h"

#include "nia_gl.h"
#include "nia_vertex.h"
#include "nia_constants.h"
#include "nia_rectangle.h"

#define NIA_BATCH_MAXIMUM_QUADS     1000000
#define NIA_BATCH_VERTICES_COUNT    (NIA_BATCH_MAXIMUM_QUADS * sizeof(niaBasicUVVertex))
#define NIA_BATCH_INDICES_COUNT     (NIA_BATCH_VERTICES_COUNT * 6)


NIA_STATIC u32 batchVao = 0;
NIA_STATIC u32 batchVbo = 0;
NIA_STATIC u32 batchVeo = 0;
NIA_STATIC u32 batchUsedIndices = 0;
NIA_STATIC u16 batchIndices[NIA_BATCH_INDICES_COUNT] = {};

NIA_STATIC u32 usedRectangles = 0;

#define NIA_UV_STRIDE_FONT NIA_NORMALS_STRIDE
// just because they are equal


niaFontRenderer::niaFontRenderer(){
}

niaFontRenderer::niaFontRenderer(const char* filename){
    niaFontRenderer();
    fontParser = new niaTTFParser(filename);
}

niaFontRenderer::~niaFontRenderer(){
}

void niaFontRenderer::renderString(r32 x, r32 y, r32 size, const char* string){
    u32 vertex = 0;
    r32 horizontalCursor = 0;
#if 1
    while(*string) {
        niaTransform transform;
        niaGlyph glyph = *fontParser->getGlyphChar(*string);

        r32 w = (glyph.bounds.xmax - glyph.bounds.xmin) / size;
        r32 h0 = (glyph.bounds.ymax + - glyph.bounds.ymin);
        r32 h = h0 * w / (glyph.bounds.xmax - glyph.bounds.xmin);

        r32 xx = x + horizontalCursor;
        r32 yy = y - glyph.metrics.verticalAdvance / size; // the vertical advance is wrong at the moment for some fonts

        // TODO maybe change the scaling...
        // transform.scale(glyph->sprite.getWidth() / w, glyph->sprite.getHeight() / h);
        transform.translate(niaVector3<r32>(xx, yy, 0));
        transform.scale(h / glyph.sprite.getHeight());

        horizontalCursor += glyph.metrics.cursorAdvance / size; // TODO make all the division already precomputed in parsing, not at rendering

        submitTransformation(transform);
        renderSprite(glyph.sprite);

        string++;
    }
#endif

}



void niaFontRenderer::render(r32 x, r32 y, r32 size, const char* string){
}
