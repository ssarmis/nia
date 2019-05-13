#include "nia_texture_gl.h"

#include "parsers/nia_bmp_parser.h"
#include "parsers/nia_tga_parser.h"
#include "parsers/nia_png_parser.h"


#include "nia_texture_streaming.h"

NIA_CALL niaTexture::niaTexture(const char* filename){
    // for now I will assume there will only be .bmp files
    // TODO add extension cheking, header checking, more file formats forsers
    textureFormatDetails textureFormat;
    const char* tmp = filename;
    while(*tmp){
        if(*tmp++ == '.'){
            switch (*tmp) {
                case 't':{
                        niaTgaParser parser(filename);
                        allocateTexture(parser.getWidth(), parser.getHeight(), parser.getPixelData(), parser.getTextureFormat());
                        textureFormat = parser.getTextureFormat();
                    }
                    break;

                case 'b':{
                        niaBmpParser parser(filename);
                        allocateTexture(parser.getWidth(), parser.getHeight(), parser.getPixelData(), parser.getTextureFormat());
                        textureFormat = parser.getTextureFormat();
                    }
                    break;

                case 'p':{
                        niaPngParser parser(filename);
                        allocateTexture(parser.getWidth(), parser.getHeight(), parser.getPixelData(), parser.getTextureFormat());
                        textureFormat = parser.getTextureFormat();
                    }
                    break;

                default:{
                    }
                    break;
            }
        }
    }
#ifdef _WIN32
    // niaTextureStreaming::appendLiveLoadingTexture(textureId, (char*)filename, textureFormat);
#endif
}

NIA_CALL niaTexture::niaTexture(u8* data, u32 width, u32 height, const textureFormatDetails& details){
    allocateTexture(width, height, data, details); // data should be first, but whatever
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

NIA_INTERNAL void niaTexture::allocateTexture(u32 width, u32 height, u8* data, const textureFormatDetails& details){ // TODO add ability to change depth
    NIA_GL_CALL(glGenTextures(1, &textureId));

    NIA_GL_CALL(glBindTexture(GL_TEXTURE_2D, textureId));

    NIA_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    NIA_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    NIA_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    NIA_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

    NIA_GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
    NIA_GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, details.storeFormat, width, height, 0, details.readFormat, details.type, data));

    NIA_GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}
