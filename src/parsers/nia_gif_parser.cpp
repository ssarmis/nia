#include "parsers/nia_gif_parser.h"


#define COMPARE_3BYTECODE(_x, _y) ((_x[0] == _y[0]) && (_x[1] == _y[1]) && (_x[2] == _y[2]))

#define WASTE(_stream, _amount) { \
    (_stream) += (_amount); \
}

#define CONSUME(_stream, _type, _dest) { \
    (_dest) = *((_type*)_stream); \
    WASTE(_stream, sizeof(_type)); \
}

#define SWAP16(_x) (((_x) >> 8) | (((_x) << 8) & 0xff00))

#define GET_NO_STREAM_BITS(_value, _position, _amount) (\
    ((_value) >> ((_position + 1) - _amount)) & ((1 << _amount) - 1)\
)

#define READ_BIT(_stream, _used)(  \
    (((_stream[_used >> 3]) >> ((_used & 7))) & 0x01) \
)

u32 POWER(u32 base, u32 exponent) {
    u32 result = 1;
    while(exponent--){
        result *= base;
    }
    return result;
}

NIA_CALL niaGifParser::niaGifParser(){}

NIA_CALL niaGifParser::niaGifParser(const char* filename){
    loadFile(filename);
    decodeFile();

    textureFormat = NIA_TEXTURE_FORMAT_RGBA_RGBA_UBYTE;
}

NIA_CALL niaGifParser::~niaGifParser(){}


u16 readBits(niaGIFImageDecodeStreamChunk*& chunk, u8 amount){
    u16 result = 0;
    for(int i = 0; i < amount; ++i){
        if((chunk->usedBits >> 3) >= chunk->dataSize){
            chunk = chunk->next;
        }

        if(!chunk){
            return result;
        }

        u16 bit = READ_BIT(chunk->data, chunk->usedBits);
        bit <<= i;
        result |= bit;
        ++chunk->usedBits;
    }
    return result;
}

niaGIFLZWEntry createEntry(u16 length){
    niaGIFLZWEntry result;
    result.length = length;
    result.values = new u16[length];

    for(int i = 0; i < length; ++i){
        result.values[i] = 0;
    }
    return result;
}

void cleanDictionary(niaGIFLZWDictionary* dictionary){
    for(int i = 0; i < 4097; ++i){
        delete[] dictionary->entries[i].values;        
    }
}

void initDictionary(niaGIFLZWDictionary* dictionary){
    dictionary->size = 0;
    for(int i = 0; i < 4097; ++i){
        dictionary->entries[i] = createEntry(0);
    }
}

void updateEntryInDictionary(niaGIFLZWDictionary* dictionary, u16 code, u8 index, u16 value){
    dictionary->entries[code].values[index] = value;
}

void addEntryToDictionary(niaGIFLZWDictionary* dictionary, niaGIFLZWEntry entry){
    dictionary->entries[dictionary->size++] = entry;
}

niaGIFLZWEntry getEntryFromDictionary(niaGIFLZWDictionary* dictionary, u16 code){
    return dictionary->entries[code];
}

niaGIFLZWEntry getMostRecentEntryFromDictionary(niaGIFLZWDictionary* dictionary){
    return dictionary->entries[dictionary->size - 1];
}


u16 getDictionarySize(niaGIFLZWDictionary* dictionary){
    return dictionary->size;
}

void resetDictionary(niaGIFLZWDictionary* dictionary, u16 clearCode){
    initDictionary(dictionary);
    niaGIFLZWEntry entry;
    for(int i = 0; i < clearCode; ++i){
        if(dictionary->entries[i].length > 0){ // so this was used before
            delete[] dictionary->entries[i].values; // clean the mess
        }
        entry = createEntry(1);
        entry.values[0] = i;
        addEntryToDictionary(dictionary, entry);
    }

    entry = createEntry(1);
    entry.values[0] = 0;
    addEntryToDictionary(dictionary, entry); // CC

    entry = createEntry(1);
    entry.values[0] = 0;
    addEntryToDictionary(dictionary, entry); // EOS
}

