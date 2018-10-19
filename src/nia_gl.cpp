#include "nia_gl.h"

PFNGLGETUNIFORMLOCATIONPROC _nia_glGetUniformLocation = NULL;
PFNGLUNIFORM1DPROC _nia_glUniform1d = NULL;
PFNGLUNIFORM1FPROC _nia_glUniform1f = NULL;

PFNGLBINDBUFFERPROC _nia_glBindBuffer = NULL;
PFNGLBUFFERDATAPROC _nia_glBufferData = NULL;
PFNGLGENBUFFERSPROC _nia_glGenBuffers = NULL;
PFNGLMAPBUFFERPROC _nia_glMapBuffer = NULL;
PFNGLUNMAPBUFFERPROC _nia_glUnmapBuffer = NULL;

PFNGLBINDVERTEXARRAYPROC _nia_glBindVertexArray = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC _nia_glDisableVertexAttribArray = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC _nia_glEnableVertexAttribArray = NULL;
PFNGLGENVERTEXARRAYSPROC _nia_glGenVertexArrays = NULL;
PFNGLVERTEXATTRIBPOINTERPROC _nia_glVertexAttribPointer = NULL;

PFNGLCREATEPROGRAMPROC _nia_glCreateProgram = NULL;
PFNGLGETATTRIBLOCATIONPROC _nia_glGetAttribLocation = NULL;
PFNGLGETPROGRAMINFOLOGPROC _nia_glGetProgramInfoLog = NULL;
PFNGLGETPROGRAMIVPROC _nia_glGetProgramiv = NULL;
PFNGLLINKPROGRAMPROC _nia_glLinkProgram = NULL;
PFNGLUSEPROGRAMPROC _nia_glUseProgram = NULL;

PFNGLATTACHSHADERPROC _nia_glAttachShader = NULL;
PFNGLCOMPILESHADERPROC _nia_glCompileShader = NULL;
PFNGLCREATESHADERPROC _nia_glCreateShader = NULL;
PFNGLDELETESHADERPROC _nia_glDeleteShader = NULL;
PFNGLDETACHSHADERPROC _nia_glDetachShader = NULL;
PFNGLGETSHADERINFOLOGPROC _nia_glGetShaderInfoLog = NULL;
PFNGLSHADERSOURCEPROC _nia_glShaderSource = NULL;
PFNGLGETSHADERIVPROC _nia_glGetShaderiv = NULL;

NIA_CALL bool niaInitGL(){
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
    extensionLoadCheck = ((glMapBuffer = (PFNGLMAPBUFFERPROC) wglGetProcAddress("glMapBuffer")) && extensionLoadCheck);
    extensionLoadCheck = ((glUnmapBuffer = (PFNGLUNMAPBUFFERPROC) wglGetProcAddress("glUnmapBuffer")) && extensionLoadCheck);

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