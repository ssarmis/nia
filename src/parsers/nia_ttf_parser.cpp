#include "parsers/nia_ttf_parser.h"

#include "nia_array.h"

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


NIA_CALL niaTTFParser::niaTTFParser():fileSize(0), initialPointer(NULL), data(NULL){}

NIA_CALL niaTTFParser::niaTTFParser(const char* filename){
    if(filename){
        if(loadFile(filename)){
            NIA_ERROR("Could not read %s, aborting font.\n", filename);
            return;
        }

        if(fileSize){
            readTableDirectory();
            readEssentialHeaders();
            readGlyphHeaders();
            readGlyphMetrics();
            generateTextureAtlas();
            mapCharactersToIndexes();
        } else {
            NIA_ERROR("Cannot decode empty font file %s, aborting.", filename);
            return;
        }
    }
}

NIA_CALL niaTTFParser::~niaTTFParser(){
    delete [] initialPointer;
    delete [] glyphOffsets;
    delete [] glyphBuffer;
}

NIA_CALL int niaTTFParser::loadFile(const char* filename){
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

NIA_CALL int niaTTFParser::readTableDirectory(){
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
}

NIA_CALL int niaTTFParser::readEssentialHeaders(){
    niaTTFHeadHeader headerHeader;
    CONSUME(niaTTFHeadHeader, essentialHeadersPointers[NIA_TTF_HEAD_POINTER], headerHeader);

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

NIA_CALL int niaTTFParser::readHorizontalHeader(){
    u8* clone = essentialHeadersPointers[NIA_TTF_HHEA_POINTER];
    niaTTFHHeaHeader hheaHeader;
    CONSUME(niaTTFHHeaHeader, clone, hheaHeader);

    ascent = SWAP16(hheaHeader.ascent);
    descent = SWAP16(hheaHeader.descent);
    return 0;
}

r32 scale = 1;
NIA_CALL int niaTTFParser::readGlyphHeaders(){
    // scale = 64.0 / (r32)(ascent - descent);
    u32* width = new u32[numberOfGlyphs];
    u32* height = new u32[numberOfGlyphs];

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

        width[i] = ((((i16)SWAP16(glyphHeader.xMax) - (i16)SWAP16(glyphHeader.xMin)) + 1) * scale);
        height[i] = ((((i16)SWAP16(glyphHeader.yMax) - (i16)SWAP16(glyphHeader.yMin)) + 1) * scale);

        bufferSize += sizeof(niaGlyph);
        bufferSize += width[i] * height[i];
    }

    glyphBuffer = new u8[bufferSize];

    u32 offset = glyphBufferBufferHeaderSize; // skip the look up table and index addressing
    for(int i = 0; i < numberOfGlyphs; ++i){
        niaTTFGlyphHeader glyphHeader;

        u8* glyphPointer = (essentialHeadersPointers[NIA_TTF_GLYF_POINTER] + glyphOffsets[i]);
        CONSUME(niaTTFGlyphHeader, glyphPointer, glyphHeader);

        niaGlyph* glyph = ((niaGlyph*)(glyphBuffer + offset));

        *(glyphBuffer + glyphBufferIndexAddressingOffset + i * 4 + 0) = (offset & 0xFF000000) >> 24;
        *(glyphBuffer + glyphBufferIndexAddressingOffset + i * 4 + 1) = (offset & 0x00FF0000) >> 16;
        *(glyphBuffer + glyphBufferIndexAddressingOffset + i * 4 + 2) = (offset & 0x0000FF00) >> 8;
        *(glyphBuffer + glyphBufferIndexAddressingOffset + i * 4 + 3) = offset & 0x000000FF;

        glyph->bounds.xmax = (i16)SWAP16(glyphHeader.xMax) * scale;
        glyph->bounds.xmin = (i16)SWAP16(glyphHeader.xMin) * scale;
        glyph->bounds.ymax = (i16)SWAP16(glyphHeader.yMax) * scale;
        glyph->bounds.ymin = (i16)SWAP16(glyphHeader.yMin) * scale;
        glyph->metrics.cursorAdvance = glyph->bounds.xmax;
        glyph->metrics.verticalAdvance = (glyph->bounds.ymin < 0 ? (glyph->bounds.ymax + glyph->bounds.ymin) : 0);

        offset += sizeof(niaGlyph);

        glyph->bitmap = (glyphBuffer + offset);
        offset += width[i] * height[i];
        generateGlyphBitmap(glyph->bitmap, glyphPointer, glyphHeader, width[i], height[i], scale);
    }

    delete [] width;
    delete [] height;
}

r32 absf(r32 value){
    return value < 0 ? -value : value;
}

NIA_CALL void scanAndFill(u8* bitmap, u32 width, u32 height){
    u32 scanLine = 0;

    while(scanLine < height){
        niaArray<i32> edges;
        niaArray<niaVec2i> pairs;

        int old = 0;

        for(int x = 0; x < width; ++x){
            if(bitmap[x + scanLine * width] != 0){
                int offset = 0;
                while(bitmap[x + offset++ + scanLine * width]){
                    x++;
                }
                edges.add(x);
            }
        }

        if(edges.getSize() > 0){
            for(int i = 0; i < edges.getSize() - 1; ++i){
                pairs.add(niaVec2i{edges[i], edges[i + 1]});
                ++i;
                if(i == edges.getSize() - 1){
                    if(edges.getSize() % 2 != 0){
                        pairs.add(niaVec2i{edges[i - 1], edges[i]});
                    }
                }
            }
        }

        for(int i = 0; i < pairs.getSize(); ++i){
            for(int x = pairs[i].x; x < pairs[i].y; ++x){
                bitmap[x + scanLine * width] = 0xff;
            }
        }
        ++scanLine;
    }
}

void drawLine(u8* bitmap, const niaTTFPoint& A, const niaTTFPoint& B, i16 xOffset, i16 yOffset, u32 width, u32 height){
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

NIA_CALL int niaTTFParser::generateGlyphBitmap(u8* bitmap, u8* glyphPointer, const niaTTFGlyphHeader& glyphHeader, u32 width, u32 height, r32 scale){
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

    for(int i = 0; i < numberOfPoints; ++i){
        niaTTFPoint newPoint = points[i];

        if(firstPassed){
            drawLine(bitmap, previous, newPoint, xOffset, yOffset, width, height);
        } else {
            firstSectionPoint = newPoint;
            firstPassed = !firstPassed;
        }
        
        // here we hit the end of the shape(contour) so we can connect the last point of the shape with the first
        if(endPtsOfContours[index] == i){
            drawLine(bitmap, newPoint, firstSectionPoint, xOffset, yOffset, width, height);
            ++index;
            firstPassed = false;
            continue;
        }

        previous = points[i];
    }

    scanAndFill(bitmap, width, height);

    // clean the mess
    flags.clean();
    delete [] points;
    delete [] endPtsOfContours;
}

NIA_CALL int niaTTFParser::generateTextureAtlas(){
    u32 entries = numberOfGlyphs;
    u32 offset = 0;

    u32 height = 0;
    u32 width = 0;

    u32 maximumWidth = 0;

    while(entries){

        u32 rowHeight = 0;
        u32 rowWidth = 0;

        i32 i = 0;
        for(; i < 10; ++i){
            if(offset >= numberOfGlyphs){ 
                break;
            }
            niaGlyph glyph = *getGlyph(offset++);

            u32 gwidth = glyph.bounds.xmax - glyph.bounds.xmin + 1;
            u32 gheight = glyph.bounds.ymax - glyph.bounds.ymin + 1;

            if(gheight > rowHeight){
                rowHeight = gheight;
                height += rowHeight;
            }

            rowWidth += gwidth;

            if(rowWidth > maximumWidth){
                maximumWidth = rowWidth;
                width = rowWidth;
            }
        }

        entries -= i;
    }
    if(width % 4){
        width += width % 4;
    }

    if(height % 4){
        height += height % 4;
    }

    u8* pixels = new u8[width * height];

    i32 horizontalOffset = 0;
    i32 verticalOffset = 0;

    i32 maximumHeight = 0;

    for(i32 i = 0; i < numberOfGlyphs; ++i){
        niaGlyph* glyph = getGlyph(i);

        u32 gwidth = glyph->bounds.xmax - glyph->bounds.xmin + 1;
        u32 gheight = glyph->bounds.ymax - glyph->bounds.ymin + 1;

        glyph->uvs.u0 = (r32)horizontalOffset / (r32)width;
        glyph->uvs.u1 = (r32)(horizontalOffset + glyph->bounds.xmax - glyph->bounds.xmin) / (r32)width;
        
        glyph->uvs.v0 = (r32)verticalOffset / (r32)height;
        glyph->uvs.v1 = (r32)(verticalOffset + glyph->bounds.ymax - glyph->bounds.ymin) / (r32)height;
        
        for(i32 y = 0; y < gheight; ++y){
            for(i32 x = 0; x < gwidth; ++x){
                i32 xx = horizontalOffset + x;
                i32 yy = verticalOffset + y;

                if(xx < 0 || xx >= width || yy < 0 || yy >= height){
                    continue;
                }

                pixels[xx + yy * width] = glyph->bitmap[x + y * gwidth];
            }
        }

        if(gheight > maximumHeight){
            maximumHeight = gheight;
        }

        horizontalOffset += gwidth;

        if(i % 10 == 0 && i){
            verticalOffset += maximumHeight;
            horizontalOffset = 0;
            maximumHeight = 0;
        }
    }

    texture = niaTexture(pixels, width, height, NIA_TEXTURE_FORMAT_R8_RED_UBYTE);
}

NIA_CALL int niaTTFParser::readGlyphMetrics(){
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

            getGlyph(index++)->metrics.cursorAdvance = SWAP16(metric.advanceWidth);
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

            getGlyph(index++)->metrics.verticalAdvance = SWAP16(metric.advanceHeight);
        }
    }
}

NIA_CALL int niaTTFParser::mapCharactersToIndexes(){
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
                            printf("Unsupported font format\n");
                        };
                        break;
            }
        }
    }
}
