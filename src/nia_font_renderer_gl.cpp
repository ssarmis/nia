#include "nia_font_renderer_gl.h"

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

NIA_STATIC void setupBatchBuffers(){
    NIA_GL_CALL(glGenVertexArrays(1, &batchVao));
    NIA_GL_CALL(glBindVertexArray(batchVao));

    NIA_GL_CALL(glGenBuffers(1, &batchVbo));
    NIA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, batchVbo));

    NIA_GL_CALL(glBufferData(GL_ARRAY_BUFFER, NIA_BATCH_VERTICES_COUNT, 0, GL_DYNAMIC_DRAW));
    
    NIA_GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(niaBasicUVVertex), (GLvoid*)NIA_VERTEX_STRIDE));
    NIA_GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(niaBasicUVVertex), (GLvoid*)NIA_COLOR_STRIDE));
    NIA_GL_CALL(glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(niaBasicUVVertex), (GLvoid*)NIA_UV_STRIDE_FONT));
    
    
    NIA_GL_CALL(glEnableVertexAttribArray(0));
    NIA_GL_CALL(glEnableVertexAttribArray(1));
    NIA_GL_CALL(glEnableVertexAttribArray(2));

    NIA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

    u32 indicesOffset = 0;

    for(int i = 0; i < NIA_BATCH_INDICES_COUNT; i += 6){
        // 0 1 2   0 3 1
        batchIndices[i + 0] = indicesOffset + 0;
        batchIndices[i + 1] = indicesOffset + 1;
        batchIndices[i + 2] = indicesOffset + 2;

        batchIndices[i + 3] = indicesOffset + 0;
        batchIndices[i + 4] = indicesOffset + 3;
        batchIndices[i + 5] = indicesOffset + 1;

        indicesOffset += 4;
    }

    NIA_GL_CALL(glGenBuffers(1, &batchVeo));

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batchVeo));
    NIA_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(batchIndices), batchIndices, GL_STATIC_DRAW));

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    NIA_GL_CALL(glBindVertexArray(0));
}

NIA_CALL niaFontRenderer::niaFontRenderer(){
    setupBatchBuffers();
}

NIA_CALL niaFontRenderer::niaFontRenderer(const char* filename){
    niaFontRenderer();
    fontParser = new niaTTFParser(filename);
}

NIA_CALL niaFontRenderer::~niaFontRenderer(){
}

NIA_CALL void niaFontRenderer::renderString(r32 x, r32 y, r32 size, const char* string){
    NIA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, batchVbo));
    niaBasicUVVertex* source = (niaBasicUVVertex*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    if(source){
        u32 vertex = 0;
        r32 horizontalCursor = 0;
        while(*string) {
            // niaTTFGlyph glyphMetrics = fontParser.getGlyphMetrics((*string));
            niaGlyph glyph = *fontParser->getGlyphChar(*string);

            r32 u0 = glyph.uvs.u0; // make these things in parsing he special characters
            r32 u1 = glyph.uvs.u1;
            r32 v0 = glyph.uvs.v0;
            r32 v1 = glyph.uvs.v1;

            r32 w = (glyph.bounds.xmax - glyph.bounds.xmin) / size;
            r32 h0 = (glyph.bounds.ymax + - glyph.bounds.ymin);
            r32 h = h0 * w / (glyph.bounds.xmax - glyph.bounds.xmin);

            r32 xx = x + horizontalCursor;
            r32 yy = y - glyph.metrics.verticalAdvance / size; // the vertical advance is wrong at the moment for some fonts

            if(*string == ' '){
                u0 = 1;
                u1 = 1;
                v0 = 1;
                v1 = 1;
                w = 0;
                h = 0;
            }

            for (u32 i = 0; i < 4; ++i){
                source[vertex + i].r = 0;
                source[vertex + i].g = 0;
                source[vertex + i].b = 0;
            }


            source[vertex + 0].x = xx;
            source[vertex + 0].y = yy;
            source[vertex + 0].z = 0;

            source[vertex + 0].u = u0;
            source[vertex + 0].v = v1;

           
            // vert 1
            source[vertex + 1].x = xx + w;
            source[vertex + 1].y = yy + h;
            source[vertex + 1].z = 0;

            source[vertex + 1].u = u1;
            source[vertex + 1].v = v0;

           
            // vert 2
            source[vertex + 2].x = xx;
            source[vertex + 2].y = yy + h;
            source[vertex + 2].z = 0;
            
            source[vertex + 2].u = u0;
            source[vertex + 2].v = v0;

         
            // vert 3
            source[vertex + 3].x = xx + w;
            source[vertex + 3].y = yy;
            source[vertex + 3].z = 0;
            
            source[vertex + 3].u = u1;
            source[vertex + 3].v = v1;

            //


            vertex += 4;
            batchUsedIndices += 6;
            horizontalCursor += glyph.metrics.cursorAdvance / size; // TODO make all the division already precomputed in parsing, not at rendering

            string++;
        }

        NIA_GL_CALL(glUnmapBuffer(GL_ARRAY_BUFFER));
        NIA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));


        NIA_GL_CALL(glBindVertexArray(batchVao));
        defaultShaderFont.useShader();
        
        NIA_GL_CALL(glBindTexture(GL_TEXTURE_2D, fontParser->texture.textureId));

        NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batchVeo));
        NIA_GL_CALL(glDrawElements(GL_TRIANGLES, batchUsedIndices, GL_UNSIGNED_SHORT, 0));

        defaultShaderFont.unuseShader();

        NIA_GL_CALL(glBindVertexArray(0));

        batchUsedIndices = 0;
    }
}



NIA_CALL void niaFontRenderer::render(r32 x, r32 y, r32 size, const char* string){
}
