#ifndef _NIA_GL_H_
#define _NIA_GL_H_

#include "nia_general.h"

#ifdef _WIN32
#   include "arch/win32/khronos/glcorearb.h"
#   include <windows.h>
#   include "nia_wgl.h"
#elif defined  __unix__
#   undef GL_VERSION_1_1
#   include <GL/glcorearb.h>
#   include "nia_xgl.h"
#endif

NIA_EXTERN PFNGLDRAWBUFFERPROC _nia_glDrawBuffer;
#define glDrawBuffer _nia_glDrawBuffer

NIA_EXTERN PFNGLGENFRAMEBUFFERSPROC _nia_glGenFramebuffers;
#define glGenFramebuffers _nia_glGenFramebuffers
NIA_EXTERN PFNGLBINDFRAMEBUFFERPROC _nia_glBindFramebuffer;
#define glBindFramebuffer _nia_glBindFramebuffer
NIA_EXTERN PFNGLFRAMEBUFFERTEXTUREPROC _nia_glFramebufferTexture;
#define glFramebufferTexture _nia_glFramebufferTexture
NIA_EXTERN PFNGLDELETEBUFFERSPROC _nia_glDeleteFramebuffers;
#define glDeleteFramebuffers _nia_glDeleteFramebuffers
NIA_EXTERN PFNGLCHECKFRAMEBUFFERSTATUSPROC _nia_glCheckFramebufferStatus;
#define glCheckFramebufferStatus _nia_glCheckFramebufferStatus
NIA_EXTERN PFNGLDRAWBUFFERPROC _nia_glDrawBuffers;
#define glDrawBuffers _nia_glDrawBuffers

NIA_EXTERN PFNGLGETUNIFORMLOCATIONPROC _nia_glGetUniformLocation;
#define glGetUniformLocation _nia_glGetUniformLocation
NIA_EXTERN PFNGLUNIFORM1DPROC _nia_glUniform1d;
#define glUniform1d _nia_glUniform1d
NIA_EXTERN PFNGLUNIFORM1FPROC _nia_glUniform1f;
#define glUniform1f _nia_glUniform1f
NIA_EXTERN PFNGLUNIFORM3FPROC _nia_glUniform3f;
#define glUniform3f _nia_glUniform3f
NIA_EXTERN PFNGLUNIFORMMATRIX4FVPROC _nia_glUniformMatrix4fv;
#define glUniformMatrix4fv _nia_glUniformMatrix4fv

NIA_EXTERN PFNGLBINDBUFFERPROC _nia_glBindBuffer;
#define glBindBuffer _nia_glBindBuffer
NIA_EXTERN PFNGLBUFFERDATAPROC _nia_glBufferData;
#define glBufferData _nia_glBufferData
NIA_EXTERN PFNGLGENBUFFERSPROC _nia_glGenBuffers;
#define glGenBuffers _nia_glGenBuffers
NIA_EXTERN PFNGLMAPBUFFERPROC _nia_glMapBuffer;
#define glMapBuffer _nia_glMapBuffer
NIA_EXTERN PFNGLUNMAPBUFFERPROC _nia_glUnmapBuffer;
#define glUnmapBuffer _nia_glUnmapBuffer
NIA_EXTERN PFNGLBUFFERSUBDATAPROC _nia_glBufferSubData;
#define glBufferSubData _nia_glBufferSubData
NIA_EXTERN PFNGLNAMEDBUFFERSUBDATAPROC _nia_glNamedBufferSubData;
#define glNamedBufferSubData _nia_glNamedBufferSubData

NIA_EXTERN PFNGLBINDVERTEXARRAYPROC _nia_glBindVertexArray;
#define glBindVertexArray _nia_glBindVertexArray
NIA_EXTERN PFNGLDISABLEVERTEXATTRIBARRAYPROC _nia_glDisableVertexAttribArray;
#define glDisableVertexAttribArray _nia_glDisableVertexAttribArray
NIA_EXTERN PFNGLENABLEVERTEXATTRIBARRAYPROC _nia_glEnableVertexAttribArray;
#define glEnableVertexAttribArray _nia_glEnableVertexAttribArray
NIA_EXTERN PFNGLGENVERTEXARRAYSPROC _nia_glGenVertexArrays;
#define glGenVertexArrays _nia_glGenVertexArrays
NIA_EXTERN PFNGLVERTEXATTRIBPOINTERPROC _nia_glVertexAttribPointer;
#define glVertexAttribPointer _nia_glVertexAttribPointer
NIA_EXTERN PFNGLDRAWELEMENTSPROC _nia_glDrawElements;
#define glDrawElements _nia_glDrawElements

