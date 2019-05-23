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

NIA_STRUCT niaPNGHeaderTIME {
    u16 year;
    u8 month;
    u8 day;
    u8 hour;
    u8 minute;
    u8 second;
} niaPNGHeaderTIME;

NIA_STRUCT niaPNGHeaderPHYS {
    u32 xPPU;
    u32 yPPU;
    u8 unitSpecifier;
} niaPNGHeaderPHYS;

NIA_STRUCT niaPNGHeaderIHDR {
    u32 width;
    u32 height;
    u8 bitDepth;
    u8 colorType;
    u8 compressionMethod;
    u8 filterMethod;
    u8 interlanceMethod;
} niaPNGHeaderIHDR;

NIA_STRUCT niaPNGHuffmanEntry {
    u8 codeLength;
    u16 code;
    u16 symbol;
} niaPNGHuffmanEntry;
#pragma pack(pop)


NIA_STRUCT niaPNGHuffmanTable {
    niaPNGHuffmanEntry* entries;
    u16 numberOfEntries;
    u16* lengthCount[16 + 1];
} niaPNGHuffmanTable;

NIA_STRUCT niaPNGHeader {
    u32 length;
    u32 type;
    u8* data;
    u32 crc;
} niaPNGHeader;

NIA_STRUCT niaPNGDecompresedOutput {
    u32 used;
    u8* data;
} niaPNGDecompresedOutput;

NIA_STRUCT niaPNGChunkData {
    u32 size;
    u32 bitsUsed;
    u8* data;
    struct niaPNGChunkData* next;
} niaPNGChunkData;

NIA_STRUCT niaPNGCompressedStream {
    struct niaPNGChunkData* head;
    struct niaPNGChunkData* tail;
} niaPNGCompressedStream;

NIA_CLASS niaPngParser {
private:
    niaPNGHeaderIHDR ihdr;
    niaPNGHeaderTIME time;
    niaPNGHeaderPHYS phys;

    niaPNGDecompresedOutput decompressedOutput;
    niaPNGCompressedStream stream;
    textureFormatDetails textureFormat;

    u8* pngData;
    u8* pixelData;

    void methodDynamicHuffman(niaPNGChunkData* chunk);
    void methodNoCompression(niaPNGChunkData* chunk);

    void iterateHeader();
    void decompressChunks();
    void defilterData();
public:

    NIA_CALL niaPngParser();
    NIA_CALL niaPngParser(const char* filename);
    NIA_CALL ~niaPngParser();

    void NIA_CALL loadFile(const char* filename);
    u32 NIA_CALL getWidth();
    u32 NIA_CALL getHeight();
    u8* NIA_CALL getPixelData() const;
    textureFormatDetails NIA_CALL getTextureFormat() const ;
};

#endif