void cleanStream(niaGIFImageDecodeStream* stream){
    niaGIFImageDecodeStreamChunk* currentChunk = stream->head;
    niaGIFImageDecodeStreamChunk* tmp;
    while(currentChunk){
        tmp = currentChunk;
        currentChunk = currentChunk->next;
        delete[] tmp;
    }
}

void initializeGifImageDecodeStream(niaGIFImageDecodeStream* stream){ // very C like but does the job
    stream->head = NULL;
    stream->tail = NULL;
}

niaGIFImageDecodeStreamChunk newImageDecodeStreamChunk(u16 dataSize){
    niaGIFImageDecodeStreamChunk chunk;
    chunk.dataSize = dataSize;
    chunk.data = new u8[dataSize];
    chunk.next = NULL;
    chunk.usedBits = 0;
    return chunk;
}

void writeImageDecodeStreamChunk(niaGIFImageDecodeStreamChunk* chunk, u8* source){
    u16 copy = chunk->dataSize;
    while(copy--){
        *chunk->data++ = *source++;
    }
    chunk->data -= chunk->dataSize;
}

void addImageDecodeStreamChunkToStream(niaGIFImageDecodeStream* stream, niaGIFImageDecodeStreamChunk* chunk){
    if(stream->head){
        stream->tail->next = chunk;
        stream->tail = chunk;
    } else {
        stream->head = stream->tail = chunk;
    }
}

u8* cloneImage(u8* source, u32 width, u32 height){
    u32 amount = width * height * 4;
    u8* result = new u8[amount];
    while(amount--){
        *result++ = *source++;
    }
    return result - (width * height * 4);
}

void putColor(u8* colorTable, u8* image, u32* position, u16 colorOffset){
    image[(*position)++] = colorTable[colorOffset * 3 + 0];
    image[(*position)++] = colorTable[colorOffset * 3 + 1];
    image[(*position)++] = colorTable[colorOffset * 3 + 2];
    image[(*position)++] = 0xff;
}

