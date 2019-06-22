/*
 * File: nia_texture_streaming.h
 * Project: Nia
 * File created on April-29-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef _NIA_NIA_TEXTURE_STREAMING_H_
#define _NIA_NIA_TEXTURE_STREAMING_H_

#include "nia_general.h"
#include "nia_array.h"

#include "nia_gl.h"

#ifdef _WIN32
#include <windows.h>
#elif defined __unix__
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#endif

#include "nia_texture_structures.h"

#include "parsers/nia_bmp_parser.h"
#include "parsers/nia_tga_parser.h"
#include "parsers/nia_png_parser.h"

NIA_CLASS niaTextureStreaming {
private:
    static textureLiveLoadingStream liveLoadingStream;

public:
    static void NIA_CALL initializeStream();
    static 
#ifdef _WIN32
    void
#elif defined __unix__
    void*
#endif
    NIA_CALL updateStreams(void*);
    static void NIA_CALL appendLiveLoadingTexture(u32 textureId, char* filename, const textureFormatDetails& details);
    static void NIA_CALL updateTexture(const textureLiveLoadingChunk& chunk);

};

#endif // _NIA_NIA_TEXTURE_STREAMING_H_