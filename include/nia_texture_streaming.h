#ifdef _WIN32 //tmp

#ifndef _NIA_NIA_TEXTURE_STREAMING_H_
#define _NIA_NIA_TEXTURE_STREAMING_H_

#include "nia_general.h"
#include "nia_array.h"

#include "nia_gl.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include "nia_texture_structures.h"

#include "parsers/nia_bmp_parser.h"
#include "parsers/nia_tga_parser.h"

NIA_CLASS niaTextureStreaming {
private:
    static textureLiveLoadingStream liveLoadingStream;

public:
    NIA_CALL static void initializeStream();
    NIA_CALL static void updateStreams();
    NIA_CALL static void appendLiveLoadingTexture(u32 textureId, char* filename, const textureFormatDetails& details);
    NIA_CALL static void updateTexture(const textureLiveLoadingChunk& chunk, HANDLE fileHandle);

};

#endif // _NIA_NIA_TEXTURE_STREAMING_H_
#endif