// TODO this should be a hole different file for a different architecture, but I have no time
#include "nia_texture_streaming.h"

#ifdef _WIN32
#include "nia_wgl.h"
#elif defined __unix__
#include "nia_xgl.h"
#endif


textureLiveLoadingStream niaTextureStreaming::liveLoadingStream;

void niaTextureStreaming::initializeStream(){
    liveLoadingStream.head = NULL;
    liveLoadingStream.tail = NULL;

#ifdef _WIN32
    DWORD liveLoadingThreadId;
    HANDLE liveLoadingThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)(&(niaTextureStreaming::updateStreams)), NULL, 0, &liveLoadingThreadId);
#elif defined __unix__
    pthread_t thread;
    pthread_create(&thread, NULL, &(niaTextureStreaming::updateStreams), NULL);
#endif
}


#ifdef _WIN32
    void
#elif defined __unix__
    void*
#endif
    niaTextureStreaming::updateStreams(void*){
#ifdef _WIN32
    wglMakeCurrent(niaWindow::deviceContext, niaWindow::glRenderContextSecond);
#elif defined __unix__
    glXMakeCurrent(niaWindow::display, niaWindow::window, niaWindow::openglContextSecond);
#endif
    
    while(!niaWindow::closed){
        textureLiveLoadingChunk* iterator = liveLoadingStream.head;
        textureLiveLoadingChunk* chunk;

        u32 minute;
        u32 second;
        while(iterator){
#ifdef _WIN32
            SYSTEMTIME utcTime;
            SYSTEMTIME localTime;

            HANDLE fileHandle = CreateFileA(iterator->texturePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
            BOOL status = GetFileTime(fileHandle, NULL, NULL, &iterator->writeTime);
            CloseHandle(fileHandle);

            FileTimeToSystemTime(&iterator->writeTime, &utcTime);
            SystemTimeToTzSpecificLocalTime(NULL, &utcTime, &localTime);

            minute = localTime.wMinute;
            second = localTime.wSecond;
#elif defined __unix__
            stat(iterator->texturePath, &iterator->writeStat);
            struct tm localTime = *gmtime(&iterator->writeStat.st_mtime);

            minute = localTime.tm_min;
            second = localTime.tm_sec;
            printf("second %d\n", second);
#endif
            if(iterator->writeMinute != minute || iterator->writeSecond != second){
                niaTextureStreaming::updateTexture(*iterator);
            }

            iterator->writeMinute = minute;
            iterator->writeSecond = second;

            iterator = iterator->next;
        }
#ifdef _WIN32
        Sleep(1000);
#elif defined __unix__
        usleep(10000);
#endif
    }

    textureLiveLoadingChunk* iterator = liveLoadingStream.head;
    textureLiveLoadingChunk* tmp = iterator;

    while(iterator){
        tmp = iterator;
        iterator = iterator->next;
        delete tmp;
    }
}

void niaTextureStreaming::appendLiveLoadingTexture(u32 textureId, char* filename, const textureFormatDetails& details){
    textureLiveLoadingChunk* chunk = new textureLiveLoadingChunk;

    chunk->textureId = textureId;
    chunk->texturePath = filename;
    chunk->details = details;
    chunk->next = NULL;
#ifdef _WIN32
    HANDLE fileHandle = CreateFileA(chunk->texturePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    BOOL status = GetFileTime(fileHandle, NULL, NULL, &chunk->writeTime);
    CloseHandle(fileHandle);
#elif defined __unix__
    // TODO 
#endif

    if(liveLoadingStream.head){
        liveLoadingStream.tail->next = chunk;
        liveLoadingStream.tail = chunk;
    } else {
        liveLoadingStream.head = liveLoadingStream.tail = chunk;
    }

    chunk->next = NULL;
}

void niaTextureStreaming::updateTexture(const textureLiveLoadingChunk& chunk){
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