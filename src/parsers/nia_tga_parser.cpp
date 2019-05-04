#include "parsers/nia_tga_parser.h"

#include <malloc.h>

#define NIA_IMAGE_TYPE_TRUE_COLOR   2

NIA_CALL niaTgaParser::niaTgaParser(const char* filename){
    loadFile(filename);
    decodeFile();
}

NIA_CALL niaTgaParser::~niaTgaParser(){
}

NIA_CALL void niaTgaParser::loadFile(const char* filename){
    tgaData.fileData = NULL;
    FILE* file = fopen(filename, "rb");

    if(!file){
        NIA_ERROR("Could not find file %s\n", filename);
        return;
    }

    fseek(file, 0, SEEK_END);
    u32 size = ftell(file);
    fseek(file, 0, SEEK_SET);

    tgaData.fileData = new u8[size];

    fread(tgaData.fileData, sizeof(u8), size, file);

    fclose(file);
}

NIA_CALL void niaTgaParser::decodeFile(){ // TODO add consume macro
    if(!tgaData.fileData){
        return;
    }
    
    tgaData.fileHeader = *((niaTGAHeader*)(tgaData.fileData));
    tgaData.fileData += sizeof(niaTGAHeader);

#if 0
    printf("%d\n", tgaData.fileHeader.idLength);
    printf("%d\n", tgaData.fileHeader.colorMapType);
    printf("%d\n\n", tgaData.fileHeader.imageType);

    printf("%x %x %x %x %x\n",
        tgaData.fileHeader.colorMapSpecification[0],
        tgaData.fileHeader.colorMapSpecification[1],
        tgaData.fileHeader.colorMapSpecification[2],
        tgaData.fileHeader.colorMapSpecification[3],
        tgaData.fileHeader.colorMapSpecification[4]
    );

    printf("%x, %x \n", tgaData.fileHeader.xOrigin, tgaData.fileHeader.yOrigin);
    printf("%x %x \n", tgaData.fileHeader.width, tgaData.fileHeader.height);
    printf("%x \n", tgaData.fileHeader.pixelDepth);
    printf("%x \n", tgaData.fileHeader.imageDesciptor);
#endif

    if(tgaData.fileHeader.idLength){
        // TODO implement consume id section...
    }

    if(tgaData.fileHeader.colorMapType){
        // TODO implement consume color map section...
    }

    typedef struct niaPiexl24 { // TODO move this into something more general, don't leave here...
        u8 red;
        u8 green;
        u8 blue;
    } niaPiexl24;

    tgaData.textureFormat = NIA_TEXTURE_FORMAT_RGB_BGR_UBYTE; // most common for our suported tga formats

    switch (tgaData.fileHeader.imageType) {
        case NIA_IMAGE_TYPE_TRUE_COLOR:{
                tgaData.pixelData = tgaData.fileData;
            }
            break;
    
        default:{
                NIA_ERROR("Unsupported image type: %d", tgaData.fileHeader.imageType);
            }
            break;
    }

}

NIA_CALL u32 niaTgaParser::getWidth() const{
    return tgaData.fileHeader.width;
}

NIA_CALL u32 niaTgaParser::getHeight() const{
    return tgaData.fileHeader.height;
}

NIA_CALL u8*niaTgaParser:: getPixelData() const{
    return tgaData.pixelData;
}

NIA_CALL textureFormatDetails niaTgaParser::getTextureFormat() const {
    return tgaData.textureFormat;
}
