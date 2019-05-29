#include "nia_sprite.h"

#include "nia_gl.h"
#include "nia_vertex.h"
#include "nia_constants.h"
#include "nia_texture_structures.h"

niaSprite::niaSprite(){
}

niaSprite::niaSprite(u8* bitmap, u32 textureWidth, u32 textureHeight, const niaRectangle& rect):
niaSprite(NULL, rect){
    loadTexture(bitmap, textureWidth, textureHeight);
}

niaSprite::niaSprite(const char* filename, const niaRectangle& rect){
    if(filename){
        loadTexture(filename);
    }

    setBounds(rect);

    NIA_GL_CALL(glGenVertexArrays(1, &rectangle.vao)); // put this inside niaMesh
    NIA_GL_CALL(glBindVertexArray(rectangle.vao));

    NIA_GL_CALL(glGenBuffers(1, &vbo));
    NIA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));

    NIA_GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(niaVertex) * 4, 0, GL_DYNAMIC_DRAW));

    u32 customUVTStride = NIA_VERTEX_STRIDE + NIA_COLOR_STRIDE;

    NIA_GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_VERTEX_STRIDE));
    NIA_GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_COLOR_STRIDE));
    NIA_GL_CALL(glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_NORMALS_STRIDE));
    NIA_GL_CALL(glVertexAttribPointer(3, 2, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_UV_STRIDE));
    
    NIA_GL_CALL(glEnableVertexAttribArray(0));
    NIA_GL_CALL(glEnableVertexAttribArray(1));
    NIA_GL_CALL(glEnableVertexAttribArray(2));
    NIA_GL_CALL(glEnableVertexAttribArray(3));

    niaVertex v[4] ={
                    {rectangle.x              , rectangle.y              , 0,        1, 1, 1,     1, 1, 1,      0, 0},
                    {rectangle.x + rectangle.w, rectangle.y + rectangle.h, 0,        1, 1, 1,     1, 1, 1,      1, 1},
                    {rectangle.x              , rectangle.y + rectangle.h, 0,        1, 1, 1,     1, 1, 1,      0, 1},
                    {rectangle.x + rectangle.w, rectangle.y              , 0,        1, 1, 1,     1, 1, 1,      1, 0}
    };

    NIA_GL_CALL(glNamedBufferSubData(vbo, 0, sizeof(niaVertex) * 4, v));

    NIA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

    u8 indices[] = {0, 1, 2,
                 0, 3, 1};

    NIA_GL_CALL(glGenBuffers(1, &veo));

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, veo));
    NIA_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

niaSprite::~niaSprite(){
}

void niaSprite::loadTexture(const char* filename){
    // TODO make this actually work normally, pull of the test case that exists now below,
    //      niaTexture(filenmae) is the only one that is needed, the flag for animated or not
    //      + frame count if is the case will come in the destructor
    texture = niaTexture(filename, NIA_TEXTURE_NOT_ANIMATED, 3);
}

void niaSprite::loadTexture(u8* bitmap, u32 width, u32 height){
    texture = niaTexture(bitmap, width, height, NIA_TEXTURE_FORMAT_R8_RED_UBYTE);
}

void NIA_CALL niaSprite::setBounds(const niaRectangle& rect){
    rectangle.w = rect.w;
    rectangle.h = rect.h;

    rectangle.x = rect.x;
    rectangle.y = rect.y;
}