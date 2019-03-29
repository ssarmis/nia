#ifndef _NIA_GL_H_
#define _NIA_GL_H_

#include "nia_general.h"

#ifdef _WIN32
#   include "arch/win32/khronos/glcorearb.h"
#   include <windows.h>
#   include <nia_wgl.h>
#elif defined  __unix__
#   include <GL/glcorearb.h>
#   include <nia_xgl.h>
#endif

NIA_EXTERN NIA_CALL PFNGLGENFRAMEBUFFERSPROC _nia_glGenFramebuffers;
#define glGenFramebuffers _nia_glGenFramebuffers
NIA_EXTERN NIA_CALL PFNGLBINDFRAMEBUFFERPROC _nia_glBindFramebuffer;
#define glBindFramebuffer _nia_glBindFramebuffer
NIA_EXTERN NIA_CALL PFNGLFRAMEBUFFERTEXTUREPROC _nia_glFramebufferTexture;
#define glFramebufferTexture _nia_glFramebufferTexture
NIA_EXTERN NIA_CALL PFNGLDELETEBUFFERSPROC _nia_glDeleteFramebuffers;
#define glDeleteFramebuffers _nia_glDeleteFramebuffers
NIA_EXTERN NIA_CALL PFNGLCHECKFRAMEBUFFERSTATUSPROC _nia_glCheckFramebufferStatus;
#define glCheckFramebufferStatus _nia_glCheckFramebufferStatus
NIA_EXTERN NIA_CALL PFNGLDRAWBUFFERPROC _nia_glDrawBuffers;
#define glDrawBuffers _nia_glDrawBuffers

NIA_EXTERN NIA_CALL PFNGLGETUNIFORMLOCATIONPROC _nia_glGetUniformLocation;
#define glGetUniformLocation _nia_glGetUniformLocation
NIA_EXTERN NIA_CALL PFNGLUNIFORM1DPROC _nia_glUniform1d;
#define glUniform1d _nia_glUniform1d
NIA_EXTERN NIA_CALL PFNGLUNIFORM1FPROC _nia_glUniform1f;
#define glUniform1f _nia_glUniform1f
NIA_EXTERN NIA_CALL PFNGLUNIFORM3FPROC _nia_glUniform3f;
#define glUniform3f _nia_glUniform3f
NIA_EXTERN NIA_CALL PFNGLUNIFORMMATRIX4FVPROC _nia_glUniformMatrix4fv;
#define glUniformMatrix4fv _nia_glUniformMatrix4fv

NIA_EXTERN NIA_CALL PFNGLBINDBUFFERPROC _nia_glBindBuffer;
#define glBindBuffer _nia_glBindBuffer
NIA_EXTERN NIA_CALL PFNGLBUFFERDATAPROC _nia_glBufferData;
#define glBufferData _nia_glBufferData
NIA_EXTERN NIA_CALL PFNGLGENBUFFERSPROC _nia_glGenBuffers;
#define glGenBuffers _nia_glGenBuffers
NIA_EXTERN NIA_CALL PFNGLMAPBUFFERPROC _nia_glMapBuffer;
#define glMapBuffer _nia_glMapBuffer
NIA_EXTERN NIA_CALL PFNGLUNMAPBUFFERPROC _nia_glUnmapBuffer;
#define glUnmapBuffer _nia_glUnmapBuffer
NIA_EXTERN NIA_CALL PFNGLBUFFERSUBDATAPROC _nia_glBufferSubData;
#define glBufferSubData _nia_glBufferSubData
NIA_EXTERN NIA_CALL PFNGLNAMEDBUFFERSUBDATAPROC _nia_glNamedBufferSubData;
#define glNamedBufferSubData _nia_glNamedBufferSubData

NIA_EXTERN NIA_CALL PFNGLBINDVERTEXARRAYPROC _nia_glBindVertexArray;
#define glBindVertexArray _nia_glBindVertexArray
NIA_EXTERN NIA_CALL PFNGLDISABLEVERTEXATTRIBARRAYPROC _nia_glDisableVertexAttribArray;
#define glDisableVertexAttribArray _nia_glDisableVertexAttribArray
NIA_EXTERN NIA_CALL PFNGLENABLEVERTEXATTRIBARRAYPROC _nia_glEnableVertexAttribArray;
#define glEnableVertexAttribArray _nia_glEnableVertexAttribArray
NIA_EXTERN NIA_CALL PFNGLGENVERTEXARRAYSPROC _nia_glGenVertexArrays;
#define glGenVertexArrays _nia_glGenVertexArrays
NIA_EXTERN NIA_CALL PFNGLVERTEXATTRIBPOINTERPROC _nia_glVertexAttribPointer;
#define glVertexAttribPointer _nia_glVertexAttribPointer
NIA_EXTERN NIA_CALL PFNGLDRAWELEMENTSPROC _nia_glDrawElements;
#define glDrawElements _nia_glDrawElements

