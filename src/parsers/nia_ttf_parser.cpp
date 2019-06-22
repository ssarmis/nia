/*
 * File: nia_ttf_parser.cpp
 * Project: Nia
 * File created on April-29-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "parsers/nia_ttf_parser.h"

#include "nia_array.h"
#include "nia_rectangle.h"

#define SWAP32(_n) ((((_n) & 0x000000FF) << 24) | (((_n) & 0x0000FF00) << 8) | (((_n) & 0x00FF0000) >> 8) | (((_n) & 0xFF000000) >> 24))
#define SWAP16(_n) ((((_n) & 0xFF00) >> 8) | (((_n) & 0x00FF) << 8))

// for FourCC
#define COMPARE_TAG(_tag, _name) (((_tag[0]) == (_name[0])) && ((_tag[1]) == (_name[1])) && ((_tag[2]) == (_name[2])) && ((_tag[3]) == (_name[3])))

// for .16 fixed point numbers
#define FIXED(_n) ((_n) / 65536.0)

#define CONSUME(_type, _stream, _dest) {\
    _dest = *(_type*)_stream;\
    _stream += sizeof(_type);\
}

#define WASTE(_buffer, _amount) {\
    _buffer += _amount;\
}


niaTTFParser::niaTTFParser():fileSize(0), initialPointer(NULL), data(NULL){}

niaTTFParser::niaTTFParser(const char* filename, u16 fontSize):fontSize(fontSize){
    if(filename){
        if(loadFile(filename)){
            NIA_ERROR("Could not read %s, aborting font.\n", filename);
            return;
        }
        // TODO make the font parser actually work for more fonts
        if(fileSize){
            readTableDirectory();
            readEssentialHeaders();
            readGlyphHeaders();
            readGlyphMetrics();
            // generateTextureAtlas();
            mapCharactersToIndexes();
        } else {
            NIA_ERROR("Cannot decode empty font file %s, aborting.", filename);
            return;
        }
    }
}

niaTTFParser::~niaTTFParser(){
    delete[] initialPointer;
    delete[] glyphOffsets;
    delete[] glyphBuffer;
}

int niaTTFParser::loadFile(const char* filename){
    FILE* file = fopen(filename, "rb");

    if(!file){
        return 1;
    }

    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    data = new u8[fileSize];
    initialPointer = data;

    fread(data, sizeof(u8), fileSize, file);

    fclose(file);

    return 0;
}

int niaTTFParser::readTableDirectory(){
    niaTTFDirectoryFolder folders;
    CONSUME(niaTTFDirectoryFolder, data, folders);
    
    for(int i = 0; i < NIA_TTF_ESSENTIAL_HEADERS_AMOUNT; ++i){
        essentialHeadersPointers[i] = NULL;
    }

    u32 entries = SWAP16(folders.numTables);
    do {
        niaTTFHeader directory;
        CONSUME(niaTTFHeader, data, directory);

        u32 offset = SWAP32(directory.offset);
        u32 length = SWAP32(directory.length);
 
        if(COMPARE_TAG(directory.tag, "head")){
            essentialHeadersPointers[NIA_TTF_HEAD_POINTER] = (initialPointer + offset);
        } else if(COMPARE_TAG(directory.tag, "hhea")){
            essentialHeadersPointers[NIA_TTF_HHEA_POINTER] = (initialPointer + offset);
        } else if(COMPARE_TAG(directory.tag, "hmtx")){
            essentialHeadersPointers[NIA_TTF_HMTX_POINTER] = (initialPointer + offset);
        } else if(COMPARE_TAG(directory.tag, "vhea")){
            essentialHeadersPointers[NIA_TTF_VHEA_POINTER] = (initialPointer + offset);
        } else if(COMPARE_TAG(directory.tag, "vmtx")){
            essentialHeadersPointers[NIA_TTF_VMTX_POINTER] = (initialPointer + offset);
        } else if(COMPARE_TAG(directory.tag, "glyf")){
            essentialHeadersPointers[NIA_TTF_GLYF_POINTER] = (initialPointer + offset);
        } else if(COMPARE_TAG(directory.tag, "loca")){
            essentialHeadersPointers[NIA_TTF_LOCA_POINTER] = (initialPointer + offset);
        } else if(COMPARE_TAG(directory.tag, "maxp")){
            essentialHeadersPointers[NIA_TTF_MAXP_POINTER] = (initialPointer + offset);
        } else if(COMPARE_TAG(directory.tag, "cmap")){
            essentialHeadersPointers[NIA_TTF_CMAP_POINTER] = (initialPointer + offset);
        }
    } while(entries--);
	return 0;
}

int niaTTFParser::readEssentialHeaders(){
    niaTTFHeadHeader headerHeader;
    CONSUME(niaTTFHeadHeader, essentialHeadersPointers[NIA_TTF_HEAD_POINTER], headerHeader);

    unitsPerEm = SWAP16(headerHeader.unitsPerEm);

    niaTTFMaxPHeader maxPHeader;
    CONSUME(niaTTFMaxPHeader, essentialHeadersPointers[NIA_TTF_MAXP_POINTER], maxPHeader);

    numberOfGlyphs = SWAP16(maxPHeader.numGlyphs);

    maxWidth = SWAP16(headerHeader.xMax) - SWAP16(headerHeader.xMin);
    maxHeight = SWAP16(headerHeader.yMax) - SWAP16(headerHeader.yMin);

    glyphOffsets = new u32[numberOfGlyphs];

    for(int i = 0; i < numberOfGlyphs; ++i){
        u32 glyphOffset;
        if(SWAP16(headerHeader.indexToLocFormat)){
            CONSUME(u32, essentialHeadersPointers[NIA_TTF_LOCA_POINTER], glyphOffset);
            glyphOffset = SWAP32(glyphOffset);
        } else {
            CONSUME(u16, essentialHeadersPointers[NIA_TTF_LOCA_POINTER], glyphOffset);
            glyphOffset = SWAP16(glyphOffset) << 1;
        }
        glyphOffsets[i] = glyphOffset;
    }

    readHorizontalHeader();

    return 0;
}

int niaTTFParser::readHorizontalHeader(){
    u8* clone = essentialHeadersPointers[NIA_TTF_HHEA_POINTER];
    niaTTFHHeaHeader hheaHeader;
    CONSUME(niaTTFHHeaHeader, clone, hheaHeader);

    ascent = SWAP16(hheaHeader.ascent);
    descent = SWAP16(hheaHeader.descent);
    return 0;
}

int niaTTFParser::readGlyphHeaders(){
    // scale = (r32)(fontSize) / (r32)(ascent - descent);
    // TODO add font scaling based on desired size in height
    u32* width = new u32[numberOfGlyphs];
    u32* height = new u32[numberOfGlyphs];
    glyphsScale = new r32[numberOfGlyphs];


    glyphBufferCharacterLUTSize = 256;
    glyphBufferIndexAddressingSize = numberOfGlyphs * sizeof(u32);
    glyphBufferIndexAddressingOffset = glyphBufferCharacterLUTSize;
    glyphBufferBufferHeaderSize = glyphBufferCharacterLUTSize + glyphBufferIndexAddressingSize;

    // preallocate whole buffer
    // I will peek needed parameters from glyph headers, not the best method, but it will do the job
    u32 bufferSize = glyphBufferBufferHeaderSize;
    for(int i = 0; i < numberOfGlyphs; ++i){
        niaTTFGlyphHeader glyphHeader;

        u8* glyphPointer = (essentialHeadersPointers[NIA_TTF_GLYF_POINTER] + glyphOffsets[i]);
        CONSUME(niaTTFGlyphHeader, glyphPointer, glyphHeader);

        width[i] = ((((i16)SWAP16(glyphHeader.xMax) - (i16)SWAP16(glyphHeader.xMin)) + 1));
        height[i] = ((((i16)SWAP16(glyphHeader.yMax) - (i16)SWAP16(glyphHeader.yMin)) + 1));

        // glyphsScale[i] = ((r32)fontSize) / ((r32)height[i]);
        glyphsScale[i] = ((r32)fontSize) / ((r32)(ascent - descent));

        width[i] *= glyphsScale[i];
        height[i] *= glyphsScale[i];

        bufferSize += sizeof(niaGlyph);

        while(width[i] % 4){
            ++width[i];
        }

        while(height[i] % 4){
            ++height[i];
        }

        bufferSize += width[i] * height[i];
    }

    glyphBuffer = new u8[bufferSize];

    u32 offset = glyphBufferBufferHeaderSize; // skip the look up table and index addressing
    niaRectangle rect;
    for(int i = 0; i < numberOfGlyphs; ++i){
        niaTTFGlyphHeader glyphHeader;

        u8* glyphPointer = (essentialHeadersPointers[NIA_TTF_GLYF_POINTER] + glyphOffsets[i]);
        CONSUME(niaTTFGlyphHeader, glyphPointer, glyphHeader);

        niaGlyph* glyph = ((niaGlyph*)(glyphBuffer + offset));

        *(glyphBuffer + glyphBufferIndexAddressingOffset + i * 4 + 0) = (offset & 0xFF000000) >> 24;
        *(glyphBuffer + glyphBufferIndexAddressingOffset + i * 4 + 1) = (offset & 0x00FF0000) >> 16;
        *(glyphBuffer + glyphBufferIndexAddressingOffset + i * 4 + 2) = (offset & 0x0000FF00) >> 8;
        *(glyphBuffer + glyphBufferIndexAddressingOffset + i * 4 + 3) = offset & 0x000000FF;

        glyph->bounds.xmax = (i16)SWAP16(glyphHeader.xMax) * glyphsScale[i];
        glyph->bounds.xmin = (i16)SWAP16(glyphHeader.xMin) * glyphsScale[i];
        glyph->bounds.ymax = (i16)SWAP16(glyphHeader.yMax) * glyphsScale[i];
        glyph->bounds.ymin = (i16)SWAP16(glyphHeader.yMin) * glyphsScale[i];
        glyph->metrics.cursorAdvance = glyph->bounds.xmax;
        glyph->metrics.verticalAdvance = (glyph->bounds.ymin < 0 ? (glyph->bounds.ymax - glyph->bounds.ymin) : 0);

        offset += sizeof(niaGlyph);

        glyph->bitmap = (glyphBuffer + offset);
        offset += width[i] * height[i];

        generateGlyphBitmap(glyph->bitmap, glyphPointer, glyphHeader, width[i], height[i], glyphsScale[i]);

        rect.x = 0;
        rect.y = 0;

        rect.w = width[i];
        rect.h = height[i];

        glyph->sprite = niaSprite(glyph->bitmap, width[i], height[i], NIA_TEXTURE_FORMAT_R8_RED_UBYTE, rect);
    }

    delete[] width;
    delete[] height;
	return 0;
}

NIA_INLINE r32 absf(r32 value){
    return value < 0 ? -value : value;
}

typedef struct edge {
    niaTTFPoint A;
    niaTTFPoint B;
    r32 slopeInverse;
    u16 ymin;
    u16 ymax;
    u16 xOfYMax;
} edge;

typedef struct edgeTable {
    u32 numberOfEdges;
    // TODO make this a list/vector not a hardcoded array
    edge edges[(1 << 10) << 3];
} edgeTable;

NIA_INLINE void initializeEdgeTable(edgeTable* table){
    table->numberOfEdges = 0;
}

NIA_INLINE edge createEdge(const niaTTFPoint& A, const niaTTFPoint& B, i16 xOffset, i16 yOffset){
    edge result;

    result.A.x = A.x - xOffset;
    result.A.y = A.y - yOffset;
    
    result.B.x = B.x - xOffset;
    result.B.y = B.y - yOffset;
    
    r32 dx = (A.x - B.x);
    r32 dy = (A.y - B.y);
    result.slopeInverse = (dy == 0) ? 0 : (dx / dy);

    if(result.A.y <= result.B.y){
        result.ymin = result.A.y;
        result.ymax = result.B.y;
        result.xOfYMax = result.B.x;
    } else {
        result.ymin = result.B.y;
        result.ymax = result.A.y;
        result.xOfYMax = result.A.x;
    }

    return result;
}

NIA_INLINE bool samePoint(const niaTTFPoint& A, const niaTTFPoint& B){
    return (A.x == B.x) && (A.y == B.y);
}

NIA_INLINE bool edgeInTable(edgeTable* table, const edge& e){
    // don't judge
    for(u32 i = 0; i < table->numberOfEdges; ++i){
        if(samePoint(table->edges[i].A, e.A) && samePoint(table->edges[i].B, e.B)){
            return true;
        }
    }
    return false;
}

NIA_INLINE void addEdgeToTable(edgeTable* table, const edge& e){
    if(!edgeInTable(table, e)){
        table->edges[table->numberOfEdges] = e;
        ++table->numberOfEdges;
    }
}

NIA_INLINE bool lineOnEdge(const edge& e, u32 line){ // line is just the Y of the scanline
    return line >= e.ymin && line <= e.ymax;
}

NIA_INLINE void sortArray(niaArray<u16>* array){
    for(u16 i = 1; i < array->getSize(); ++i){
        u16 value = array->getData()[i];
        while(value < array->getData()[i - 1] && i){
            u16 aux = array->getData()[i - 1];
            array->getData()[i - 1] = value;
            array->getData()[i] = aux;
            --i;
        }
    }
}

NIA_INLINE u16 valueInArray(const niaArray<u16>& array, u16 value){
    u16 result = 0;
    for(int i = 0; i < array.getSize(); ++i){
        if(array[i] == value){
            ++result;
        }
    }
    return result;
}

NIA_INLINE void scanlineFill(u8* bitmap, u32 width, u32 height, edgeTable* table){
    niaArray<u16> pointsOnScanLine;
    u32 pairAdvance = 0;

    for(u32 scanLine = 0; scanLine < height; ++scanLine){
        for(u32 edge = 0; edge < table->numberOfEdges; ++edge){
            if(table->edges[edge].A.y == table->edges[edge].B.y){
                continue;
            }
            
            if(lineOnEdge(table->edges[edge], scanLine)){
                u16 xOfIntersection = (table->edges[edge].ymax - scanLine) * (-table->edges[edge].slopeInverse) + table->edges[edge].xOfYMax;

                // true if this point is an edge endpoint
                if((xOfIntersection == table->edges[edge].A.x && scanLine == table->edges[edge].A.y) ||
                   (xOfIntersection == table->edges[edge].B.x && scanLine == table->edges[edge].B.y)){
                    if(scanLine != table->edges[edge].ymax){
                        continue;
                    }
                }

                pointsOnScanLine.add(xOfIntersection);
            }
        }

        sortArray(&pointsOnScanLine);

        if(pointsOnScanLine.getSize() > 1){
            if((pointsOnScanLine.getSize() & 1)){
                pairAdvance = 1;
            } else {
                pairAdvance = 2;
            }

            for(u16 i = 0; i < pointsOnScanLine.getSize() - 1; i += pairAdvance){
                for(u16 x = pointsOnScanLine[i]; x < pointsOnScanLine[i + 1]; ++x){
                    bitmap[x + scanLine * width] = 0xff;
                }
            }
        }
        pointsOnScanLine.clean();
    }
}

NIA_INLINE void drawLine(u8* bitmap, const niaTTFPoint& A, const niaTTFPoint& B, i16 xOffset, i16 yOffset, u32 width, u32 height){
    r32 deltaX = B.x - A.x;
    r32 deltaY = B.y - A.y;

    r32 steps;

    if(absf(deltaX) >= absf(deltaY)){
        steps = absf(deltaX);
    } else {
        steps = absf(deltaY);
    }

    r32 xIncrement = deltaX / steps;
    r32 yIncrement = deltaY / steps;

    r32 xr = A.x;
    r32 yr = A.y;

    while(steps--) {
        int x = (int)(xr + 0.5);
        int y = (int)(yr + 0.5);

        bitmap[(x - xOffset) + (y - yOffset) * width] = 0xff;

        xr = xr + xIncrement;
        yr = yr + yIncrement;
    }
}

NIA_INLINE u16 r32u16(r32 number){
    if((u16)(number + 0.5) == (u16)number){
        return (u16)number;
    }
    return (u16)(number + 0.5);
}

NIA_INLINE void computePointOnEdge(){
}

NIA_INLINE void createAndAddBezierEdges(const niaTTFPoint& p0, const niaTTFPoint& p1, const niaTTFPoint& p2,
                                        i16 xOffset, i16 yOffset, edgeTable* table){

    // TODO make this function be able to take any number of control points
    //      for now this number is decent.

    edge A = createEdge(p0, p1, xOffset, yOffset);
    edge B = createEdge(p1, p2, xOffset, yOffset);

    edge Q;

    niaTTFPoint q0;
    niaTTFPoint q1;
    niaTTFPoint interest;
    niaTTFPoint previous;

    // TODO make something so this won't be ugly anymore...
    previous.x = p0.x - xOffset;
    previous.y = p0.y - yOffset;

    for(r32 iterator = 0; iterator < 1; iterator += 0.01){
        q0.x = r32u16((1 - iterator) * A.A.x + iterator * A.B.x);
        q0.y = r32u16((1 - iterator) * A.A.y + iterator * A.B.y);

        q1.x = r32u16((1 - iterator) * B.A.x + iterator * B.B.x);
        q1.y = r32u16((1 - iterator) * B.A.y + iterator * B.B.y);

        interest.x = r32u16((1 - iterator) * q0.x + iterator * q1.x);
        interest.y = r32u16((1 - iterator) * q0.y + iterator * q1.y);

        if(samePoint(previous, interest)){
            continue;
        }

        Q = createEdge(previous, interest, 0, 0); // no offset because it was applied before, so our coordinates are already offseted
        addEdgeToTable(table, Q);
        previous = interest;
    }
}

int niaTTFParser::generateGlyphBitmap(u8* bitmap, u8* glyphPointer, const niaTTFGlyphHeader& glyphHeader, u32 width, u32 height, r32 scale){
    i16 numberOfContours = SWAP16(glyphHeader.numberOfContours);

    if(numberOfContours < 0){
        // compound glyph here, take another route
        return 0;
    }

    u16* endPtsOfContours = new u16[numberOfContours];

    for(int i = 0; i < numberOfContours; ++i){
        u16 index;
        CONSUME(u16, glyphPointer, index);
        endPtsOfContours[i] = SWAP16(index);
    }

    u16 instructionLength;
    CONSUME(u16, glyphPointer, instructionLength);

    // jump over instructions
    WASTE(glyphPointer, SWAP16(instructionLength));

    u32 numberOfPoints = 0;

    for(int i = 0; i < numberOfContours; ++i){
        if(numberOfPoints < endPtsOfContours[i]){
            numberOfPoints = endPtsOfContours[i];
        }
    }
    ++numberOfPoints;

    niaTTFPoint* points = new niaTTFPoint[numberOfPoints];

    niaArray<u8> flags;

    int offset = 0;
    for(int i = 0; i < numberOfPoints; ++i){
        u8 flag;
        CONSUME(u8, glyphPointer, flag);
        
        flags.add(flag);
        points[offset++].onCurve = flag & NIA_POINT_ON_CURVE;
        if(flag & NIA_FLAG_REPEAT){
            u8 repeatAmount;
            CONSUME(u8, glyphPointer, repeatAmount);

            i += repeatAmount;
            while(repeatAmount--){
                flags.add(flag);
                points[offset++].onCurve = flag & NIA_POINT_ON_CURVE;
            }
        }
    }

    // xs
    i16 finalValueX = 0;
    for(int i = 0; i < numberOfPoints; ++i){
        i16 value;
        if (flags[i] & NIA_X_SHORT){
            CONSUME(u8, glyphPointer, value);
            if(!(flags[i] & NIA_X_SAME)){
                value *= -1;
            }
        } else {
            if((flags[i] & NIA_X_SAME)){
                if(i - 1 >= 0){
                    points[i].x = points[i - 1].x;
                } else {
                    points[i].x = 0;
                }

                continue;
            }

            CONSUME(i16, glyphPointer, value);
            value = SWAP16(value);
        }

        finalValueX += value;

        points[i].x = finalValueX * scale;
    }

    // ys
    i16 finalValueY = 0;
    for(int i = 0; i < numberOfPoints; ++i){
        i16 value;
        if (flags[i] & NIA_Y_SHORT){
            CONSUME(u8, glyphPointer, value);
            if(!(flags[i] & NIA_Y_SAME)){
                value *= -1;
            }
        } else {
            if((flags[i] & NIA_Y_SAME)){
                if(i - 1 >= 0){
                    points[i].y = points[i - 1].y;
                } else {
                    points[i].y = 0;
                }

                continue;
            }

            CONSUME(i16, glyphPointer, value);
            value = SWAP16(value);
        }

        finalValueY += value;

        points[i].y = finalValueY * scale;
    }


    niaTTFPoint previous;

    bool sectionStarted;
    niaTTFPoint firstSectionPoint;

    bool firstPassed = false;

    i16 xOffset = ((i16)(SWAP16(glyphHeader.xMin))) * scale;
    i16 yOffset = ((i16)(SWAP16(glyphHeader.yMin))) * scale;

    int index = 0;

    edgeTable table;

    initializeEdgeTable(&table);

    // filling the edge table first then drawing everything
    // lines, filling the glyph, so we can make the curves and then 
    // make everything in terms of edges
    bool processedBezierPoint;
    // TODO refactor this thing, it looks hideous, its not slow, but it looks ugly, it was rushed
    for(int i = 0; i < numberOfPoints; ++i){
        niaTTFPoint newPoint = points[i];

        if(firstPassed){
            if(!newPoint.onCurve && !processedBezierPoint){
                processedBezierPoint = true;
                niaTTFPoint nextPoint;
                if(i + 1 <= numberOfPoints - 1){
                    nextPoint = points[i + 1];

                    if(i + 1 > endPtsOfContours[index]){
                        nextPoint = firstSectionPoint;
                        createAndAddBezierEdges(previous, newPoint, nextPoint, xOffset, yOffset, &table);

                        ++index;
                        firstPassed = false;
                        processedBezierPoint = false;
                        continue;
                    }

                    createAndAddBezierEdges(previous, newPoint, nextPoint, xOffset, yOffset, &table); // peek next point and also add him 
                } else { // treat the last point in shape as being a normal point no matter what...
                    if(i + 1 > endPtsOfContours[index]){
                        edge e = createEdge(newPoint, firstSectionPoint, xOffset, yOffset);
                        if(!samePoint(e.A, e.B)){
                            addEdgeToTable(&table, e);
                        }

                        ++index;
                        firstPassed = false;
                        processedBezierPoint = false;
                        continue;
                    }

                    edge e = createEdge(previous, newPoint, xOffset, yOffset);
                    if(!samePoint(e.A, e.B)){
                        addEdgeToTable(&table, e);
                    }
                }
            } else {
                if(!processedBezierPoint){
                    edge e = createEdge(previous, newPoint, xOffset, yOffset);
                    if(!samePoint(e.A, e.B)){ // we don't want edges that have the same point as both points...
                        addEdgeToTable(&table, e);
                    }
                } else {
                    processedBezierPoint = false;
                }
            }

        } else {
            firstSectionPoint = newPoint;
            firstPassed = !firstPassed;
        }
        
        // here we hit the end of the shape(contour) so we can connect the last point of the shape with the first
        if(endPtsOfContours[index] == i){
            edge e = createEdge(newPoint, firstSectionPoint, xOffset, yOffset);

            if(!samePoint(e.A, e.B)){
                addEdgeToTable(&table, e);
            }

            ++index;
            firstPassed = false;
            processedBezierPoint = false;
            continue;
        }

        previous = points[i];
    }

    // for(int i = 0; i < table.numberOfEdges; ++i){
    //     drawLine(bitmap, table.edges[i].A, table.edges[i].B, 0, 0, width, height);
    // }

    scanlineFill(bitmap, width, height, &table);

    // clean the mess
    flags.clean();
    delete[] points;
    delete[] endPtsOfContours;
}

int niaTTFParser::readGlyphMetrics(){
    u32 index;
    // fill in details for bearings and advances
    if(essentialHeadersPointers[NIA_TTF_HHEA_POINTER]){
        u8* source6 = essentialHeadersPointers[NIA_TTF_HHEA_POINTER];
        niaTTFHHeaHeader hheaHeader;
        CONSUME(niaTTFHHeaHeader, source6, hheaHeader);

        u16 numberOfLongHorMetrics = SWAP16(hheaHeader.numOfLongHorMetrics);

        u8* source7 = essentialHeadersPointers[NIA_TTF_HMTX_POINTER];
        index = 0;
        while(numberOfLongHorMetrics--){
            niaLongHorMetric metric;
            CONSUME(niaLongHorMetric, source7, metric);

            getGlyph(index)->metrics.cursorAdvance = SWAP16(metric.advanceWidth) * getGlyphScale(index);
            ++index;
        }
    }

    if(essentialHeadersPointers[NIA_TTF_VHEA_POINTER]){
        u8* source8 = essentialHeadersPointers[NIA_TTF_VHEA_POINTER];
        niaTTFVHeaHeader vheaHeader;
        CONSUME(niaTTFVHeaHeader, source8, vheaHeader);

        u16 numberOfLongVerMetrics = SWAP16(vheaHeader.numOfLongVerMetrics);

        u8* source9 = essentialHeadersPointers[NIA_TTF_VMTX_POINTER];
        index = 0;
        while(numberOfLongVerMetrics--){
            niaLongVerMetric metric;
            CONSUME(niaLongVerMetric, source9, metric);

            getGlyph(index++)->metrics.verticalAdvance = SWAP16(metric.advanceHeight) * getGlyphScale(index);
            ++index;
        }
    }
	return 0;
}

int niaTTFParser::mapCharactersToIndexes(){
    u8* source5 = essentialHeadersPointers[NIA_TTF_CMAP_POINTER];
    niaTTFCmapHeader cmapHeader;
    CONSUME(niaTTFCmapHeader, source5, cmapHeader);

    u16 numberOfSubtableEntries = SWAP16(cmapHeader.numberSubtables);
    bool parsedFormat = false;
    // map character to index
    while(numberOfSubtableEntries--){
        niaTTFCmapSubtable cmapSubtable;
        CONSUME(niaTTFCmapSubtable, source5, cmapSubtable);

        // printf("platform id %d \n", SWAP16(cmapSubtable.platformID));
        // printf("platform specific id %d \n", SWAP16(cmapSubtable.platformSpecificID));
        // printf("offset %ul\n", SWAP32(cmapSubtable.offset));

        u8* subtable = (essentialHeadersPointers[NIA_TTF_CMAP_POINTER] + SWAP32(cmapSubtable.offset));
        niaTTFCmapSubtableGeneral generalData;
        CONSUME(niaTTFCmapSubtableGeneral, subtable, generalData);

        // printf("\tformat %hu\n", SWAP16(generalData.format));
        // printf("\tlength %hu\n", SWAP16(generalData.length));
        // printf("\tlanguage %hu\n", SWAP16(generalData.language));

        if(!parsedFormat){
            switch(SWAP16(generalData.format)){
                case NIA_FORMAT_0: {
                            niaTTFCmapSubtableFormat0 format0Subtable;
                            CONSUME(niaTTFCmapSubtableFormat0, subtable, format0Subtable);

                            for(int i = 0; i < 256; ++i){
                                // glyphCodeToIndex.add(format0Subtable.glyphIndexArray[i]);
                                glyphBuffer[i] = format0Subtable.glyphIndexArray[i];
                            }

                            parsedFormat = true;
                        };
                        break;

                case NIA_FORMAT_6: {
                            // this will only work well, with basic ASCII characters, will need some work for unicodes
                            niaTTFCmapSubtableFormat6 format6Subtable;
                            CONSUME(niaTTFCmapSubtableFormat6, subtable, format6Subtable);

                            u16 firstCode = SWAP16(format6Subtable.firstCode);
                            u16 entryCount = SWAP16(format6Subtable.entryCount);

                            niaArray<u16> glyphIndexArray; // not fast but will do I guess

                            while(entryCount--){
                                u16 code;
                                CONSUME(u16, subtable, code);
                                glyphIndexArray.add(SWAP16(code));
                            }

                            // index for 'a' is glyphIndexArray['a' - firstCode]

                            for(int i = firstCode; i < glyphIndexArray.getSize(); ++i){
                                glyphBuffer[i - firstCode] = glyphIndexArray[i - firstCode];
                            }

                            parsedFormat = true;
                        };
                        break;
                
                    // TODO add support for format 4
                    default: {
                            NIA_TRACE("Unsupported font format\n");
                        };
                        break;
            }
        }
    }
	return 0;
}
