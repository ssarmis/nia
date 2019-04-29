#include "nia_texture_streaming.h"
#include "nia_wgl.h"

textureLiveLoadingStream niaTextureStreaming::liveLoadingStream;

NIA_CALL void niaTextureStreaming::initializeStream(){
    liveLoadingStream.head = NULL;
    liveLoadingStream.tail = NULL;

    DWORD liveLoadingThreadId;
    HANDLE liveLoadingThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)(&(niaTextureStreaming::updateStreams)), NULL, 0, &liveLoadingThreadId);
}

NIA_CALL void niaTextureStreaming::updateStreams(){
    wglMakeCurrent(niaWindow::deviceContext, niaWindow::glRenderContextSecond);
    
    while(true){
        textureLiveLoadingChunk* iterator = liveLoadingStream.head;
        textureLiveLoadingChunk* chunk;
        while(iterator){
            u16 lastMinute;
            u16 lastSecond;

            SYSTEMTIME utcTime;
            SYSTEMTIME localTime;

            HANDLE fileHandle = CreateFileA(iterator->texturePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
            BOOL status = GetFileTime(fileHandle, NULL, NULL, &iterator->writeTime);
            CloseHandle(fileHandle);

            FileTimeToSystemTime(&iterator->writeTime, &utcTime);
            SystemTimeToTzSpecificLocalTime(NULL, &utcTime, &localTime);

            if(localTime.wMinute != lastMinute || localTime.wSecond != lastSecond){
                niaTextureStreaming::updateTexture(*iterator, fileHandle);
            }

            lastMinute = localTime.wMinute;
            lastSecond = localTime.wSecond;

            Sleep(1000);
            iterator = iterator->next;
        }
    }
}

NIA_CALL void niaTextureStreaming::appendLiveLoadingTexture(u32 textureId, char* filename, const textureFormatDetails& details){
    textureLiveLoadingChunk* chunk = new textureLiveLoadingChunk[1];

    chunk->textureId = textureId;
    chunk->texturePath = filename;
    chunk->details = details;
    chunk->next = NULL;

    HANDLE fileHandle = CreateFileA(chunk->texturePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    BOOL status = GetFileTime(fileHandle, NULL, NULL, &chunk->writeTime);
    CloseHandle(fileHandle);

    if(liveLoadingStream.head){
        liveLoadingStream.tail->next = chunk;
        liveLoadingStream.tail = chunk;
    } else {
        liveLoadingStream.head = liveLoadingStream.tail = chunk;
    }

    chunk->next = NULL;
}

NIA_CALL void niaTextureStreaming::updateTexture(const textureLiveLoadingChunk& chunk, HANDLE fileHandle){
    NIA_GL_CALL(glBindTexture(GL_TEXTURE_2D, chunk.textureId));
    

    const char* tmp = chunk.texturePath;
    while(*tmp){
        if(*tmp++ == '.'){
            switch (*tmp) {
                case 't':{
                        niaTgaParser parser(chunk.texturePath);
                        NIA_GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, chunk.details.storeFormat, parser.getWidth(), parser.getHeight(), 0, chunk.details.readFormat, chunk.details.type, parser.getPixelData()));
                    }
                    break;

                case 'b':{
                        niaBmpParser parser(chunk.texturePath);
                        NIA_GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, chunk.details.storeFormat, parser.getWidth(), parser.getHeight(), 0, chunk.details.readFormat, chunk.details.type, parser.getPixelData()));
                    }
                    break;

                default:{
                    }
                    break;
            }
        }
    }
    NIA_GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}