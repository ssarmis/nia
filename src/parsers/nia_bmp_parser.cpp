#include "parsers/nia_bmp_parser.h"
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

    bmpData.fileData = new u8[size];

    fread(bmpData.fileData, sizeof(u8), size, file);

    fclose(file);
}

#define SWAP32(_x) (((_x) >> 16) | \
                    (((_x) & 0x00ff0000) >> 8) | \
                    (((_x) & 0x0000ff00) << 8) | \
                    (((_x) & 0x000000ff) << 16))

NIA_CALL void niaBmpParser::decodeFile(){
    u8* source = bmpData.fileData;
    if(!source){
        return;
    }
    
    bmpData.fileHeader = *(niaBMPHeader*)(source);
    source += sizeof(niaBMPHeader);

    bmpData.imageHeader = *(niaBMPImageHeader*)(source);
    source += sizeof(niaBMPImageHeader);

    u8* colorTable = source;
    source += bmpData.imageHeader.colorMapEntries * sizeof(u8) * 4;

    bmpData.textureFormat = NIA_TEXTURE_FORMAT_RGB_BGR_UBYTE; // most common one, this is for 24 bit bmps

    switch(bmpData.imageHeader.bitCount){
        case NIA_BMP_24_BIT:{
                bmpData.pixelData = source;
                // TODO make this work even if outise scope of object... e.g. not loading the niaTexture in the same scope as where this object was created ma leave to problems if source is freed
            }
            break;

        case NIA_BMP_4_BIT:{ // I cound't find one of these, they are very rare, I could force one to generate, but if they are so rare, I have no interest
                switch(bmpData.imageHeader.compression){
                    case NIA_BMP_COMPRESSION_0:{
                            // no compression....

                        }
                        break;

                    case NIA_BMP_COMPRESSION_2:{

                        }
                        break;
                    default:{
                            NIA_ERROR("Invalid compression for specified bit count!\n");
                        }
                        break;
                }
            }
            break;

        case NIA_BMP_8_BIT:{
                switch(bmpData.imageHeader.compression){
                    case NIA_BMP_COMPRESSION_0:{
                            // no compression.... but 8 bits so we need to search in the color table
                            
                            u8 padding = bmpData.imageHeader.width % 4;
                            
                            while(padding % 4){
                                ++padding;
                            }

                            u8 skipAmount = 3;

                            if(padding){
                                ++skipAmount;
                            }

                            u32 textureSize = bmpData.imageHeader.width * bmpData.imageHeader.height * skipAmount;
                            bmpData.pixelData = new u8[textureSize];

                            if(padding){
                                bmpData.textureFormat = NIA_TEXTURE_FORMAT_RGBA_RGBA_UBYTE;
                            }

                            for(int i = 0; i < textureSize; i += skipAmount){
                                if(padding){ // we have a padding, so we need to make it on a 4 byte boundry
                                    if(!(i % bmpData.imageHeader.width) && i){
                                        source += padding - 1;
                                    }
                                    bmpData.pixelData[i + 0] = colorTable[(*source) * 4 + 0];
                                    bmpData.pixelData[i + 1] = colorTable[(*source) * 4 + 1];
                                    bmpData.pixelData[i + 2] = colorTable[(*source) * 4 + 2];
                                    bmpData.pixelData[i + 3] = colorTable[(*source) * 4 + 3];
                                } else {
                                    bmpData.pixelData[i + 0] = colorTable[(*source) * 4 + 0];
                                    bmpData.pixelData[i + 1] = colorTable[(*source) * 4 + 1];
                                    bmpData.pixelData[i + 2] = colorTable[(*source) * 4 + 2];
                                }

                                ++source;
                            }
                        }
                        break;

                    case NIA_BMP_COMPRESSION_1:{
                        }
                        break;
                    default:{
                            NIA_ERROR("Invalid compression for specified bit count!\n");
                        }
                        break;
                }
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

NIA_CALL textureFormatDetails niaBmpParser::getTextureFormat() const {
    return bmpData.textureFormat;
}