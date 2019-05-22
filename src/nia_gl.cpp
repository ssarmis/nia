#include "nia_gl.h"

PFNGLDRAWBUFFERPROC _nia_glDrawBuffer = NULL;

PFNGLGENFRAMEBUFFERSPROC _nia_glGenFramebuffers = NULL;
PFNGLBINDFRAMEBUFFERPROC _nia_glBindFramebuffer = NULL;
PFNGLFRAMEBUFFERTEXTUREPROC _nia_glFramebufferTexture = NULL;
PFNGLDELETEBUFFERSPROC _nia_glDeleteFramebuffers = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSPROC _nia_glCheckFramebufferStatus = NULL;
PFNGLDRAWBUFFERPROC _nia_glDrawBuffers = NULL;

PFNGLGETUNIFORMLOCATIONPROC _nia_glGetUniformLocation = NULL;
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

PFNGLGENTEXTURESPROC _nia_glGenTextures = NULL;
PFNGLBINDTEXTUREPROC _nia_glBindTexture = NULL;
PFNGLTEXPARAMETERIPROC _nia_glTexParameteri = NULL;
PFNGLTEXPARAMETERFVPROC _nia_glTexParameterfv = NULL;
PFNGLGENERATEMIPMAPPROC _nia_glGenerateMipmap = NULL;
PFNGLTEXIMAGE2DPROC _nia_glTexImage2D = NULL;

#include <wingdi.h>

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
	if(!(_x)){\
		NIA_ERROR(_s"\n");\
	}\
}

