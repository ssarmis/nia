#ifndef _NIA_TTF_PARSER_H_
#define _NIA_TTF_PARSER_H_

#include "nia_general.h"
#include "nia_vector.h"

#define NIA_FORMAT_0 0
#define NIA_FORMAT_6 6


#define NIA_POINT_ON_CURVE      1
#define NIA_X_SHORT             2
#define NIA_Y_SHORT             4
#define NIA_FLAG_REPEAT         8
#define NIA_X_SAME              16
#define NIA_Y_SAME              32

#pragma pack(push, 1)

/////////////// HEADERS

NIA_STRUCT niaTTFHeadHeader {
    i32 version;
    i32 fontRevision;
    u32 checkSumAdjustment;
    u32 magicNumber;
    u16 flags;
    u16 unitsPerEm;
    i32 created[2];
    i32 modified[2];
    i16 xMin;
    i16 yMin;
    i16 xMax;
    i16 yMax;
    u16 macStyle;
    u16 lowestRecPPEM;
    i16 fontDirectionHint;
    i16 indexToLocFormat;
    i16 glyphDataFormat;
} niaTTFHeadHeader;

NIA_STRUCT niaTTFDirectoryFolder {
    u8	scalerType[4];
    u16	numTables;
    u16	searchRange;
    u16	entrySelector;
    u16	rangeShift;
} niaTTFDirectoryFolder;

NIA_STRUCT niaTTFHeader {
    u8 tag[4];
    u32 checkSum;
    u32 offset;
    u32 length;
} niaTTFHeader;

NIA_STRUCT niaTTFMaxPHeader {
    i32 version;
    u16	numGlyphs;
} niaTTFMaxPHeader;

NIA_STRUCT niaTTFHHeaHeader {
    i32 version;
    i16 ascent;
    i16 descent;
    i16 lineGap;
    u16 advanceWidthMax;
    i16 minLeftSideBearing;
    i16 minRightSideBearing;
    i16 xMaxExtent;
    i16 caretSlopeRise;
    i16 caretSlopeRun;
    i16 caretOffset;
    i16 reserved[4];
    i16 metricDataFormat;
    u16 numOfLongHorMetrics;
} niaTTFHHeaHeader;

NIA_STRUCT niaTTFVHeaHeader {
    i32	version;
    i16	vertTypoAscender;
    i16	vertTypoDescender;
    i16	vertTypoLineGap;
    i16	advanceHeightMax;
    i16	minTopSideBearing;
    i16	minBottomSideBearing;
    i16	yMaxExtent;
    i16	caretSlopeRise;
    i16	caretSlopeRun;
    i16	caretOffset;
    i16	reserved[4];
    i16	metricDataFormat;
    u16 numOfLongVerMetrics;
} niaTTFVHeaHeader;

NIA_STRUCT niaTTFGlyphHeader {
    i16	numberOfContours;
    i16	xMin;
    i16	yMin;
    i16	xMax;
    i16	yMax;
} niaTTFGlyphHeader;

NIA_STRUCT niaTTFCmapHeader {
    u16	version;
    u16	numberSubtables;
} niaTTFCmapHeader;

NIA_STRUCT niaTTFCmapSubtable {
    u16	platformID;
    u16	platformSpecificID;
    u32	offset;
} niaTTFCmapSubtable;

NIA_STRUCT niaTTFCmapSubtableGeneral {
    u16	format;
    u16	length;
    u16	language;
} niaTTFCmapSubtableGeneral;

NIA_STRUCT niaTTFCmapSubtableFormat0 {
    u8 glyphIndexArray[256];
} niaTTFCmapSubtableFormat0;

NIA_STRUCT niaTTFCmapSubtableFormat6 {
    u16	firstCode;
    u16	entryCount;
} niaTTFCmapSubtableFormat6;

NIA_STRUCT niaLongHorMetric {
    u16 advanceWidth;
    i16 leftSideBearing;
} niaLongHorMetric;

NIA_STRUCT niaLongVerMetric {
    u16 advanceHeight;
    i16 topSideBearing;
} niaLongVerMetric;

///////////////

