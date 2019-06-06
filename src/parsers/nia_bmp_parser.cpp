/*
 * File: nia_bmp_parser.cpp
 * Project: Nia
 * File created on April-29-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "parsers/nia_bmp_parser.h"
#include <malloc.h>

#define CONSUME(_stream, _type, _dest) {\
    _dest = *((_type*)_stream);\
    _stream += sizeof(_type);\
}

niaBmpParser::niaBmpParser(const char* filename){
    loadFile(filename);
    decodeFile();
}

niaBmpParser::~niaBmpParser(){ // TODO free buffers
}

void niaBmpParser::loadFile(const char* filename){
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

void niaBmpParser::decodeFile(){
    u8* source = bmpData.fileData;
    if(!source){
        return;
    }
    
    CONSUME(source, niaBMPHeader, bmpData.fileHeader);
    CONSUME(source, niaBMPImageHeader, bmpData.imageHeader);

    u8* colorTable = source;
    source += bmpData.imageHeader.colorMapEntries * sizeof(u8) * 4;

    bmpData.textureFormat = NIA_TEXTURE_FORMAT_RGB_BGR_UBYTE; // most common one, this is for 24 bit bmps

    u8 padding = (bmpData.imageHeader.width % 4) != 0;

    u8 skipAmount = 3;

    if(padding){
        ++skipAmount;
    }

    if(padding){
        bmpData.textureFormat = NIA_TEXTURE_FORMAT_RGBA_RGBA_UBYTE;
    }

    u32 textureSize = bmpData.imageHeader.width * bmpData.imageHeader.height * skipAmount;

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
                            u8* sourceCopy = source;

                            u8 amount;
                            CONSUME(source, u8, amount);

                            u8 value;
                            CONSUME(source, u8, value);

                            u32 index = 0;
                            bmpData.pixelData = new u8[textureSize];

                            bool end = false;

                            while(!end){
                                if(!amount){
                                    switch (value){
                                        case 0:{ // end of line
                                            }
                                            break;

                                        case 1:{ // end of bitmap
                                                end = !end;
                                            }
                                            break;
                                        
                                        case 2:{ // offset party
                                                u8 x;
                                                u8 y;

                                                CONSUME(source, u8, x);
                                                CONSUME(source, u8, y);

                                                index += x + y * bmpData.imageHeader.width * skipAmount;
                                            }
                                            break;
                                        // TODO refactor use function for pixel write...
                                        default:{
                                                if(value >= 3){
                                                    u8 colorTableIndices;
                                                    u8 indices[2];

                                                    while(value--){
                                                        CONSUME(source, u8, colorTableIndices);
                                                        indices[0] = (colorTableIndices & 0xf0) >> 4;
                                                        indices[1] = colorTableIndices & 0x0f;

                                                        for(u8 i = 0; i < 2; ++i){
                                                            bmpData.pixelData[index + 0] = colorTable[indices[i] * 4 + 0];
                                                            bmpData.pixelData[index + 1] = colorTable[indices[i] * 4 + 1];
                                                            bmpData.pixelData[index + 2] = colorTable[indices[i] * 4 + 2];
                                                            if(padding){
                                                                bmpData.pixelData[index + 3] = colorTable[indices[i] * 4 + 3];
                                                                ++index;
                                                            }
                                                            index += 3;
                                                        }
                                                    }
                                                }
                                            }
                                            break;
                                    }
                                } else {    
                                    u8 indices[2];
                                    indices[0] = (value & 0xf0) >> 4;
                                    indices[1] = value & 0x0f;

                                    u8 counter = 0;

                                    while(amount--){
                                        bmpData.pixelData[index + 0] = colorTable[indices[counter % 2] * 4 + 0];
                                        bmpData.pixelData[index + 1] = colorTable[indices[counter % 2] * 4 + 1];
                                        bmpData.pixelData[index + 2] = colorTable[indices[counter % 2] * 4 + 2];
                                        if(padding){
                                            bmpData.pixelData[index + 3] = colorTable[indices[counter % 2] * 4 + 3];
                                            ++index;
                                        }
                                        index += 3;

                                        ++counter;
                                    }
                                }

                                if((source - sourceCopy) % 2){
                                    source += sizeof(u8);
                                }

                                CONSUME(source, u8, amount);
                                CONSUME(source, u8, value);
                            }

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
                            u8* sourceCopy = source;

                            u8 amount;
                            CONSUME(source, u8, amount);

                            u8 value;
                            CONSUME(source, u8, value);

                            u32 index = 0;
                            bmpData.pixelData = new u8[textureSize];

                            bool end = false;

                            while(!end){
                                if(!amount){
                                    switch (value){
                                        case 0:{ // end of line
                                            }
                                            break;

                                        case 1:{ // end of bitmap
                                                end = !end;
                                            }
                                            break;
                                        
                                        case 2:{ // offset party
                                                u8 x;
                                                u8 y;

                                                CONSUME(source, u8, x);
                                                CONSUME(source, u8, y);

                                                index += x + y * bmpData.imageHeader.width * skipAmount;
                                            }
                                            break;
                                        // TODO refactor use function for pixel write...
                                        default:{
                                                if(value >= 3){
                                                    u8 colorTableIndex;
                                                    while(value--){
                                                        CONSUME(source, u8, colorTableIndex);

                                                        bmpData.pixelData[index + 0] = colorTable[colorTableIndex * 4 + 0];
                                                        bmpData.pixelData[index + 1] = colorTable[colorTableIndex * 4 + 1];
                                                        bmpData.pixelData[index + 2] = colorTable[colorTableIndex * 4 + 2];
                                                        if(padding){
                                                            bmpData.pixelData[index + 3] = colorTable[colorTableIndex * 4 + 3];
                                                            ++index;
                                                        }
                                                        index += 3;
                                                    }
                                                }
                                            }
                                            break;
                                    }
                                } else {
                                    while(amount--){
                                        bmpData.pixelData[index + 0] = colorTable[value * 4 + 0];
                                        bmpData.pixelData[index + 1] = colorTable[value * 4 + 1];
                                        bmpData.pixelData[index + 2] = colorTable[value * 4 + 2];
                                        if(padding){
                                            bmpData.pixelData[index + 3] = colorTable[value * 4 + 3];
                                            ++index;
                                        }
                                        index += 3;
                                    }
                                }


                                if((source - sourceCopy) % 2){
                                    source += sizeof(u8);
                                }

                                CONSUME(source, u8, amount);
                                CONSUME(source, u8, value);
                            }

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
                NIA_ERROR("Bit count unsupported: %d\n", bmpData.imageHeader.bitCount);
            }
            break;
    }
}

u32 niaBmpParser::getWidth() const {
    return bmpData.imageHeader.width;
}

u32 niaBmpParser::getHeight() const {
    return bmpData.imageHeader.height;
}

u8* niaBmpParser::getPixelData() const {
    return bmpData.pixelData;
}

u8 niaBmpParser::getBitCount() const {
    return bmpData.imageHeader.bitCount;
}

textureFormatDetails niaBmpParser::getTextureFormat() const {
    return bmpData.textureFormat;
}