NIA_CALL void niaGifParser::decodeImageData(u8* colorTable){
    ++numberOfFrames;
    niaGIFImageDescriptor imageDescriptor;
    CONSUME(data, niaGIFImageDescriptor, imageDescriptor);

    u8 isLocalColorTablePresent = GET_NO_STREAM_BITS(imageDescriptor.packedFields, 7, 1);
    u8 isImageInterlaced = GET_NO_STREAM_BITS(imageDescriptor.packedFields, 6, 1);
    u8 bitsPerPixel = GET_NO_STREAM_BITS(imageDescriptor.packedFields, 2, 3) + 1;

    if(isLocalColorTablePresent){
        u32 numberOfColors = 3 * POWER(2, bitsPerPixel);
        u8* localColorTable = new u8[numberOfColors * 3];
        u32 index = 0;
        while(numberOfColors--){
            CONSUME(data, u8, localColorTable[index++]);
        }
        colorTable = localColorTable;
    }

    if(isImageInterlaced){
        // TODO
        // do something for it..
    }

    u8 LZWMinimumCodeSize;
    CONSUME(data, u8, LZWMinimumCodeSize);

    u8 dataSize;
    CONSUME(data, u8, dataSize);

    u8* finalImage;
    u8* pixels;
    pixels = new u8[imageDescriptor.imageWidth * imageDescriptor.imageHeight * 4]; // our current image will be here
    if(numberOfFrames > 1){ // not our first frame
        frames[numberOfFrames - 1].width = frames[0].width;
        frames[numberOfFrames - 1].height = frames[0].height;
        // pixels = new u8[imageDescriptor.imageWidth * imageDescriptor.imageHeight * 4];
        switch(disposalMethod){
            case NIA_DISPOSAL_NO_ACTION: { // don't change anything in the image, I guess
                    finalImage = new u8[frames[0].width * frames[0].height * 4];
                }
                break;

            case NIA_DISPOSAL_LEFT_IN_PLACE: { // don't change anything in the image, I guess
                    finalImage = cloneImage( // clone the previous frame
                        frames[numberOfFrames - 2].data, 
                        frames[numberOfFrames - 2].width,
                        frames[numberOfFrames - 2].height);
                }
                break;

            case NIA_DISPOSAL_RESTORE_BACKGROUND: { // fill with background before frame
                    u32 amount = imageDescriptor.imageWidth * imageDescriptor.imageHeight * 4;
                    finalImage = new u8[amount];
                    while(amount){
                        pixels[amount - 0] = 0xff;
                        pixels[amount - 1] = colorTable[backgroundColorIndex * 3 + 2];
                        pixels[amount - 2] = colorTable[backgroundColorIndex * 3 + 1];
                        pixels[amount - 3] = colorTable[backgroundColorIndex * 3 + 0];
                        amount -= 4;
                    }
                }
                break;

            case NIA_DISPOSAL_RESTORE_PROVIOUS: { // fill with previous frame where it was overriten, this code is not doing it, will implement later
                    finalImage = cloneImage( 
                        frames[numberOfFrames - 2].data, 
                        frames[numberOfFrames - 2].width,
                        frames[numberOfFrames - 2].height);
                }
                break;

            default: { // I guess
                    finalImage = cloneImage( 
                        frames[numberOfFrames - 2].data, 
                        frames[numberOfFrames - 2].width,
                        frames[numberOfFrames - 2].height);
                }
                break;
        }
    } else {
        frames[0].width = imageDescriptor.imageWidth;
        frames[0].height = imageDescriptor.imageHeight;
        finalImage = new u8[imageDescriptor.imageWidth * imageDescriptor.imageHeight * 4];
    }
    frames[numberOfFrames - 1].data = finalImage;

    u32 position = 0;

    niaGIFLZWDictionary dictionary;
    u32 usedBits = 0;
        
    u8 codeSize = LZWMinimumCodeSize + 1;
    u16 clearCode = (1 << LZWMinimumCodeSize); // for min code size 2, clear code is 0x04
    u16 endCode = (1 << LZWMinimumCodeSize) + 1; // for min code size 2, clear code is 0x05
    
    u16 code;
    u16 oldCode;

    resetDictionary(&dictionary, clearCode);

    niaGIFImageDecodeStream stream;
    initializeGifImageDecodeStream(&stream);

    while(dataSize){
        niaGIFImageDecodeStreamChunk* chunk = new niaGIFImageDecodeStreamChunk;

        *chunk = newImageDecodeStreamChunk(dataSize);
        writeImageDecodeStreamChunk(chunk, data);
        addImageDecodeStreamChunkToStream(&stream, chunk);

        WASTE(data, chunk->dataSize); // jump over the copied data
        CONSUME(data, u8, dataSize);
    }

    niaGIFImageDecodeStreamChunk* currentChunk = stream.head;

    while(currentChunk){
        oldCode = code;
        assert(codeSize <= 12);
        code = readBits(currentChunk, codeSize);

        if(code == endCode){
            break;
        } else {
            if(getDictionarySize(&dictionary) >= 4096){
                codeSize = LZWMinimumCodeSize + 1;
                resetDictionary(&dictionary, clearCode);
            }

            if(code == clearCode){
                codeSize = LZWMinimumCodeSize + 1;
                cleanDictionary(&dictionary);
                resetDictionary(&dictionary, clearCode);

                code = readBits(currentChunk, codeSize);

                niaGIFLZWEntry entry = getEntryFromDictionary(&dictionary, code);
                for(int i = 0; i < entry.length; ++i){
                    putColor(colorTable, pixels, &position, entry.values[i]);
                }
                continue;
            }

            if(code >= getDictionarySize(&dictionary)){
                niaGIFLZWEntry oldEntry = getEntryFromDictionary(&dictionary, oldCode);
                u16 K = oldEntry.values[0];

                niaGIFLZWEntry newEntry = createEntry(oldEntry.length + 1);

                for(int i = 0; i < oldEntry.length; ++i){
                    newEntry.values[i] = oldEntry.values[i];
                }
                newEntry.values[oldEntry.length] = K;

                for(int i = 0; i < newEntry.length; ++i){
                    putColor(colorTable, pixels, &position, newEntry.values[i]);
                }
                
                addEntryToDictionary(&dictionary, newEntry);
            } else {
                niaGIFLZWEntry entry = getEntryFromDictionary(&dictionary, code);

                for(int i = 0; i < entry.length; ++i){
                    putColor(colorTable, pixels, &position, entry.values[i]);
                }

                if(oldCode == clearCode){
                    continue;
                }
                u16 K = entry.values[0];

                niaGIFLZWEntry oldEntry = getEntryFromDictionary(&dictionary, oldCode);
                niaGIFLZWEntry newEntry = createEntry(oldEntry.length + 1);
                
                for(int i = 0; i < oldEntry.length; ++i){
                    newEntry.values[i] = oldEntry.values[i];
                }
                newEntry.values[oldEntry.length] = K;

                addEntryToDictionary(&dictionary, newEntry);
            }

            if((codeSize < 12) && (getDictionarySize(&dictionary) == (1 << codeSize))){
                ++codeSize;
            }
        }
    }
    cleanDictionary(&dictionary);
    cleanStream(&stream);
    // TODO replace this will writing directly to he finalimage, not to the pixels first
    // fill the image
    // not the best approch but its done only once per frame and the frames will be done
    for(u32 y = 0; y < imageDescriptor.imageHeight; ++y){
        u32 yy = y + imageDescriptor.imageTopPosition;
        for(u32 x = 0; x < imageDescriptor.imageWidth * 4; x += 4){
            u32 xx = x + imageDescriptor.imageLeftPosition * 4;

            u32 r = pixels[(x + 0 + y * imageDescriptor.imageWidth * 4)];
            u32 g = pixels[(x + 1 + y * imageDescriptor.imageWidth * 4)];
            u32 b = pixels[(x + 2 + y * imageDescriptor.imageWidth * 4)];

            if(hasTransperancy){
                if(r == colorTable[transparentColorIndex * 3 + 0] &&
                   g == colorTable[transparentColorIndex * 3 + 1] &&
                   b == colorTable[transparentColorIndex * 3 + 2]){
                    continue;
                }
            }
            finalImage[(xx + 0 + yy * frames[0].width * 4)] = r;
            finalImage[(xx + 1 + yy * frames[0].width * 4)] = g;
            finalImage[(xx + 2 + yy * frames[0].width * 4)] = b;
            finalImage[(xx + 3 + yy * frames[0].width * 4)] = 0xff;
        }
    }

    if(isLocalColorTablePresent){
        delete[] colorTable;
    }
}

