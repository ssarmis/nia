#include "nia_texture_gl.h"

#include "nia_bmp_parser.h"

NIA_CALL niaTexture::niaTexture(const char* filename){
    // for now I will assume there will only be .bmp files
    // TODO add extension cheking, header checking, more file formats forsers
    niaBmpParser parser(filename);
    allocateTexture(parser.getWidth(), parser.getHeight(), parser.getPixelData());
}

NIA_CALL niaTexture::niaTexture(){
    u8 pixels[] = {
        255, 255, 255,
        255, 255, 255
    };

    allocateTexture(1, 1, pixels);
}

NIA_CALL niaTexture::~niaTexture(){
}

NIA_INTERNAL void niaTexture::allocateTexture(u32 width, u32 height, u8* data){ // TODO add ability to change depth
    NIA_GL_CALL(glGenTextures(1, &textureId));

    NIA_GL_CALL(glBindTexture(GL_TEXTURE_2D, textureId));

    NIA_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    NIA_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    NIA_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    NIA_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

    NIA_GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
    NIA_GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data));

    NIA_GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}