NIA_STRUCT niaGlyphBounds {
    i32 xmin;
    i32 xmax;
    i32 ymin;
    i32 ymax;
} niaGlyphBounds;

NIA_STRUCT niaGlyphMetrics {
    i32 verticalAdvance;
    u32 cursorAdvance;
} niaGlyphMetrics;

NIA_STRUCT niaGlyphCompoundDetails {
    niaVec2i offsets[4];
    u16 additionalGlyphs[4];
} niaGlyphCompoundDetails;

NIA_STRUCT niaGlyphUVS {
    union {
        r32 uvs[4];
        struct {
            r32 u0;
            r32 v0;
            r32 u1;
            r32 v1;
        };
    };
} niaGlyphUVS;

NIA_STRUCT niaGlyph {
    niaGlyphBounds bounds;
    niaGlyphCompoundDetails compoundDetails;
    niaGlyphMetrics metrics;
    niaGlyphUVS uvs;
    u8* bitmap;
} niaGlyph;

NIA_STRUCT niaTTFPoint {
    u8 onCurve;
    i16 x;
    i16 y;
} niaTTFPoint;

#pragma pack(pop)

#define NIA_TTF_HEAD_POINTER 0
#define NIA_TTF_HHEA_POINTER 1
#define NIA_TTF_HMTX_POINTER 2
#define NIA_TTF_VHEA_POINTER 3
#define NIA_TTF_VMTX_POINTER 4
#define NIA_TTF_GLYF_POINTER 5
#define NIA_TTF_LOCA_POINTER 6
#define NIA_TTF_MAXP_POINTER 7
#define NIA_TTF_CMAP_POINTER 8

#define NIA_TTF_ESSENTIAL_HEADERS_AMOUNT 9
#include "nia_texture_gl.h"

NIA_CLASS niaTTFParser {
private:
    u32 numberOfGlyphs;
    u32 maxWidth;
    u32 maxHeight;
    u32 fileSize;

    i16 ascent;
    i16 descent;

    u8* initialPointer;
    u8* data;
    u8* essentialHeadersPointers[NIA_TTF_ESSENTIAL_HEADERS_AMOUNT];

    u32* glyphOffsets;

    // glyph buffer
    u32 glyphBufferCharacterLUTSize;
    u32 glyphBufferIndexAddressingSize;
    u32 glyphBufferIndexAddressingOffset;
    u32 glyphBufferBufferHeaderSize;

    u8* glyphBuffer;
    //

public:
    niaTexture texture;

    NIA_CALL niaTTFParser();
    NIA_CALL niaTTFParser(const char* filename);
    NIA_CALL ~niaTTFParser();

    int NIA_CALL loadFile(const char* filename);
    int NIA_CALL readTableDirectory();
    int NIA_CALL readEssentialHeaders();
    int NIA_CALL readHorizontalHeader();
    int NIA_CALL generateGlyphBitmap(u8* bitmap, u8* glyphPointer, const niaTTFGlyphHeader& glyphHeader, u32 width, u32 height, r32 scale);
    int NIA_CALL generateTextureAtlas();
    int NIA_CALL readGlyphHeaders();
    int NIA_CALL readGlyphMetrics();
    int NIA_CALL mapCharactersToIndexes();

    inline niaGlyph* getGlyph(u16 index){
        if(index >= numberOfGlyphs){
            return (niaGlyph*)(glyphBuffer + *(glyphBuffer + glyphBufferIndexAddressingOffset + 0)); // just return the first glyph for an out of bounds index
        }

        return (niaGlyph*)((u8*)(glyphBuffer + 
            (((*(glyphBuffer + glyphBufferIndexAddressingOffset + index * 4 + 0)) << 24) |
            ((*(glyphBuffer + glyphBufferIndexAddressingOffset + index * 4 + 1)) << 16) |
            ((*(glyphBuffer + glyphBufferIndexAddressingOffset + index * 4 + 2)) << 8) |
            (*(glyphBuffer + glyphBufferIndexAddressingOffset + index * 4 + 3))))
        );
    }

    inline niaGlyph* getGlyphChar(const char c){
        return getGlyph(glyphBuffer[c]);
    }
};

#endif // _NIA_TTF_PARSER_H_