NIA_CALL void niaGifParser::decodeFile(){
    numberOfFrames = 0;

    niaGIFHeader header;
    CONSUME(data, niaGIFHeader, header);

    niaGIFLogicalScreenDescriptor screenDescriptor;
    CONSUME(data, niaGIFLogicalScreenDescriptor, screenDescriptor);

    width = screenDescriptor.localScreenWidth;
    height = screenDescriptor.localScreenHeight;

    u8 globalColorTablePresent = GET_NO_STREAM_BITS(screenDescriptor.packedFields, 7, 1);
    u8 bitsPerColorResolution = GET_NO_STREAM_BITS(screenDescriptor.packedFields, 6, 3) + 1;
    u8 sortFlag = GET_NO_STREAM_BITS(screenDescriptor.packedFields, 3, 1);
    u8 bitsPerPixel = GET_NO_STREAM_BITS(screenDescriptor.packedFields, 2, 3) + 1;

    u8* colorTable = NULL;
    u32 colorTableSize = 0;
    backgroundColorIndex = screenDescriptor.backgroundColorIndex;

    if(globalColorTablePresent){
        colorTableSize = 3 * POWER(2, bitsPerPixel);
        colorTable = new u8[colorTableSize];

        u32 index = 0;
        while(colorTableSize--){
            CONSUME(data, u8, colorTable[index++]);
        }
    }

    u8 flag = 0;
    while(flag != ';'){
        CONSUME(data, u8, flag);

        switch(flag){
            case '!': {
                    u8 extensionLabel;
                    CONSUME(data, u8, extensionLabel);


                    switch(extensionLabel){
                        case 0xF9: {
                                niaGIFGraphicControlExtension graphicControlExtension;
                                CONSUME(data, niaGIFGraphicControlExtension, graphicControlExtension);

                                disposalMethod = GET_NO_STREAM_BITS(graphicControlExtension.packedFileds, 4, 3);
                                transparentColorIndex = graphicControlExtension.transparentColorIndex;
                                hasTransperancy = GET_NO_STREAM_BITS(graphicControlExtension.packedFileds, 0, 1);
                            }
                            break;

                        case 0xFF: { // application extension block
                                niaGIFApplicationExtension applicationExtension;
                                CONSUME(data, niaGIFApplicationExtension, applicationExtension);
                                printf("%c%c%c%c%c%c%c%c \n", 
                                    applicationExtension.applicationIndentifire[0],
                                    applicationExtension.applicationIndentifire[1],
                                    applicationExtension.applicationIndentifire[2],
                                    applicationExtension.applicationIndentifire[3],
                                    applicationExtension.applicationIndentifire[4],
                                    applicationExtension.applicationIndentifire[5],
                                    applicationExtension.applicationIndentifire[6],
                                    applicationExtension.applicationIndentifire[7]
                                );

                                printf("%c%c%C\n", 
                                    applicationExtension.applicationAuthentificationCode[0],
                                    applicationExtension.applicationAuthentificationCode[1],
                                    applicationExtension.applicationAuthentificationCode[2]
                                );

                                if(COMPARE_3BYTECODE("XMP", applicationExtension.applicationAuthentificationCode)){
                                    u8 garbage = 1;
                                    while(garbage != 0){
                                        CONSUME(data, u8, garbage);
                                    }                                
                                } else { // netscape
                                    u8 blockSize; // TODO goto struct 
                                    CONSUME(data, u8, blockSize);

                                    u8 garbage;
                                    while(blockSize--){
                                        CONSUME(data, u8, garbage);
                                    }                                
                                }

                            } 
                            break;

                        case 0xFE: { // comment extension
                                u8 blockSize;
                                CONSUME(data, u8, blockSize);
                                while(blockSize--){
                                    u8 character;
                                    CONSUME(data, u8, character);
                                    printf("%c", character);
                                }
                                printf("\n");
                            }
                            break;


                        default: {
                            }
                            break;
                    }
                }
                break;

            case ',': {
                    decodeImageData(colorTable);
                }
                break;
        }
    }

    delete[] colorTable;
}

NIA_CALL void niaGifParser::loadFile(const char* filename){
    FILE* file = fopen(filename, "rb");

    NIA_ASSERT(file);
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    data = new u8[fileSize];
    NIA_ASSERT(data);

    u32 read = fread(data, sizeof(u8), fileSize, file);
    NIA_ASSERT(read == fileSize);

    fclose(file);
}


NIA_CALL u32 niaGifParser::getWidth(){
    return width;
}

NIA_CALL u32 niaGifParser::getHeight(){
    return height;
}

NIA_CALL u8* niaGifParser::getPixelData() const { // just return the first frame
    return frames[0].data;
}

NIA_CALL textureFormatDetails niaGifParser::getTextureFormat() const {
    return textureFormat;
}

NIA_CALL niaGIFFrame* niaGifParser::getFrames(){
    return frames;
}

NIA_CALL u32 niaGifParser::getNumberOfFrames(){
    return numberOfFrames;
}