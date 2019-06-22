/*
 * File: nia_png_parser.cpp
 * Project: Nia
 * File created on May-16-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "parsers/nia_png_parser.h"


#define SWAP32(_source) \
    ((_source & 0xFF) << 24) | ((_source & 0xFF00) << 16) | ((_source & 0xFF0000) >> 8) | ((_source & 0xFF000000) >> 24)

#define SWAP16(_source) \
    ((_source & 0xFF) << 8) | (_source >> 8)

#define MAKEU32(_source) \
    ((_source[0]&0xFF) << 24) | ((_source[1]&0xFF) << 16) | ((_source[2]&0xFF) << 8) | ((_source[3]&0xFF))

#define WASTE(_source, _amount){\
    _source += _amount;\
}

#define CONSUME(_source, _typedest, _dest) {\
    _dest = *((_typedest*)_source);\
    _source += sizeof(_typedest);\
}

#define compareHeader(_needed, _got) \
    (\
    (_needed[3] == ((_got & 0xFF000000) >> 24) & 0xFF) && \
    (_needed[2] == ((_got & 0x00FF0000) >> 16) & 0xFF) && \
    (_needed[1] == ((_got & 0x0000FF00) >>  8) & 0xFF) && \
    (_needed[0] == ((_got & 0x000000FF)      ) & 0xFF))


NIA_STATIC NIA_INLINE u8 readBit(niaPNGChunkData*& chunk){
    u8 result = (chunk->data[chunk->bitsUsed >> 3] >> ((chunk->bitsUsed++) & 7)) & 1;
    if((chunk->bitsUsed >> 3) >= chunk->size){
        chunk = chunk->next;
        fflush(stdout);
    }

    if(!chunk){
        NIA_TRACE("Something is wrong? Wanted to read more bits than in file.");
        return result;
    }
    return result;
}

NIA_STATIC NIA_INLINE u32 readBits(niaPNGChunkData*& chunk, u32 amount){
    u32 result = 0;
    for(u32 i = 0; i < amount; ++i){
        if((chunk->bitsUsed >> 3) >= chunk->size){
            chunk = chunk->next;
            fflush(stdout);
        }

        if(!chunk){
            NIA_TRACE("Something is wrong? Wanted to read more bits than in file.");
            return result;
        }
        result |= readBit(chunk) << i;
    }
    return result;
}

NIA_STATIC NIA_INLINE void wasteCurrentByte(niaPNGChunkData*& chunk){
    while(chunk->bitsUsed & 7 != 0){
        if((chunk->bitsUsed >> 3) >= chunk->size){
            chunk = chunk->next;
            fflush(stdout);
        }

        if(!chunk){
            NIA_TRACE("Something is wrong? Wanted to read more bits than in file.");
            break;
        }
        chunk->bitsUsed++;
    }
}

NIA_STATIC NIA_INLINE void writeToOutput(niaPNGDecompresedOutput* output, u8 data){
    *(output->data + (output->used++)) = data;
}

NIA_STATIC NIA_INLINE void writeToOutputAmount(niaPNGDecompresedOutput* output, u8* data, u32 amount){
    while(amount--){
        *(output->data + (output->used++)) = *(data++);
    }
}

NIA_STATIC NIA_INLINE void copyDataToOutput(u8* data, niaPNGDecompresedOutput* output, u32 amount){
    while(amount--){
        *(output->data + (output->used++)) = *(data++);
    }
}

NIA_STATIC NIA_INLINE void copyChunkToOutput(niaPNGChunkData*& chunk, niaPNGDecompresedOutput* output, u32 amount){
    while(amount--){
        *(output->data + (output->used++)) = *(chunk->data + (chunk->bitsUsed >> 3));
        chunk->bitsUsed += 8;
        if((chunk->bitsUsed >> 3) >= chunk->size){
            chunk = chunk->next;
            fflush(stdout);
        }

        if(!chunk){
            NIA_TRACE("Something is wrong? Wanted to read more bits than in file.");
            break;
        }
    }
}

NIA_STATIC u32 lengthBases[] = {
    3, 4, 5, 6, 7, 8, 9, 10,
    11, 13, 15, 17, 19, 23 ,27, 31, 35, 43, 51, 59,
    67, 83, 99, 115, 131, 163, 195, 227, 258
};

NIA_STATIC u32 lengthExtraBits[] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1,
    2, 2, 2, 2,
    3, 3, 3, 3,
    4, 4, 4, 4,
    5, 5, 5, 5,
    0
};

NIA_STATIC u32 distanceBases[] = {
    1, 2, 3, 4, 5, 7, 9, 13, 17, 25,
    33, 49, 65, 97, 129, 193, 257, 385, 513, 769, 
    1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577
};

NIA_STATIC u32 distanceExtraBits[] = {
    0, 0, 0, 0,
    1, 1, 2, 2, 
    3, 3, 4, 4,
    5, 5, 6, 6,
    7, 7, 8, 8,
    9, 9, 10, 10, 
    11, 11, 12, 12, 13, 13
};

niaPngParser::niaPngParser(){}

niaPngParser::niaPngParser(const char* filename){
    loadFile(filename);
    iterateHeader();

    switch(ihdr.colorType){
        case 2: {
                textureFormat = NIA_TEXTURE_FORMAT_RGB_RGB_UBYTE;
                numberOfComponents = 3;
            }
            break;
        case 6: default: {
                textureFormat = NIA_TEXTURE_FORMAT_RGBA_RGBA_UBYTE;
                numberOfComponents = 4;
            }
            break;
    }


    decompressedOutput.used = 0;
    decompressedOutput.data = new u8[ihdr.width * ihdr.height * numberOfComponents];

    decompressChunks();
    defilterData();
}

niaPngParser::~niaPngParser(){}

void niaPngParser::loadFile(const char* filename){
    FILE* file = fopen(filename, "rb");

    if(!file){
        NIA_ERROR("File could not be read.\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    u32 size = ftell(file);
    fseek(file, 0, SEEK_SET);

    pngData = new u8[size];

    fread(pngData, sizeof(u8), size, file);

    fclose(file);
}

static void compressedStreamInit(niaPNGCompressedStream* stream){
    stream->head = NULL;
    stream->tail = NULL;
}

static niaPNGChunkData* chunkDataInit(niaPNGCompressedStream* stream, u32 size, u8* input){
    niaPNGChunkData* current = new niaPNGChunkData;
    current->next = NULL;
    current->bitsUsed = 0;
    current->size = size;
    current->data = input;

    if(stream->head){
        stream->tail->next = current;
        stream->tail = current;
        return current;
    }

    stream->head = stream->tail = current;
    return current;
}

void niaPngParser::iterateHeader(){
    u8* source = pngData;

    WASTE(source, NIA_PNG_SIGNATURE_SIZE);

    niaPNGHeader header;

    compressedStreamInit(&stream);
    
    u8 done = 0;

    while(!done){
        CONSUME(source, u32, header.length);
        header.length = SWAP32(header.length);
    
        CONSUME(source, u32, header.type);

        header.data = source;

        if(compareHeader("IHDR", header.type)){
            CONSUME(header.data, niaPNGHeaderIHDR, ihdr);
            ihdr.width = SWAP32(ihdr.width);
            ihdr.height = SWAP32(ihdr.height);
        } else if(compareHeader("IEND", header.type)){
            done = 1;
        } else if(compareHeader("IDAT", header.type)){
            niaPNGChunkData* chunk = chunkDataInit(&stream, header.length, header.data);        
            WASTE(header.data, header.length);
        } else if(compareHeader("pHYs", header.type)){
            CONSUME(header.data, niaPNGHeaderPHYS, phys);
            phys.xPPU = SWAP32(phys.xPPU);
            phys.yPPU = SWAP32(phys.yPPU);
        } else if(compareHeader("gAMA", header.type)){
            CONSUME(header.data, niaPNGHeaderGAMA, gama);
            gama.imageGamma = SWAP32(gama.imageGamma); // gamma * 100000
        } else if(compareHeader("tIME", header.type)){
            CONSUME(header.data, niaPNGHeaderTIME, time);
            time.year = SWAP16(time.year);
        } else if(compareHeader("tEXt", header.type)){
#if 0
            u8* keyword = header.data;
            NIA_TRACE("\t%s\n", keyword);

            while(*keyword++);
            u8* text = keyword;

            u32 count = header.length - (keyword - header.data); // playing with fire

            NIA_TRACE("\t");
            while(count--){
                NIA_TRACE("%c", *text++);
            }
            NIA_TRACE("\n");
#endif
            WASTE(header.data, header.length);
        } else {
            WASTE(header.data, header.length);
        }

        source = header.data;

        CONSUME(source, u32, header.crc);
    }
}

void niaPngParser::methodNoCompression(niaPNGChunkData*& chunk){
    wasteCurrentByte(chunk);
    u16 len = readBits(chunk, 16);
    u16 nlen = readBits(chunk, 16);

    if(len == ~nlen){
        NIA_ERROR("Saddly there was an error while trying to process the file.\n");
        return;
    }
    copyChunkToOutput(chunk, &decompressedOutput, len);
}

NIA_INLINE i16 getSymbolIfCodeIsReal(u32 code, u16 length, niaPNGHuffmanTable* table){
    for(int entry = 0; entry <= table->numberOfEntries; ++entry){
        if(code == table->entries[entry].code && length == table->entries[entry].codeLength){
            return table->entries[entry].symbol;
        }
    }
    return -1;
}

NIA_INLINE void huffmanCreate(niaPNGHuffmanTable* table, u32* lengths, u32 numberOfCodes, u32 codeOffset = 0){
    table->numberOfEntries = numberOfCodes + 1;
    u32 lengths2[16 + 1] = {}; 
    u32 nextCode[16 + 1] = {};

    lengths2[0] = 0;
    for (u32 code = 0; code < numberOfCodes; ++code){
        ++lengths2[lengths[code]];
    }

    nextCode[0] = 0;
    for(u32 length = 1; length <= 16; ++length){
        nextCode[length] = ((nextCode[length - 1] + lengths2[length - 1]) << 1) + codeOffset;
    }

    table->entries = new niaPNGHuffmanEntry[table->numberOfEntries + 1];
    NIA_ASSERT(table->entries);

    for(int i = 0; i <= table->numberOfEntries; ++i){
        table->entries[i].codeLength = 0;
        table->entries[i].symbol = 0;
        table->entries[i].code = 0; // kind of useless, because the length is more important here, the code can be 0 anyway
    }

    for (u32 entry = 0; entry <= numberOfCodes; ++entry) {
        if (lengths[entry]) {
            u32 code = nextCode[lengths[entry]]++;
            u32 codeMask = (1 << lengths[entry]) - 1;
            code &= codeMask;
            table->entries[entry].code = code;
            table->entries[entry].symbol = entry;
        }
        table->entries[entry].codeLength = lengths[entry];
    }
}

NIA_INLINE u16 decodeHuffmanSymbol(niaPNGHuffmanTable* table, niaPNGChunkData*& chunk){
    u32 code = 0;
    u32 readBits = 0;
    i16 symbol = 0;

    for(;;){
        code <<= 1;
        code |= readBit(chunk);

        ++readBits;

        symbol = getSymbolIfCodeIsReal(code, readBits, table);
        if(symbol != -1){
            return symbol;
        }
    }
}

void niaPngParser::methodFixedHuffman(niaPNGChunkData*& chunk){
//      0 - 143     8          00110000 through
//                             10111111
//    144 - 255     9          110010000 through
//                             111111111
//    256 - 279     7          0000000 through
//                             0010111
//    280 - 287     8          11000000 through
//                             11000111
    u32 litLenLengths[512] = {};
    u32 distLengths[512] = {};

    for(u16 i = 0; i <= 143; ++i){
        litLenLengths[i] = 8;
    }

    for(u16 i = 144; i <= 255; ++i){
        litLenLengths[i] = 9;
    }

    for(u16 i = 256; i <= 279; ++i){
        litLenLengths[i] = 7;
    }

    for(u16 i = 280; i <= 287; ++i){
        litLenLengths[i] = 8;
    }

    for(u16 i = 0; i <= 32; ++i){
        distLengths[i] = 5;
    }

    niaPNGHuffmanTable litLengthAlphabet;
    huffmanCreate(&litLengthAlphabet, litLenLengths, 288);

    niaPNGHuffmanTable distLengthAlphabet;
    huffmanCreate(&distLengthAlphabet, distLengths, 32);

    for(;;){
        u32 symbol = decodeHuffmanSymbol(&litLengthAlphabet, chunk);
        if(symbol < 256){
            writeToOutput(&(decompressedOutput), symbol);
        } else {
            if(symbol == 256){
                break;
            } else if (symbol >= 257 && symbol <= 288){
                // symbol is converted to length here
                u32 length = lengthBases[symbol - 257];
                u32 extraLengthBitsAmount = lengthExtraBits[symbol - 257];
                u32 extraLegnthBits = readBits(chunk, extraLengthBitsAmount);
                u32 finalLength = length + extraLegnthBits;

                // distance
                u32 distanceIndex = decodeHuffmanSymbol(&distLengthAlphabet, chunk);
                u32 distance = distanceBases[distanceIndex];
                u32 extraDistanceBitsAmount = distanceExtraBits[distanceIndex];
                u32 extraDistanceBits = readBits(chunk, extraDistanceBitsAmount);
                u32 finalDistance = distance + extraDistanceBits;
                writeToOutputAmount(&(decompressedOutput),
                             (decompressedOutput.data + (decompressedOutput.used - finalDistance)), finalLength);
            }
        }
    }
}

void niaPngParser::methodDynamicHuffman(niaPNGChunkData*& chunk){
    u32 numberOfLiteralsCodes = readBits(chunk, 5) + 257;
    u32 numberOfDistancesCodes = readBits(chunk, 5) + 1;
    u32 numberOfLengthsCodes = readBits(chunk, 4) + 4;

    u8 lengths[] = {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
    u32 lengthHistogram[20] = {0};

    u32 litLenLengths[512] = {0};
    u32 distLengths[512] = {0};

    for(u32 i = 0; i < 19; ++i){
        if(i < numberOfLengthsCodes){
            u8 codeLength = readBits(chunk, 3);
            lengthHistogram[lengths[i]] = codeLength;
        }
    }

    niaPNGHuffmanTable codeLengthAlphabet;
    huffmanCreate(&codeLengthAlphabet, lengthHistogram, 19);
    
    for(u16 i = 0; i < numberOfLiteralsCodes; ++i){
        u16 repeats = 0;
        u16 repeatValue = 0;
        u16 length = decodeHuffmanSymbol(&codeLengthAlphabet, chunk);
        
        if(length < 16){
            litLenLengths[i] = length;
            continue;
        } else if(length == 16){
            repeats = 3 + readBits(chunk, 2);
            repeatValue = litLenLengths[i - 1];
        } else if(length == 17){
            repeats = 3 + readBits(chunk, 3);
        } else if(length == 18){
            repeats = 11 + readBits(chunk, 7);
        } else {
            break;
        }

        for(u32 repeat = 0; repeat < repeats; ++repeat){
            litLenLengths[i++] = repeatValue;
        }
        
        --i;
    }

    for(u16 i = 0; i < numberOfDistancesCodes; ++i){
        u16 repeats = 0;
        u16 repeatValue = 0;
        u16 length = decodeHuffmanSymbol(&codeLengthAlphabet, chunk);

        if(length < 16){
            distLengths[i] = length;
            continue;
        } else if(length == 16){
            repeats = 3 + readBits(chunk, 2);
            repeatValue = distLengths[i - 1];
        } else if(length == 17){
            repeats = 3 + readBits(chunk, 3);
        } else if(length == 18){
            repeats = 11 + readBits(chunk, 7);
        } else {
            break;
        }

        for(u32 repeat = 0; repeat < repeats; ++repeat){
            distLengths[i++] = repeatValue;
        }

        --i;
    }

    niaPNGHuffmanTable litLengthAlphabet;
    huffmanCreate(&litLengthAlphabet, litLenLengths, 288);

    niaPNGHuffmanTable distLengthAlphabet;
    huffmanCreate(&distLengthAlphabet, distLengths, 32);
    
    for(;;){
        u32 symbol = decodeHuffmanSymbol(&litLengthAlphabet, chunk);
        if(symbol < 256){
            writeToOutput(&(decompressedOutput), symbol);
        } else {
            if(symbol == 256){
                break;
            } else if (symbol >= 257 && symbol <= 285){
                // symbol is converted to length here
                u32 length = lengthBases[symbol - 257];
                u32 extraLengthBitsAmount = lengthExtraBits[symbol - 257];
                u32 extraLegnthBits = readBits(chunk, extraLengthBitsAmount);
                u32 finalLength = length + extraLegnthBits;

                // distance
                u32 distanceIndex = decodeHuffmanSymbol(&distLengthAlphabet, chunk);
                u32 distance = distanceBases[distanceIndex];
                u32 extraDistanceBitsAmount = distanceExtraBits[distanceIndex];
                u32 extraDistanceBits = readBits(chunk, extraDistanceBitsAmount);
                u32 finalDistance = distance + extraDistanceBits;
                writeToOutputAmount(&(decompressedOutput),
                             (decompressedOutput.data + (decompressedOutput.used - finalDistance)), finalLength);
            }
        }
    }
}

void niaPngParser::decompressChunks(){
    niaPNGChunkData*& current = stream.head;

    u8 compressionMethod = readBits(current, 4);
    u8 compressionInfo = readBits(current, 4);

    u8 fcheck = readBits(current, 5);
    u8 fdict = readBit(current);
    u8 flevel = readBits(current, 2);

    NIA_ASSERT(compressionMethod == 8);

    u8 btype = 0;
    u8 bfinal = 0;
    while(!bfinal){
        bfinal = readBit(current);
        btype = readBits(current, 2);

        switch(btype){
            case NIA_PNG_METHOD_NO_COMPRESSION: {
                    methodNoCompression(current);
                }
                break;

            case NIA_PNG_METHOD_FIXED_HUFFMAN:{
                    methodFixedHuffman(current);
                }
                break;                

            case NIA_PNG_METHOD_DYNAMIC_HUFFMAN:{
                    methodDynamicHuffman(current);
                }
                break;             

            case NIA_PNG_METHOD_ERROR:{
                    NIA_ERROR("Invalid zlib chunk detected, the png file is corrupted.\n");
                }
                break;                

            default:{
                }
                break;
        }
    }
}

void niaPngParser::defilterData(){
    switch(ihdr.filterMethod){
        case NIA_PNG_FILTER0:{
                // TODO change this depending on the color type
#if 0
                for(int i = 0;  i < ihdr.width * ihdr.height * numberOfComponents; ++i){
                    if(!(i % ihdr.width * numberOfComponents)){
                        printf("\n");
                    }
                    printf("%x ", decompressedOutput.data[i]);
                }
#endif
                pixelData = new u8[ihdr.width * ihdr.height * numberOfComponents];

                u32 pixelOffset = 0;
                u32 dataOffset = 1;

                for(int y = 0; y < ihdr.height; ++y){
                    for(int i = 0; i < ihdr.width * numberOfComponents; ++i){
                        pixelData[pixelOffset++] = decompressedOutput.data[dataOffset++];
                    }
                    ++dataOffset;
                }
            }
            break;
        
        case NIA_PNG_FILTER1:{
            }
            break;

        case NIA_PNG_FILTER2:{
            }
            break;
        
        case NIA_PNG_FILTER3:{
            }
            break;
        
        case NIA_PNG_FILTER4:{
            }
            break;
    }
}

u32 niaPngParser::getWidth(){
    return ihdr.height;
}

u32 niaPngParser::getHeight(){
    return ihdr.height;
}

u8* niaPngParser::getPixelData() const {
    return pixelData;
}

textureFormatDetails niaPngParser::getTextureFormat() const {
    return textureFormat;
}