NIA_EXTERN PFNGLCREATEPROGRAMPROC _nia_glCreateProgram;
#define glCreateProgram _nia_glCreateProgram
NIA_EXTERN PFNGLGETATTRIBLOCATIONPROC _nia_glGetAttribLocation;
#define glGetAttribLocation _nia_glGetAttribLocation
NIA_EXTERN PFNGLGETPROGRAMINFOLOGPROC _nia_glGetProgramInfoLog;
#define glGetProgramInfoLog _nia_glGetProgramInfoLog
NIA_EXTERN PFNGLGETPROGRAMIVPROC _nia_glGetProgramiv;
#define glGetProgramiv _nia_glGetProgramiv
NIA_EXTERN PFNGLLINKPROGRAMPROC _nia_glLinkProgram;
#define glLinkProgram _nia_glLinkProgram
NIA_EXTERN PFNGLUSEPROGRAMPROC _nia_glUseProgram;
#define glUseProgram _nia_glUseProgram
NIA_EXTERN PFNGLDELETEPROGRAMPROC _nia_glDeleteProgram;
#define glDeleteProgram _nia_glDeleteProgram
NIA_EXTERN PFNGLVALIDATEPROGRAMPROC _nia_glValidateProgram;
#define glValidateProgram _nia_glValidateProgram

NIA_EXTERN PFNGLATTACHSHADERPROC _nia_glAttachShader;
#define glAttachShader _nia_glAttachShader
NIA_EXTERN PFNGLCOMPILESHADERPROC _nia_glCompileShader;
#define glCompileShader _nia_glCompileShader
NIA_EXTERN PFNGLCREATESHADERPROC _nia_glCreateShader;
#define glCreateShader _nia_glCreateShader
NIA_EXTERN PFNGLDELETESHADERPROC _nia_glDeleteShader;
#define glDeleteShader _nia_glDeleteShader
NIA_EXTERN PFNGLDETACHSHADERPROC _nia_glDetachShader;
#define glDetachShader _nia_glDetachShader
NIA_EXTERN PFNGLGETSHADERINFOLOGPROC _nia_glGetShaderInfoLog;
#define glGetShaderInfoLog _nia_glGetShaderInfoLog
NIA_EXTERN PFNGLSHADERSOURCEPROC _nia_glShaderSource;
#define glShaderSource _nia_glShaderSource
NIA_EXTERN PFNGLGETSHADERIVPROC _nia_glGetShaderiv;
#define glGetShaderiv _nia_glGetShaderiv

NIA_EXTERN PFNGLGENTEXTURESPROC _nia_glGenTextures;
#define glGenTextures _nia_glGenTextures
NIA_EXTERN PFNGLBINDTEXTUREPROC _nia_glBindTexture;
#define glBindTexture _nia_glBindTexture
NIA_EXTERN PFNGLTEXPARAMETERIPROC _nia_glTexParameteri;
#define glTexParameteri _nia_glTexParameteri
NIA_EXTERN PFNGLTEXPARAMETERFVPROC _nia_glTexParameterfv;
#define glTexParameterfv _nia_glTexParameterfv
NIA_EXTERN PFNGLGENERATEMIPMAPPROC _nia_glGenerateMipmap;
#define glGenerateMipmap _nia_glGenerateMipmap
NIA_EXTERN PFNGLTEXIMAGE2DPROC _nia_glTexImage2D;
#define glTexImage2D _nia_glTexImage2D


#ifdef _WIN32
NIA_INTERNAL WINGDIAPI PROC WINAPI niaGetProcAddress(const char* name);
#elif defined __unix__
void* niaGetProcAddress(const char* name);
#endif

bool niaInitGL();

#endif// _NIA_GL_H_