NIA_EXTERN NIA_CALL PFNGLCREATEPROGRAMPROC _nia_glCreateProgram;
#define glCreateProgram _nia_glCreateProgram
NIA_EXTERN NIA_CALL PFNGLGETATTRIBLOCATIONPROC _nia_glGetAttribLocation;
#define glGetAttribLocation _nia_glGetAttribLocation
NIA_EXTERN NIA_CALL PFNGLGETPROGRAMINFOLOGPROC _nia_glGetProgramInfoLog;
#define glGetProgramInfoLog _nia_glGetProgramInfoLog
NIA_EXTERN NIA_CALL PFNGLGETPROGRAMIVPROC _nia_glGetProgramiv;
#define glGetProgramiv _nia_glGetProgramiv
NIA_EXTERN NIA_CALL PFNGLLINKPROGRAMPROC _nia_glLinkProgram;
#define glLinkProgram _nia_glLinkProgram
NIA_EXTERN NIA_CALL PFNGLUSEPROGRAMPROC _nia_glUseProgram;
#define glUseProgram _nia_glUseProgram
NIA_EXTERN NIA_CALL PFNGLDELETEPROGRAMPROC _nia_glDeleteProgram;
#define glDeleteProgram _nia_glDeleteProgram
NIA_EXTERN NIA_CALL PFNGLVALIDATEPROGRAMPROC _nia_glValidateProgram;
#define glValidateProgram _nia_glValidateProgram

NIA_EXTERN NIA_CALL PFNGLATTACHSHADERPROC _nia_glAttachShader;
#define glAttachShader _nia_glAttachShader
NIA_EXTERN NIA_CALL PFNGLCOMPILESHADERPROC _nia_glCompileShader;
#define glCompileShader _nia_glCompileShader
NIA_EXTERN NIA_CALL PFNGLCREATESHADERPROC _nia_glCreateShader;
#define glCreateShader _nia_glCreateShader
NIA_EXTERN NIA_CALL PFNGLDELETESHADERPROC _nia_glDeleteShader;
#define glDeleteShader _nia_glDeleteShader
NIA_EXTERN NIA_CALL PFNGLDETACHSHADERPROC _nia_glDetachShader;
#define glDetachShader _nia_glDetachShader
NIA_EXTERN NIA_CALL PFNGLGETSHADERINFOLOGPROC _nia_glGetShaderInfoLog;
#define glGetShaderInfoLog _nia_glGetShaderInfoLog
NIA_EXTERN NIA_CALL PFNGLSHADERSOURCEPROC _nia_glShaderSource;
#define glShaderSource _nia_glShaderSource
NIA_EXTERN NIA_CALL PFNGLGETSHADERIVPROC _nia_glGetShaderiv;
#define glGetShaderiv _nia_glGetShaderiv

NIA_EXTERN NIA_CALL PFNGLGENTEXTURESPROC _nia_glGenTextures;
#define glGenTextures _nia_glGenTextures
NIA_EXTERN NIA_CALL PFNGLBINDTEXTUREPROC _nia_glBindTexture;
#define glBindTexture _nia_glBindTexture
NIA_EXTERN NIA_CALL PFNGLTEXPARAMETERIPROC _nia_glTexParameteri;
#define glTexParameteri _nia_glTexParameteri
NIA_EXTERN NIA_CALL PFNGLTEXPARAMETERFVPROC _nia_glTexParameterfv;
#define glTexParameterfv _nia_glTexParameterfv
NIA_EXTERN NIA_CALL PFNGLGENERATEMIPMAPPROC _nia_glGenerateMipmap;
#define glGenerateMipmap _nia_glGenerateMipmap
NIA_EXTERN NIA_CALL PFNGLTEXIMAGE2DPROC _nia_glTexImage2D;
#define glTexImage2D _nia_glTexImage2D


#ifdef _WIN32
NIA_INTERNAL WINGDIAPI PROC WINAPI niaGetProcAddress(const char* name);
#elif defined __unix__
void* niaGetProcAddress(const char* name);
#endif

NIA_CALL bool niaInitGL();

#endif// _NIA_GL_H_