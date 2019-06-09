/*
 * File: nia_gl.cpp
 * Project: Nia
 * File created on October-20-2018
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "nia_gl.h"

PFNGLDRAWBUFFERPROC _nia_glDrawBuffer = NULL;

PFNGLBLITFRAMEBUFFERPROC _nia_glBlitFramebuffer = NULL;
PFNGLGENFRAMEBUFFERSPROC _nia_glGenFramebuffers = NULL;
PFNGLBINDFRAMEBUFFERPROC _nia_glBindFramebuffer = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC _nia_glFramebufferTexture2D = NULL;
PFNGLFRAMEBUFFERTEXTUREPROC _nia_glFramebufferTexture = NULL;
PFNGLDELETEBUFFERSPROC _nia_glDeleteFramebuffers = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSPROC _nia_glCheckFramebufferStatus = NULL;
PFNGLDRAWBUFFERPROC _nia_glDrawBuffers = NULL;

PFNGLGETUNIFORMLOCATIONPROC _nia_glGetUniformLocation = NULL;
PFNGLUNIFORM1IPROC _nia_glUniform1i = NULL;
PFNGLUNIFORM1UIPROC _nia_glUniform1ui = NULL;
PFNGLUNIFORM1DPROC _nia_glUniform1d = NULL;
PFNGLUNIFORM1FPROC _nia_glUniform1f = NULL;
PFNGLUNIFORM3FPROC _nia_glUniform3f = NULL;
PFNGLUNIFORMMATRIX4FVPROC _nia_glUniformMatrix4fv = NULL;

PFNGLBINDBUFFERPROC _nia_glBindBuffer = NULL;
PFNGLBUFFERDATAPROC _nia_glBufferData = NULL;
PFNGLGENBUFFERSPROC _nia_glGenBuffers = NULL;
PFNGLMAPBUFFERPROC _nia_glMapBuffer = NULL;
PFNGLUNMAPBUFFERPROC _nia_glUnmapBuffer = NULL;
PFNGLBUFFERSUBDATAPROC _nia_glBufferSubData = NULL;
PFNGLNAMEDBUFFERSUBDATAPROC _nia_glNamedBufferSubData = NULL;

PFNGLBINDVERTEXARRAYPROC _nia_glBindVertexArray = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC _nia_glDisableVertexAttribArray = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC _nia_glEnableVertexAttribArray = NULL;
PFNGLGENVERTEXARRAYSPROC _nia_glGenVertexArrays = NULL;
PFNGLVERTEXATTRIBPOINTERPROC _nia_glVertexAttribPointer = NULL;
PFNGLDRAWELEMENTSPROC _nia_glDrawElements = NULL;

PFNGLCREATEPROGRAMPROC _nia_glCreateProgram = NULL;
PFNGLGETATTRIBLOCATIONPROC _nia_glGetAttribLocation = NULL;
PFNGLGETPROGRAMINFOLOGPROC _nia_glGetProgramInfoLog = NULL;
PFNGLGETPROGRAMIVPROC _nia_glGetProgramiv = NULL;
PFNGLLINKPROGRAMPROC _nia_glLinkProgram = NULL;
PFNGLUSEPROGRAMPROC _nia_glUseProgram = NULL;
PFNGLDELETEPROGRAMPROC _nia_glDeleteProgram = NULL;
PFNGLVALIDATEPROGRAMPROC _nia_glValidateProgram = NULL;

PFNGLATTACHSHADERPROC _nia_glAttachShader = NULL;
PFNGLCOMPILESHADERPROC _nia_glCompileShader = NULL;
PFNGLCREATESHADERPROC _nia_glCreateShader = NULL;
PFNGLDELETESHADERPROC _nia_glDeleteShader = NULL;
PFNGLDETACHSHADERPROC _nia_glDetachShader = NULL;
PFNGLGETSHADERINFOLOGPROC _nia_glGetShaderInfoLog = NULL;
PFNGLSHADERSOURCEPROC _nia_glShaderSource = NULL;
PFNGLGETSHADERIVPROC _nia_glGetShaderiv = NULL;

PFNGLACTIVETEXTUREPROC _nia_glActiveTexture = NULL;
PFNGLGENTEXTURESPROC _nia_glGenTextures = NULL;
PFNGLBINDTEXTUREPROC _nia_glBindTexture = NULL;
PFNGLTEXPARAMETERIPROC _nia_glTexParameteri = NULL;
PFNGLTEXPARAMETERFVPROC _nia_glTexParameterfv = NULL;
PFNGLGENERATEMIPMAPPROC _nia_glGenerateMipmap = NULL;
PFNGLTEXIMAGE2DPROC _nia_glTexImage2D = NULL;

#ifdef _WIN32
NIA_INTERNAL PROC WINAPI niaGetProcAddress(const char* name){
#elif defined __unix__
void* niaGetProcAddress(const char* name){
#endif
#ifdef _WIN32
    NIA_STATIC HINSTANCE gl = LoadLibrary("opengl32.dll");
    
    PROC WINAPI result = GetProcAddress(gl, name);
    if (result){
        return result;
    }
	result = wglGetProcAddress(name);
	return result;
#elif defined __unix__
    return (void*)glXGetProcAddress((const GLubyte*)name);
#endif
}

#define checkPrint(_x, _s) {\
	_s;\
	if(!(_x)){\
		NIA_ERROR(#_x"\n");\
	}\
}

bool niaInitGL(){
    bool extensionLoadCheck = true; 

    checkPrint(glDrawBuffer, extensionLoadCheck = ((glDrawBuffer = (PFNGLDRAWBUFFERPROC) niaGetProcAddress("glDrawBuffer")) && extensionLoadCheck));

	checkPrint(glBlitFramebuffer, extensionLoadCheck = ((glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC) niaGetProcAddress("glBlitFramebuffer")) && extensionLoadCheck));
	checkPrint(glGenFramebuffers, extensionLoadCheck = ((glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC) niaGetProcAddress("glGenFramebuffers")) && extensionLoadCheck));
	checkPrint(glBindFramebuffer, extensionLoadCheck = ((glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC) niaGetProcAddress("glBindFramebuffer")) && extensionLoadCheck));
	checkPrint(glFramebufferTexture2D, extensionLoadCheck = ((glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC) niaGetProcAddress("glFramebufferTexture2D")) && extensionLoadCheck));
	checkPrint(glFramebufferTexture, extensionLoadCheck = ((glFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC) niaGetProcAddress("glFramebufferTexture")) && extensionLoadCheck));
	checkPrint(glDeleteFramebuffers, extensionLoadCheck = ((glDeleteFramebuffers = (PFNGLDELETEBUFFERSPROC) niaGetProcAddress("glDeleteFramebuffers")) && extensionLoadCheck));
	checkPrint(glCheckFramebufferStatus, extensionLoadCheck = ((glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC) niaGetProcAddress("glCheckFramebufferStatus")) && extensionLoadCheck));
	checkPrint(glDrawBuffers, extensionLoadCheck = ((glDrawBuffers = (PFNGLDRAWBUFFERPROC) niaGetProcAddress("glDrawBuffers")) && extensionLoadCheck));

	checkPrint(glGetUniformLocation, extensionLoadCheck = ((glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) niaGetProcAddress("glGetUniformLocation")) && extensionLoadCheck));
	checkPrint(glUniform1i, extensionLoadCheck = ((glUniform1i = (PFNGLUNIFORM1IPROC) niaGetProcAddress("glUniform1i")) && extensionLoadCheck));
	checkPrint(glUniform1ui, extensionLoadCheck = ((glUniform1ui = (PFNGLUNIFORM1UIPROC) niaGetProcAddress("glUniform1ui")) && extensionLoadCheck));
	checkPrint(glUniform1d, extensionLoadCheck = ((glUniform1d = (PFNGLUNIFORM1DPROC) niaGetProcAddress("glUniform1d")) && extensionLoadCheck));
	checkPrint(glUniform1f, extensionLoadCheck = ((glUniform1f = (PFNGLUNIFORM1FPROC) niaGetProcAddress("glUniform1f")) && extensionLoadCheck));
	checkPrint(glUniform3f, extensionLoadCheck = ((glUniform3f = (PFNGLUNIFORM3FPROC) niaGetProcAddress("glUniform3f")) && extensionLoadCheck));
	checkPrint(glUniformMatrix4fv, extensionLoadCheck = ((glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC) niaGetProcAddress("glUniformMatrix4fv")) && extensionLoadCheck));

	checkPrint(glBindVertexArray, extensionLoadCheck = ((glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC) niaGetProcAddress("glBindVertexArray")) && extensionLoadCheck));
	checkPrint(glDisableVertexAttribArray, extensionLoadCheck = ((glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) niaGetProcAddress("glDisableVertexAttribArray")) && extensionLoadCheck));
	checkPrint(glEnableVertexAttribArray, extensionLoadCheck = ((glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) niaGetProcAddress("glEnableVertexAttribArray")) && extensionLoadCheck));
	checkPrint(glGenVertexArrays, extensionLoadCheck = ((glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC) niaGetProcAddress("glGenVertexArrays")) && extensionLoadCheck));
	checkPrint(glVertexAttribPointer, extensionLoadCheck = ((glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC) niaGetProcAddress("glVertexAttribPointer")) && extensionLoadCheck));
	checkPrint(glGetAttribLocation, extensionLoadCheck = ((glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC) niaGetProcAddress("glGetAttribLocation")) && extensionLoadCheck));
	checkPrint(glDrawElements, extensionLoadCheck = ((glDrawElements = (PFNGLDRAWELEMENTSPROC) niaGetProcAddress("glDrawElements")) && extensionLoadCheck));

	checkPrint(glGenBuffers, extensionLoadCheck = ((glGenBuffers = (PFNGLGENBUFFERSPROC) niaGetProcAddress("glGenBuffers")) && extensionLoadCheck));
	checkPrint(glBindBuffer, extensionLoadCheck = ((glBindBuffer = (PFNGLBINDBUFFERPROC) niaGetProcAddress("glBindBuffer")) && extensionLoadCheck));
	checkPrint(glBufferData, extensionLoadCheck = ((glBufferData = (PFNGLBUFFERDATAPROC) niaGetProcAddress("glBufferData")) && extensionLoadCheck));
	checkPrint(glMapBuffer, extensionLoadCheck = ((glMapBuffer = (PFNGLMAPBUFFERPROC) niaGetProcAddress("glMapBuffer")) && extensionLoadCheck));
	checkPrint(glUnmapBuffer, extensionLoadCheck = ((glUnmapBuffer = (PFNGLUNMAPBUFFERPROC) niaGetProcAddress("glUnmapBuffer")) && extensionLoadCheck));
	checkPrint(glBufferSubData, extensionLoadCheck = ((glBufferSubData = (PFNGLBUFFERSUBDATAPROC) niaGetProcAddress("glBufferSubData")) && extensionLoadCheck));
	checkPrint(glNamedBufferSubData, extensionLoadCheck = ((glNamedBufferSubData = (PFNGLNAMEDBUFFERSUBDATAPROC) niaGetProcAddress("glNamedBufferSubData")) && extensionLoadCheck));

	checkPrint(glCreateProgram, extensionLoadCheck = ((glCreateProgram = (PFNGLCREATEPROGRAMPROC) niaGetProcAddress("glCreateProgram")) && extensionLoadCheck));
	checkPrint(glGetProgramInfoLog, extensionLoadCheck = ((glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC) niaGetProcAddress("glGetProgramInfoLog")) && extensionLoadCheck));
	checkPrint(glGetProgramiv, extensionLoadCheck = ((glGetProgramiv = (PFNGLGETPROGRAMIVPROC) niaGetProcAddress("glGetProgramiv")) && extensionLoadCheck));
	checkPrint(glLinkProgram, extensionLoadCheck = ((glLinkProgram = (PFNGLLINKPROGRAMPROC) niaGetProcAddress("glLinkProgram")) && extensionLoadCheck));
	checkPrint(glUseProgram, extensionLoadCheck = ((glUseProgram = (PFNGLUSEPROGRAMPROC) niaGetProcAddress("glUseProgram")) && extensionLoadCheck));
	checkPrint(glDeleteProgram, extensionLoadCheck = ((glDeleteProgram = (PFNGLDELETEPROGRAMPROC) niaGetProcAddress("glDeleteProgram")) && extensionLoadCheck));
	checkPrint(glValidateProgram, extensionLoadCheck = ((glValidateProgram = (PFNGLVALIDATEPROGRAMPROC) niaGetProcAddress("glValidateProgram")) && extensionLoadCheck));
    
	checkPrint(glAttachShader, extensionLoadCheck = ((glAttachShader = (PFNGLATTACHSHADERPROC) niaGetProcAddress("glAttachShader")) && extensionLoadCheck));
	checkPrint(glCompileShader, extensionLoadCheck = ((glCompileShader = (PFNGLCOMPILESHADERPROC) niaGetProcAddress("glCompileShader")) && extensionLoadCheck));
	checkPrint(glCreateShader, extensionLoadCheck = ((glCreateShader = (PFNGLCREATESHADERPROC) niaGetProcAddress("glCreateShader")) && extensionLoadCheck));
	checkPrint(glDeleteShader, extensionLoadCheck = ((glDeleteShader = (PFNGLDELETESHADERPROC) niaGetProcAddress("glDeleteShader")) && extensionLoadCheck));
	checkPrint(glDetachShader, extensionLoadCheck = ((glDetachShader = (PFNGLDETACHSHADERPROC) niaGetProcAddress("glDetachShader")) && extensionLoadCheck));
	checkPrint(glGetShaderInfoLog, extensionLoadCheck = ((glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC) niaGetProcAddress("glGetShaderInfoLog")) && extensionLoadCheck));
	checkPrint(glGetShaderiv, extensionLoadCheck = ((glGetShaderiv = (PFNGLGETSHADERIVPROC) niaGetProcAddress("glGetShaderiv")) && extensionLoadCheck));
	checkPrint(glShaderSource, extensionLoadCheck = ((glShaderSource = (PFNGLSHADERSOURCEPROC) niaGetProcAddress("glShaderSource")) && extensionLoadCheck));

	checkPrint(glActiveTexture, extensionLoadCheck = ((glActiveTexture = (PFNGLACTIVETEXTUREPROC) niaGetProcAddress("glActiveTexture")) && extensionLoadCheck));
	checkPrint(glGenTextures, extensionLoadCheck = ((glGenTextures = (PFNGLGENTEXTURESPROC) niaGetProcAddress("glGenTextures")) && extensionLoadCheck));
	checkPrint(glBindTexture, extensionLoadCheck = ((glBindTexture = (PFNGLBINDTEXTUREPROC) niaGetProcAddress("glBindTexture")) && extensionLoadCheck));
	checkPrint(glTexParameteri, extensionLoadCheck = ((glTexParameteri = (PFNGLTEXPARAMETERIPROC) niaGetProcAddress("glTexParameteri")) && extensionLoadCheck));
	checkPrint(glTexParameterfv, extensionLoadCheck = ((glTexParameterfv = (PFNGLTEXPARAMETERFVPROC) niaGetProcAddress("glTexParameterfv")) && extensionLoadCheck));
	checkPrint(glGenerateMipmap, extensionLoadCheck = ((glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC) niaGetProcAddress("glGenerateMipmap")) && extensionLoadCheck));
	checkPrint(glTexImage2D, extensionLoadCheck = ((glTexImage2D = (PFNGLTEXIMAGE2DPROC) niaGetProcAddress("glTexImage2D")) && extensionLoadCheck));

    return extensionLoadCheck;
}