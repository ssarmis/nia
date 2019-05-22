#include "nia_texture_gl.h"

#include "parsers/nia_bmp_parser.h"
#include "parsers/nia_tga_parser.h"
#include "parsers/nia_png_parser.h"
#include "parsers/nia_gif_parser.h"

#include "nia_texture_streaming.h"

void initializeAnimatedTextureChain(niaAnimatedTextureChain* textureChain){
    textureChain->current = NULL;
    textureChain->head = NULL;
    textureChain->tail = NULL;
}

niaAnimatedTexture* createAnimatedTexture(u32 width, u32 height, u8* data, const textureFormatDetails& details){
    niaAnimatedTexture* result = new niaAnimatedTexture;
    result->next = NULL;
    result->texture = new niaTexture(data, width, height, details);
    return result;
}

void addAnimatedTextureToChain(niaAnimatedTextureChain* textureChain, niaAnimatedTexture* texture){
    if(textureChain->head){
        textureChain->tail->next = texture;
        textureChain->tail = texture;
    } else {
        textureChain->head = textureChain->tail = texture;
    }
}

void writeGIFFramesToTextureChain(niaGIFFrame* frame, niaAnimatedTextureChain* textureChain, u32 numberOfFrames){
    u32 index = 0;
    while(numberOfFrames--){
        niaAnimatedTexture* textureInChain = createAnimatedTexture(frame[index].width, frame[index].height, frame[index].data, NIA_TEXTURE_FORMAT_RGBA_RGBA_UBYTE);
        addAnimatedTextureToChain(textureChain, textureInChain);
        ++index;
    }
}

niaTexture::niaTexture(const char* filename, u32 flag, u16 frameNumber){
    // for now I will assume there will only be .bmp files
    // TODO add extension cheking, header checking, more file formats forsers
    isAnimated = flag & NIA_TEXTURE_ANIMATED;
    bool canBeStreamed = true;

    textureFormatDetails textureFormat;
    const char* tmp = filename;
    while(*tmp){
        if(*tmp++ == '.'){
            switch (*tmp) {
                case 't':{
                        isAnimated = false; // this format doesn't support animation, force it to false
                        niaTgaParser parser(filename);
                        allocateTexture(parser.getWidth(), parser.getHeight(), parser.getPixelData(), parser.getTextureFormat());
                        textureFormat = parser.getTextureFormat();
                    }
                    break;

                case 'b':{
                        isAnimated = false; // this format doesn't support animation, force it to false
                        niaBmpParser parser(filename);
                        allocateTexture(parser.getWidth(), parser.getHeight(), parser.getPixelData(), parser.getTextureFormat());
                        textureFormat = parser.getTextureFormat();
                    }
                    break;

                case 'p':{
                        isAnimated = false; // this format doesn't support animation, force it to false
                        niaPngParser parser(filename);
                        allocateTexture(parser.getWidth(), parser.getHeight(), parser.getPixelData(), parser.getTextureFormat());
                        textureFormat = parser.getTextureFormat();
                    }
                    break;

                case 'g':{
                        niaGifParser parser(filename);

                        if(isAnimated){
                            initializeAnimatedTextureChain(&textureChain);
                            writeGIFFramesToTextureChain(parser.getFrames(), &textureChain, parser.getNumberOfFrames());
                        } else {
                            if(frameNumber > parser.getNumberOfFrames()){
                                frameNumber = parser.getNumberOfFrames();
                            }
                            allocateTexture(parser.getWidth(), parser.getHeight(), parser.getFrames()[frameNumber - 1].data, parser.getTextureFormat()); 
                        }
                        
                        canBeStreamed = false; // it would rather take a long time to reload everything, can be done, will be, but not now
                    }
                    break;

                default:{
                    }
                    break;
            }
        }
    }
#ifdef _WIN32
    if(canBeStreamed){
        niaTextureStreaming::appendLiveLoadingTexture(textureId, (char*)filename, textureFormat);
    }
#endif
}

niaTexture::niaTexture(u8* data, u32 width, u32 height, const textureFormatDetails& details){
    isAnimated = false;
    allocateTexture(width, height, data, details); // data should be first, but whatever
}

niaTexture::niaTexture(){
    u8 pixels[] = {
        255, 255, 255,
        255, 255, 255
    };

    allocateTexture(1, 1, pixels);
}

niaTexture::~niaTexture(){
}

NIA_INTERNAL void niaTexture::allocateTexture(u32 width, u32 height, u8* data, const textureFormatDetails& details){ // TODO add ability to change depth
    NIA_GL_CALL(glGenTextures(1, &textureId));

    NIA_GL_CALL(glBindTexture(GL_TEXTURE_2D, textureId));

    NIA_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    NIA_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    NIA_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    NIA_GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

    NIA_GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
    NIA_GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, details.storeFormat, width, height, 0, details.readFormat, details.type, data));

    NIA_GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}

niaTexture* getNextTextureInChain(niaAnimatedTextureChain* textureChain){
    NIA_ASSERT(textureChain->head); // what to take from an empty chain
    // if by mistake the flag is set we just set it to false
    if(textureChain->current){
        if(textureChain->current->next){
            textureChain->current = textureChain->current->next;
            return textureChain->current->texture;
        }
    }
    textureChain->current = textureChain->head;
    return textureChain->current->texture;
}

u32 niaTexture::getTextureId() {
    if(isAnimated){
        return (*getNextTextureInChain(&textureChain)).textureId;
    }
    return textureId;
}