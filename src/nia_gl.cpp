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
PFNGLDRAWELEMENTSPROC _nia_glDrawElements = NULL;

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


#ifdef _WIN32
NIA_INTERNAL WINGDIAPI PROC WINAPI getProcAddress(const char* name){
#elif defined __unix__
void* getProcAddress(const char* name){
#endif
#ifdef _WIN32
    NIA_STATIC HINSTANCE gl = LoadLibrary("opengl32.dll");
    
    WINGDIAPI PROC WINAPI result = GetProcAddress(gl, name);
    if (result){
        return result;
    }
    return wglGetProcAddress(name);
#elif defined __unix__
    return glXGetProcAddress(name);
#endif
}

NIA_CALL bool niaInitGL(){
    bool extensionLoadCheck = true; 

    extensionLoadCheck = ((glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) getProcAddress("glGetUniformLocation")) && extensionLoadCheck);
    extensionLoadCheck = ((glUniform1d = (PFNGLUNIFORM1DPROC) getProcAddress("glUniform1d")) && extensionLoadCheck);
    extensionLoadCheck = ((glUniform1f = (PFNGLUNIFORM1FPROC) getProcAddress("glUniform1f")) && extensionLoadCheck);

    extensionLoadCheck = ((glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC) getProcAddress("glBindVertexArray")) && extensionLoadCheck);
    extensionLoadCheck = ((glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) getProcAddress("glDisableVertexAttribArray")) && extensionLoadCheck);
    extensionLoadCheck = ((glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) getProcAddress("glEnableVertexAttribArray")) && extensionLoadCheck);
    extensionLoadCheck = ((glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC) getProcAddress("glGenVertexArrays")) && extensionLoadCheck);
    extensionLoadCheck = ((glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC) getProcAddress("glVertexAttribPointer")) && extensionLoadCheck);
    extensionLoadCheck = ((glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC) getProcAddress("glGetAttribLocation")) && extensionLoadCheck);
    extensionLoadCheck = ((glDrawElements = (PFNGLDRAWELEMENTSPROC) getProcAddress("glDrawElements")) && extensionLoadCheck);

    extensionLoadCheck = ((glGenBuffers = (PFNGLGENBUFFERSPROC) getProcAddress("glGenBuffers")) && extensionLoadCheck);
    extensionLoadCheck = ((glBindBuffer = (PFNGLBINDBUFFERPROC) getProcAddress("glBindBuffer")) && extensionLoadCheck);
    extensionLoadCheck = ((glBufferData = (PFNGLBUFFERDATAPROC) getProcAddress("glBufferData")) && extensionLoadCheck);
    extensionLoadCheck = ((glMapBuffer = (PFNGLMAPBUFFERPROC) getProcAddress("glMapBuffer")) && extensionLoadCheck);
    extensionLoadCheck = ((glUnmapBuffer = (PFNGLUNMAPBUFFERPROC) getProcAddress("glUnmapBuffer")) && extensionLoadCheck);

    extensionLoadCheck = ((glCreateProgram = (PFNGLCREATEPROGRAMPROC) getProcAddress("glCreateProgram")) && extensionLoadCheck);
    extensionLoadCheck = ((glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC) getProcAddress("glGetProgramInfoLog")) && extensionLoadCheck);
    extensionLoadCheck = ((glGetProgramiv = (PFNGLGETPROGRAMIVPROC) getProcAddress("glGetProgramiv")) && extensionLoadCheck);
    extensionLoadCheck = ((glLinkProgram = (PFNGLLINKPROGRAMPROC) getProcAddress("glLinkProgram")) && extensionLoadCheck);
    extensionLoadCheck = ((glUseProgram = (PFNGLUSEPROGRAMPROC) getProcAddress("glUseProgram")) && extensionLoadCheck);

    extensionLoadCheck = ((glAttachShader = (PFNGLATTACHSHADERPROC) getProcAddress("glAttachShader")) && extensionLoadCheck);
    extensionLoadCheck = ((glCompileShader = (PFNGLCOMPILESHADERPROC) getProcAddress("glCompileShader")) && extensionLoadCheck);
    extensionLoadCheck = ((glCreateShader = (PFNGLCREATESHADERPROC) getProcAddress("glCreateShader")) && extensionLoadCheck);
    extensionLoadCheck = ((glDeleteShader = (PFNGLDELETESHADERPROC) getProcAddress("glDeleteShader")) && extensionLoadCheck);
    extensionLoadCheck = ((glDetachShader = (PFNGLDETACHSHADERPROC) getProcAddress("glDetachShader")) && extensionLoadCheck);
    extensionLoadCheck = ((glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC) getProcAddress("glGetShaderInfoLog")) && extensionLoadCheck);
    extensionLoadCheck = ((glGetShaderiv = (PFNGLGETSHADERIVPROC) getProcAddress("glGetShaderiv")) && extensionLoadCheck);
    extensionLoadCheck = ((glShaderSource = (PFNGLSHADERSOURCEPROC) getProcAddress("glShaderSource")) && extensionLoadCheck);

    return extensionLoadCheck;
}