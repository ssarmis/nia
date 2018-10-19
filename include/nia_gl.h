#ifndef _NIA_GL_H_
#define _NIA_GL_H_

#include <windows.h>

#include "nia_general.h"

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/wglext.h>

NIA_CALL PFNGLGETUNIFORMLOCATIONPROC _nia_glGetUniformLocation;
#define glGetUniformLocation _nia_glGetUniformLocation
NIA_CALL PFNGLUNIFORM1DPROC _nia_glUniform1d;
#define glUniform1d _nia_glUniform1d
NIA_CALL PFNGLUNIFORM1FPROC _nia_glUniform1f;
#define glUniform1f _nia_glUniform1f

NIA_CALL PFNGLBINDBUFFERPROC _nia_glBindBuffer;
#define glBindBuffer _nia_glBindBuffer
NIA_CALL PFNGLBUFFERDATAPROC _nia_glBufferData;
#define glBufferData _nia_glBufferData
NIA_CALL PFNGLGENBUFFERSPROC _nia_glGenBuffers;
#define glGenBuffers _nia_glGenBuffers

NIA_CALL PFNGLBINDVERTEXARRAYPROC _nia_glBindVertexArray;
#define glBindVertexArray _nia_glBindVertexArray
NIA_CALL PFNGLDISABLEVERTEXATTRIBARRAYPROC _nia_glDisableVertexAttribArray;
#define glDisableVertexAttribArray _nia_glDisableVertexAttribArray
NIA_CALL PFNGLENABLEVERTEXATTRIBARRAYPROC _nia_glEnableVertexAttribArray;
#define glEnableVertexAttribArray _nia_glEnableVertexAttribArray
NIA_CALL PFNGLGENVERTEXARRAYSPROC _nia_glGenVertexArrays;
#define glGenVertexArrays _nia_glGenVertexArrays
NIA_CALL PFNGLVERTEXATTRIBPOINTERPROC _nia_glVertexAttribPointer;
#define glVertexAttribPointer _nia_glVertexAttribPointer

NIA_CALL PFNGLCREATEPROGRAMPROC _nia_glCreateProgram;
#define glCreateProgram _nia_glCreateProgram
NIA_CALL PFNGLGETATTRIBLOCATIONPROC _nia_glGetAttribLocation;
#define glGetAttribLocation _nia_glGetAttribLocation
NIA_CALL PFNGLGETPROGRAMINFOLOGPROC _nia_glGetProgramInfoLog;
#define glGetProgramInfoLog _nia_glGetProgramInfoLog
NIA_CALL PFNGLGETPROGRAMIVPROC _nia_glGetProgramiv;
#define glGetProgramiv _nia_glGetProgramiv
NIA_CALL PFNGLLINKPROGRAMPROC _nia_glLinkProgram;
#define glLinkProgram _nia_glLinkProgram
NIA_CALL PFNGLUSEPROGRAMPROC _nia_glUseProgram;
#define glUseProgram _nia_glUseProgram

NIA_CALL PFNGLATTACHSHADERPROC _nia_glAttachShader;
#define glAttachShader _nia_glAttachShader
NIA_CALL PFNGLCOMPILESHADERPROC _nia_glCompileShader;
#define glCompileShader _nia_glCompileShader
NIA_CALL PFNGLCREATESHADERPROC _nia_glCreateShader;
#define glCreateShader _nia_glCreateShader
NIA_CALL PFNGLDELETESHADERPROC _nia_glDeleteShader;
#define glDeleteShader _nia_glDeleteShader
NIA_CALL PFNGLDETACHSHADERPROC _nia_glDetachShader;
#define glDetachShader _nia_glDetachShader
NIA_CALL PFNGLGETSHADERINFOLOGPROC _nia_glGetShaderInfoLog;
#define glGetShaderInfoLog _nia_glGetShaderInfoLog
NIA_CALL PFNGLSHADERSOURCEPROC _nia_glShaderSource;
#define glShaderSource _nia_glShaderSource
NIA_CALL PFNGLGETSHADERIVPROC _nia_glGetShaderiv;
#define glGetShaderiv _nia_glGetShaderiv

