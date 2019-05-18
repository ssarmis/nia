#ifndef _NIA_NIA_TEXTURE_STRCTURES_H_
#define _NIA_NIA_TEXTURE_STRCTURES_H_

#include "nia_general.h"

#ifdef _WIN32
#include <windows.h>
#elif defined __unix__
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#include "nia_gl.h"

NIA_STRUCT textureFormatDetails{
    GLenum storeFormat;
    GLenum readFormat;
    GLenum type;
} textureFormatDetails;

NIA_STRUCT textureLiveLoadingChunk {
    char* texturePath;
    textureFormatDetails details;
    u32 textureId;
#ifdef _WIN32
    FILETIME writeTime;
#elif defined __unix__
    struct stat writeStat;
#endif

    i16 writeMinute;
    i16 writeSecond;

    struct textureLiveLoadingChunk* next;
} textureLiveLoadingChunk;

NIA_STRUCT textureLiveLoadingStream {
    textureLiveLoadingChunk* head;
    textureLiveLoadingChunk* tail;
} textureLiveLoadingStream;


NIA_STATIC textureFormatDetails NIA_TEXTURE_FORMAT_RGB_BGR_UBYTE = {
    GL_RGB,
    GL_BGR,
    GL_UNSIGNED_BYTE
};

NIA_STATIC textureFormatDetails NIA_TEXTURE_FORMAT_RGB_RGB_UBYTE = {
    GL_RGB,
    GL_RGB,
    GL_UNSIGNED_BYTE
};

NIA_STATIC textureFormatDetails NIA_TEXTURE_FORMAT_RGBA_RGBA_UBYTE = {
    GL_RGBA,
    GL_RGBA,
    GL_UNSIGNED_BYTE
};

NIA_STATIC textureFormatDetails NIA_TEXTURE_FORMAT_R8_RED_UBYTE = {
    GL_R8,
    GL_RED,
    GL_UNSIGNED_BYTE
};

#endif // _NIA_NIA_TEXTURE_STRCTURES_H_