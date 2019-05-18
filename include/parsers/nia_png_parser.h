#ifndef _NIA_PNG_PARSER_H_
#define _NIA_PNG_PARSER_H_

#include "nia_general.h"
#include "nia_texture_structures.h"

#define NIA_PNG_METHOD_NO_COMPRESSION   0
#define NIA_PNG_METHOD_FIXED_HUFFMAN    1
#define NIA_PNG_METHOD_DYNAMIC_HUFFMAN  2
#define NIA_PNG_METHOD_ERROR            3

#define NIA_PNG_FILTER0 0
#define NIA_PNG_FILTER1 1
#define NIA_PNG_FILTER2 2
#define NIA_PNG_FILTER3 3
#define NIA_PNG_FILTER4 4

#define NIA_PNG_SIGNATURE_SIZE 8

static u8 pngSignature[NIA_PNG_SIGNATURE_SIZE] = {137, 80, 78, 71, 13, 10, 26, 10};

#pragma pack(push, 1)

typedef struct niaPNGHeaderTIME {
    u16 year;
    u8 month;
    u8 day;
    u8 hour;
    u8 minute;
    u8 second;
} niaPNGHeaderTIME;

typedef struct niaPNGHeaderPHYS {
    u32 xPPU;
    u32 yPPU;
    u8 unitSpecifier;
} niaPNGHeaderPHYS;

typedef struct niaPNGHeaderIHDR {
    u32 width;
    u32 height;
    u8 bitDepth;
    u8 colorType;
    u8 compressionMethod;
    u8 filterMethod;
    u8 interlanceMethod;
} niaPNGHeaderIHDR;

typedef struct niaPNGHuffmanEntry {
    u8 codeLength;
    u16 code;
    u16 symbol;
} niaPNGHuffmanEntry;
#pragma pack(pop)


typedef struct niaPNGHuffmanTable {
    niaPNGHuffmanEntry* entries;
    u16 numberOfEntries;
    u16* lengthCount[16 + 1];
} niaPNGHuffmanTable;

typedef struct niaPNGHeader {
    u32 length;
    u32 type;
    u8* data;
    u32 crc;
} niaPNGHeader;

typedef struct niaPNGDecompresedOutput {
    u32 used;
    u8* data;
} niaPNGDecompresedOutput;

typedef struct niaPNGChunkData {
    u32 size;
    u32 bitsUsed;
    u8* data;
    struct niaPNGChunkData* next;
} niaPNGChunkData;

typedef struct niaPNGCompressedStream {
    struct niaPNGChunkData* head;
    struct niaPNGChunkData* tail;
} niaPNGCompressedStream;

typedef struct niaPNG {
    niaPNGHeaderIHDR ihdr;
    niaPNGHeaderTIME time;
    niaPNGHeaderPHYS phys;
    niaPNGDecompresedOutput decompressedOutput;
    niaPNGCompressedStream stream;
    textureFormatDetails textureFormat;
    u8* pngData;
    u8* pixelData;
} niaPNG;

static inline u8 readBit(niaPNGChunkData* chunk){
    return (chunk->data[chunk->bitsUsed >> 3] >> ((chunk->bitsUsed++) & 7)) & 1;
}

static inline u32 readBits(niaPNGChunkData* chunk, u32 amount){
    u32 result = 0;
    for(u32 i = 0; i < amount; ++i){
        result |= readBit(chunk) << i;
    }
    return result;
}

static inline u32 readBitsReversed(niaPNGChunkData* chunk, u32 amount){
    u32 result = 0;
    for(int i = amount - 1; i >= 0; --i){
        result |= readBit(chunk) << i;
    }
    return result;
}


static inline u32 lookAtBitsReversed(niaPNGChunkData* chunk, u32 amount){
    u32 result = readBitsReversed(chunk, amount);
    chunk->bitsUsed -= amount;
    return result;
} 

static inline u32 lookAtBits(niaPNGChunkData* chunk, u32 amount){
    u32 result = readBits(chunk, amount);
    chunk->bitsUsed -= amount;
    return result;
}

static inline void wasteCurrentByte(niaPNGChunkData* chunk){
    while(chunk->bitsUsed & 7 != 0){
        chunk->bitsUsed++;
    }
}

static inline void writeToOutput(niaPNGDecompresedOutput* output, u8 data){
    *(output->data + (output->used++)) = data;
}

static inline void writeToOutputAmount(niaPNGDecompresedOutput* output, u8* data, u32 amount){
    while(amount--){
        *(output->data + (output->used++)) = *(data++);
    }
}

static inline void copyDataToOutput(u8* data, niaPNGDecompresedOutput* output, u32 amount){
    while(amount--){
        *(output->data + (output->used++)) = *(data++);
    }
}

static inline void copyChunkToOutput(niaPNGChunkData* chunk, niaPNGDecompresedOutput* output, u32 amount){
    while(amount--){
        *(output->data + (output->used++)) = *(chunk->data + (chunk->bitsUsed >> 3));
        chunk->bitsUsed += 8;
    }
}

NIA_CLASS niaPngParser {
private:
    niaPNG png;
    NIA_CALL void methodDynamicHuffman(niaPNGChunkData* chunk);
    NIA_CALL void methodNoCompression(niaPNGChunkData* chunk);

public:
    NIA_CALL niaPngParser();
    NIA_CALL niaPngParser(const char* filename);
    NIA_CALL ~niaPngParser();

    NIA_CALL niaPNG loadFile(const char* filename);
    NIA_CALL void iterateHeader();
    NIA_CALL void decompressChunks();
    NIA_CALL void defilterData();
    NIA_CALL u32 getWidth();
    NIA_CALL u32 getHeight();
    NIA_CALL u8* getPixelData() const;
    NIA_CALL textureFormatDetails getTextureFormat() const ;
};

#endif