/*
 * File: nia_gif_parser.h
 * Project: Nia
 * File created on May-18-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef _NIA_GIF_PARSER_H_
#define _NIA_GIF_PARSER_H_

#include "nia_general.h"
#include "nia_texture_structures.h"

#define NIA_DISPOSAL_NO_ACTION 0
#define NIA_DISPOSAL_LEFT_IN_PLACE 1
#define NIA_DISPOSAL_RESTORE_BACKGROUND 2
#define NIA_DISPOSAL_RESTORE_PROVIOUS 3

#pragma pack(push, 1)
// https://www.w3.org/Graphics/GIF/spec-gif89a.txt
NIA_STRUCT niaGIFHeader {
    u8 signature[3];
    u8 version[3];
} niaGIFHeader;

NIA_STRUCT niaGIFLogicalScreenDescriptor {
    u16 localScreenWidth;
    u16 localScreenHeight;
    u8 packedFields;
    u8 backgroundColorIndex;
    u8 pixelAspectRatio;
} niaGIFLogicalScreenDescriptor;

NIA_STRUCT niaGIFImageDescriptor {
    u16 imageLeftPosition;
    u16 imageTopPosition;
    u16 imageWidth;
    u16 imageHeight;
    u8 packedFields;
} niaGIFImageDescriptor;


NIA_STRUCT niaGIFGraphicControlExtension {
    u8 blockSize;
    u8 packedFileds;
    u16 delayTime;
    u8 transparentColorIndex;
    u8 blockTerminator; // 0 ?
} niaGIFGraphicControlExtension;

NIA_STRUCT niaGIFApplicationExtension {
    u8 blockSize;
    u8 applicationIndentifire[8];
    u8 applicationAuthentificationCode[3];
} niaGIFApplicationExtension;
#pragma pack(pop)

NIA_STRUCT niaGIFFrame {
    u32 width;
    u32 height;
    u8* data;
} niaGIFFrame;

NIA_STRUCT niaGIFLZWEntry {
    u16 length; 
    u16* values;
} niaGIFLZWEntry;

NIA_STRUCT niaGIFLZWDictionary {
    u16 size;
    niaGIFLZWEntry entries[4097];
} niaGIFLZWDictionary;

NIA_STRUCT niaGIFImageDecodeStreamChunk {
    u32 usedBits;
    u16 dataSize;
    u8* data;
    struct niaGIFImageDecodeStreamChunk* next;
} niaGIFImageDecodeStreamChunk;

NIA_STRUCT niaGIFImageDecodeStream {
    niaGIFImageDecodeStreamChunk* head;
    niaGIFImageDecodeStreamChunk* tail;
} niaGIFImageDecodeStream;

NIA_CLASS niaGifParser {
private:
    u32 width;
    u32 height;

    u32 fileSize;
    u8* data;

    u8 disposalMethod;
    u8 numberOfFrames;
    u8 transparentColorIndex;
    u8 backgroundColorIndex;
    u8 hasTransperancy;

    niaGIFFrame frames[128];

    textureFormatDetails textureFormat;

    void loadFile(const char* filename);
    void decodeFile();
    void decodeImageData(u8* colorTable);

public:
    NIA_CALL niaGifParser();
    NIA_CALL niaGifParser(const char* filename);
    NIA_CALL ~niaGifParser();

    u32 NIA_CALL getWidth();
    u32 NIA_CALL getHeight();
    u8* NIA_CALL getPixelData() const;
    textureFormatDetails NIA_CALL getTextureFormat() const;
    niaGIFFrame* NIA_CALL getFrames();
    u32 NIA_CALL getNumberOfFrames();
};

#endif