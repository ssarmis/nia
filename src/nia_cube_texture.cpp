#include "nia_cube_texture.h"

#include "parsers/nia_bmp_parser.h"
#include "parsers/nia_tga_parser.h"
// GL_TEXTURE_CUBE_MAP_POSITIVE_X	Right
// GL_TEXTURE_CUBE_MAP_NEGATIVE_X	Left
// GL_TEXTURE_CUBE_MAP_POSITIVE_Y	Top
// GL_TEXTURE_CUBE_MAP_NEGATIVE_Y	Bottom
// GL_TEXTURE_CUBE_MAP_POSITIVE_Z	Back
// GL_TEXTURE_CUBE_MAP_NEGATIVE_Z	Front

niaCubeTexture::niaCubeTexture(const char* textures[6]){

    NIA_GL_CALL(glGenTextures(1, &textureId));

    NIA_GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, textureId));

    NIA_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT));
    NIA_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT));

    NIA_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    NIA_GL_CALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

    NIA_GL_CALL(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));

    for(int i = 0; i < 6; ++i){
        u8* pixels;
        u32 width;
        u32 height;
        const char* tmp = textures[i];
        while(*tmp){
            if(*tmp++ == '.'){
                switch (*tmp) {
                    case 't':{
                            niaTgaParser parser(textures[i]);
                            width = parser.getWidth();
                            height = parser.getHeight();
                            pixels = parser.getPixelData();
                        }
                        break;

                    case 'b':{
                            niaBmpParser parser(textures[i]);
                            width = parser.getWidth();
                            height = parser.getHeight();
                            pixels = parser.getPixelData();
                        }
                        break;

                    default:{
                        }
                        break;
                }
            }
        }

        allocateTexture(width, height, pixels, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
    }


    NIA_GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

niaCubeTexture::niaCubeTexture(){
}

niaCubeTexture::~niaCubeTexture(){
}

NIA_INTERNAL void niaCubeTexture::allocateTexture(u32 width, u32 height, u8* data, u32 side){ // TODO add ability to change depth
    NIA_GL_CALL(glTexImage2D(side, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data));
}