bool niaInitGL(){
    bool extensionLoadCheck = true; 

    extensionLoadCheck = ((glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) wglGetProcAddress("glGetUniformLocation")) && extensionLoadCheck);
    extensionLoadCheck = ((glUniform1d = (PFNGLUNIFORM1DPROC) wglGetProcAddress("glUniform1d")) && extensionLoadCheck);
    extensionLoadCheck = ((glUniform1f = (PFNGLUNIFORM1FPROC) wglGetProcAddress("glUniform1f")) && extensionLoadCheck);

    extensionLoadCheck = ((glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC) wglGetProcAddress("glBindVertexArray")) && extensionLoadCheck);
    extensionLoadCheck = ((glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) wglGetProcAddress("glDisableVertexAttribArray")) && extensionLoadCheck);
    extensionLoadCheck = ((glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) wglGetProcAddress("glEnableVertexAttribArray")) && extensionLoadCheck);
    extensionLoadCheck = ((glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC) wglGetProcAddress("glGenVertexArrays")) && extensionLoadCheck);
    extensionLoadCheck = ((glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC) wglGetProcAddress("glVertexAttribPointer")) && extensionLoadCheck);
    extensionLoadCheck = ((glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC) wglGetProcAddress("glGetAttribLocation")) && extensionLoadCheck);

    extensionLoadCheck = ((glGenBuffers = (PFNGLGENBUFFERSPROC) wglGetProcAddress("glGenBuffers")) && extensionLoadCheck);
    extensionLoadCheck = ((glBindBuffer = (PFNGLBINDBUFFERPROC) wglGetProcAddress("glBindBuffer")) && extensionLoadCheck);
    extensionLoadCheck = ((glBufferData = (PFNGLBUFFERDATAPROC) wglGetProcAddress("glBufferData")) && extensionLoadCheck);

    extensionLoadCheck = ((glCreateProgram = (PFNGLCREATEPROGRAMPROC) wglGetProcAddress("glCreateProgram")) && extensionLoadCheck);
    extensionLoadCheck = ((glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC) wglGetProcAddress("glGetProgramInfoLog")) && extensionLoadCheck);
    extensionLoadCheck = ((glGetProgramiv = (PFNGLGETPROGRAMIVPROC) wglGetProcAddress("glGetProgramiv")) && extensionLoadCheck);
    extensionLoadCheck = ((glLinkProgram = (PFNGLLINKPROGRAMPROC) wglGetProcAddress("glLinkProgram")) && extensionLoadCheck);
    extensionLoadCheck = ((glUseProgram = (PFNGLUSEPROGRAMPROC) wglGetProcAddress("glUseProgram")) && extensionLoadCheck);

    extensionLoadCheck = ((glAttachShader = (PFNGLATTACHSHADERPROC) wglGetProcAddress("glAttachShader")) && extensionLoadCheck);
    extensionLoadCheck = ((glCompileShader = (PFNGLCOMPILESHADERPROC) wglGetProcAddress("glCompileShader")) && extensionLoadCheck);
    extensionLoadCheck = ((glCreateShader = (PFNGLCREATESHADERPROC) wglGetProcAddress("glCreateShader")) && extensionLoadCheck);
    extensionLoadCheck = ((glDeleteShader = (PFNGLDELETESHADERPROC) wglGetProcAddress("glDeleteShader")) && extensionLoadCheck);
    extensionLoadCheck = ((glDetachShader = (PFNGLDETACHSHADERPROC) wglGetProcAddress("glDetachShader")) && extensionLoadCheck);
    extensionLoadCheck = ((glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC) wglGetProcAddress("glGetShaderInfoLog")) && extensionLoadCheck);
    extensionLoadCheck = ((glGetShaderiv = (PFNGLGETSHADERIVPROC) wglGetProcAddress("glGetShaderiv")) && extensionLoadCheck);
    extensionLoadCheck = ((glShaderSource = (PFNGLSHADERSOURCEPROC) wglGetProcAddress("glShaderSource")) && extensionLoadCheck);

    return extensionLoadCheck;
}
#endif// _NIA_GL_H_