#ifndef _NIA_TGA_PARSER_H_
#define _NIA_TGA_PARSER_H_

#include "nia_general.h"
#include "nia_texture_structures.h"

#pragma pack(push, 1)
typedef struct niaTGAHeader {
    u8 idLength;
    u8 colorMapType;
    u8 imageType;
    u8 colorMapSpecification[5];
    union {
        u8 imageSpecification[10];
        struct {
            u16 xOrigin;
            u16 yOrigin;
            u16 width;
            u16 height;
            u8 pixelDepth;
            u8 imageDesciptor;
        };
    };
} niaTGAHeader;
#pragma pack(pop)

typedef struct niaTGA{
    textureFormatDetails textureFormat;
    niaTGAHeader fileHeader;
    u8* fileData;
    u8* pixelData;
} niaTGA;


NIA_CLASS niaTgaParser {
private:
    niaTGA tgaData;

    void decodeFile();

public:
    niaTgaParser(const char* filename);
    ~niaTgaParser();

    void loadFile(const char* filename);
    u32 getWidth() const;
    u32 getHeight() const;
    u8* getPixelData() const;
    textureFormatDetails getTextureFormat() const;

};

#endif //_NIA_TGA_PARSER_H_
