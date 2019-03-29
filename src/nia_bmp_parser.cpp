#include "nia_bmp_parser.h"

#include <malloc.h>

NIA_CALL niaBmpParser::niaBmpParser(const char* filename){
    loadFile(filename);
    decodeFile();
}

NIA_CALL niaBmpParser::~niaBmpParser(){ // TODO free buffers
}

NIA_CALL void niaBmpParser::loadFile(const char* filename){
    bmpData.fileData = NULL;
    FILE* file = fopen(filename, "rb");

    if(!file){
        NIA_ERROR("Could not find file %s\n", filename);
        return;
    }

    fseek(file, 0, SEEK_END);
    u32 size = ftell(file);
    fseek(file, 0, SEEK_SET);

    bmpData.fileData = (u8*)malloc(sizeof(u8) * size);

    fread(bmpData.fileData, sizeof(u8), size, file);

    fclose(file);
}

NIA_CALL void niaBmpParser::decodeFile(){
    u8* source = bmpData.fileData;
    if(!source){
        return;
    }

    bmpData.fileHeader = *(niaBMPHeader*)(source);
    source += sizeof(niaBMPHeader);


    bmpData.imageHeader = *(niaBMPImageHeader*)(source);
    source += sizeof(niaBMPImageHeader);

    switch(bmpData.imageHeader.bitCount){
        case NIA_BMP_24_BIT:{
                bmpData.pixelData = source;
                // TODO make this work even if outise scope of object... e.g. not loading the niaTexture in the same scope as where this object was created ma leave to problems if source is freed
            }
            break;

        default :{
                printf("Bit count unsupported: %d\n", bmpData.imageHeader.bitCount);
            }
            break;
    }
}

NIA_CALL u32 niaBmpParser::getWidth() const {
    return bmpData.imageHeader.width;
}

NIA_CALL u32 niaBmpParser::getHeight() const {
    return bmpData.imageHeader.height;
}

NIA_CALL u8* niaBmpParser::getPixelData() const {
    return bmpData.pixelData;
}

NIA_CALL u8 niaBmpParser::getBitCount() const {
    return bmpData.imageHeader.bitCount;
}