bool niaInitGL(){
    bool extensionLoadCheck = true; 

    checkPrint(extensionLoadCheck = ((glDrawBuffer = (PFNGLDRAWBUFFERPROC) niaGetProcAddress("glDrawBuffer")) && extensionLoadCheck), "glDrawBuffer");

	checkPrint(extensionLoadCheck = ((glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC) niaGetProcAddress("glGenFramebuffers")) && extensionLoadCheck), "glGenFramebuffers");
	checkPrint(extensionLoadCheck = ((glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC) niaGetProcAddress("glBindFramebuffer")) && extensionLoadCheck), "glBindFramebuffer");
	checkPrint(extensionLoadCheck = ((glFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC) niaGetProcAddress("glFramebufferTexture")) && extensionLoadCheck), "glFramebufferTexture");
	checkPrint(extensionLoadCheck = ((glDeleteFramebuffers = (PFNGLDELETEBUFFERSPROC) niaGetProcAddress("glDeleteFramebuffers")) && extensionLoadCheck), "glDeleteFramebuffers");
	checkPrint(extensionLoadCheck = ((glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC) niaGetProcAddress("glCheckFramebufferStatus")) && extensionLoadCheck), "glCheckFramebufferStatus");
	checkPrint(extensionLoadCheck = ((glDrawBuffers = (PFNGLDRAWBUFFERPROC) niaGetProcAddress("glDrawBuffers")) && extensionLoadCheck), "glDrawBuffers");

	checkPrint(extensionLoadCheck = ((glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) niaGetProcAddress("glGetUniformLocation")) && extensionLoadCheck), "glGetUniformLocation");
	checkPrint(extensionLoadCheck = ((glUniform1d = (PFNGLUNIFORM1DPROC) niaGetProcAddress("glUniform1d")) && extensionLoadCheck), "glUniform1d");
	checkPrint(extensionLoadCheck = ((glUniform1f = (PFNGLUNIFORM1FPROC) niaGetProcAddress("glUniform1f")) && extensionLoadCheck), "glUniform1f");
	checkPrint(extensionLoadCheck = ((glUniform3f = (PFNGLUNIFORM3FPROC) niaGetProcAddress("glUniform3f")) && extensionLoadCheck), "glUniform3f");
	checkPrint(extensionLoadCheck = ((glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC) niaGetProcAddress("glUniformMatrix4fv")) && extensionLoadCheck), "glUniformMatrix4fv");

	checkPrint(extensionLoadCheck = ((glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC) niaGetProcAddress("glBindVertexArray")) && extensionLoadCheck), "glBindVertexArray");
	checkPrint(extensionLoadCheck = ((glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) niaGetProcAddress("glDisableVertexAttribArray")) && extensionLoadCheck), "glDisableVertexAttribArray");
	checkPrint(extensionLoadCheck = ((glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) niaGetProcAddress("glEnableVertexAttribArray")) && extensionLoadCheck), "glEnableVertexAttribArray");
	checkPrint(extensionLoadCheck = ((glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC) niaGetProcAddress("glGenVertexArrays")) && extensionLoadCheck), "glGenVertexArrays");
	checkPrint(extensionLoadCheck = ((glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC) niaGetProcAddress("glVertexAttribPointer")) && extensionLoadCheck), "glVertexAttribPointer");
	checkPrint(extensionLoadCheck = ((glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC) niaGetProcAddress("glGetAttribLocation")) && extensionLoadCheck), "glGetAttribLocation");
	checkPrint(extensionLoadCheck = ((glDrawElements = (PFNGLDRAWELEMENTSPROC) niaGetProcAddress("glDrawElements")) && extensionLoadCheck), "glDrawElements");

	checkPrint(extensionLoadCheck = ((glGenBuffers = (PFNGLGENBUFFERSPROC) niaGetProcAddress("glGenBuffers")) && extensionLoadCheck), "glGenBuffers");
	checkPrint(extensionLoadCheck = ((glBindBuffer = (PFNGLBINDBUFFERPROC) niaGetProcAddress("glBindBuffer")) && extensionLoadCheck), "glBindBuffer");
	checkPrint(extensionLoadCheck = ((glBufferData = (PFNGLBUFFERDATAPROC) niaGetProcAddress("glBufferData")) && extensionLoadCheck), "glBufferData");
	checkPrint(extensionLoadCheck = ((glMapBuffer = (PFNGLMAPBUFFERPROC) niaGetProcAddress("glMapBuffer")) && extensionLoadCheck), "glMapBuffer");
	checkPrint(extensionLoadCheck = ((glUnmapBuffer = (PFNGLUNMAPBUFFERPROC) niaGetProcAddress("glUnmapBuffer")) && extensionLoadCheck), "glUnmapBuffer");
	checkPrint(extensionLoadCheck = ((glBufferSubData = (PFNGLBUFFERSUBDATAPROC) niaGetProcAddress("glBufferSubData")) && extensionLoadCheck), "glBufferSubData");
	checkPrint(extensionLoadCheck = ((glNamedBufferSubData = (PFNGLNAMEDBUFFERSUBDATAPROC) niaGetProcAddress("glNamedBufferSubData")) && extensionLoadCheck), "glNamedBufferSubData");

	checkPrint(extensionLoadCheck = ((glCreateProgram = (PFNGLCREATEPROGRAMPROC) niaGetProcAddress("glCreateProgram")) && extensionLoadCheck), "glCreateProgram");
	checkPrint(extensionLoadCheck = ((glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC) niaGetProcAddress("glGetProgramInfoLog")) && extensionLoadCheck), "glGetProgramInfoLog");
	checkPrint(extensionLoadCheck = ((glGetProgramiv = (PFNGLGETPROGRAMIVPROC) niaGetProcAddress("glGetProgramiv")) && extensionLoadCheck), "glGetProgramiv");
	checkPrint(extensionLoadCheck = ((glLinkProgram = (PFNGLLINKPROGRAMPROC) niaGetProcAddress("glLinkProgram")) && extensionLoadCheck), "glLinkProgram");
	checkPrint(extensionLoadCheck = ((glUseProgram = (PFNGLUSEPROGRAMPROC) niaGetProcAddress("glUseProgram")) && extensionLoadCheck), "glUseProgram");
	checkPrint(extensionLoadCheck = ((glDeleteProgram = (PFNGLDELETEPROGRAMPROC) niaGetProcAddress("glDeleteProgram")) && extensionLoadCheck), "glDeleteProgram");
	checkPrint(extensionLoadCheck = ((glValidateProgram = (PFNGLVALIDATEPROGRAMPROC) niaGetProcAddress("glValidateProgram")) && extensionLoadCheck), "glValidateProgram");
    
	checkPrint(extensionLoadCheck = ((glAttachShader = (PFNGLATTACHSHADERPROC) niaGetProcAddress("glAttachShader")) && extensionLoadCheck), "glAttachShader");
	checkPrint(extensionLoadCheck = ((glCompileShader = (PFNGLCOMPILESHADERPROC) niaGetProcAddress("glCompileShader")) && extensionLoadCheck), "glCompileShader");
	checkPrint(extensionLoadCheck = ((glCreateShader = (PFNGLCREATESHADERPROC) niaGetProcAddress("glCreateShader")) && extensionLoadCheck), "glCreateShader");
	checkPrint(extensionLoadCheck = ((glDeleteShader = (PFNGLDELETESHADERPROC) niaGetProcAddress("glDeleteShader")) && extensionLoadCheck), "glDeleteShader");
	checkPrint(extensionLoadCheck = ((glDetachShader = (PFNGLDETACHSHADERPROC) niaGetProcAddress("glDetachShader")) && extensionLoadCheck), "glDetachShader");
	checkPrint(extensionLoadCheck = ((glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC) niaGetProcAddress("glGetShaderInfoLog")) && extensionLoadCheck), "glGetShaderInfoLog");
	checkPrint(extensionLoadCheck = ((glGetShaderiv = (PFNGLGETSHADERIVPROC) niaGetProcAddress("glGetShaderiv")) && extensionLoadCheck), "glGetShaderiv");
	checkPrint(extensionLoadCheck = ((glShaderSource = (PFNGLSHADERSOURCEPROC) niaGetProcAddress("glShaderSource")) && extensionLoadCheck), "glShaderSource");

	checkPrint(extensionLoadCheck = ((glGenTextures = (PFNGLGENTEXTURESPROC) niaGetProcAddress("glGenTextures")) && extensionLoadCheck), "glGenTextures");
	checkPrint(extensionLoadCheck = ((glBindTexture = (PFNGLBINDTEXTUREPROC) niaGetProcAddress("glBindTexture")) && extensionLoadCheck), "glBindTexture");
	checkPrint(extensionLoadCheck = ((glTexParameteri = (PFNGLTEXPARAMETERIPROC) niaGetProcAddress("glTexParameteri")) && extensionLoadCheck), "glTexParameteri");
	checkPrint(extensionLoadCheck = ((glTexParameterfv = (PFNGLTEXPARAMETERFVPROC) niaGetProcAddress("glTexParameterfv")) && extensionLoadCheck), "glTexParameterfv");
	checkPrint(extensionLoadCheck = ((glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC) niaGetProcAddress("glGenerateMipmap")) && extensionLoadCheck), "glGenerateMipmap");
	checkPrint(extensionLoadCheck = ((glTexImage2D = (PFNGLTEXIMAGE2DPROC) niaGetProcAddress("glTexImage2D")) && extensionLoadCheck), "glTexImage2D");

    return extensionLoadCheck;
}