#ifndef _NIA_BMP_PARSER_H_
#define _NIA_BMP_PARSER_H_

#include "nia_general.h"


#define NIA_BMP_1_BIT 1
#define NIA_BMP_4_BIT 4
#define NIA_BMP_8_BIT 8
#define NIA_BMP_16_BIT 16
#define NIA_BMP_24_BIT 24
#define NIA_BMP_32_BIT 32

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

#pragma pack(push, 1)

typedef struct niaBMPHeader{
    u8 type[2];
    u32 size;
    u16 reserved1;
    u16 reserved2;
    u32 offsetBits;
} niaBMPHeader;

typedef struct niaBMPImageHeader{
    u32 size;
    u32 width;
    u32 height;
    u16 planes;
    u16 bitCount;
    u32 compression;
    u32 sizeImage;
    u32 xppm;
    u32 yppm;
    u32 colorMapEntries;
    u32 significantColors;
} niaBMPImageHeader;

typedef struct niaBMP{
    niaBMPHeader fileHeader;  
    niaBMPImageHeader imageHeader;
    u8* fileData;
    u8* pixelData;
} niaBMP;

#pragma pack(pop)

NIA_CLASS niaBmpParser { // make a parser class
private:
    niaBMP bmpData;

    NIA_CALL void decodeFile();

public:
    NIA_CALL niaBmpParser(const char* filename);
    NIA_CALL ~niaBmpParser();

    NIA_CALL void loadFile(const char* filename);
    NIA_CALL u32 getWidth() const;
    NIA_CALL u32 getHeight() const;
    NIA_CALL u8* getPixelData() const;
    NIA_CALL u8 getBitCount() const;
};

#endif // _NIA_BMP_PARSER_H_