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


#ifdef _WIN32
NIA_INTERNAL WINGDIAPI PROC WINAPI niaGetProcAddress(const char* name){
#elif defined __unix__
void* niaGetProcAddress(const char* name){
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

    extensionLoadCheck = ((glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) niaGetProcAddress("glGetUniformLocation")) && extensionLoadCheck);
    extensionLoadCheck = ((glUniform1d = (PFNGLUNIFORM1DPROC) niaGetProcAddress("glUniform1d")) && extensionLoadCheck);
    extensionLoadCheck = ((glUniform1f = (PFNGLUNIFORM1FPROC) niaGetProcAddress("glUniform1f")) && extensionLoadCheck);

    extensionLoadCheck = ((glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC) niaGetProcAddress("glBindVertexArray")) && extensionLoadCheck);
    extensionLoadCheck = ((glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) niaGetProcAddress("glDisableVertexAttribArray")) && extensionLoadCheck);
    extensionLoadCheck = ((glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC) niaGetProcAddress("glEnableVertexAttribArray")) && extensionLoadCheck);
    extensionLoadCheck = ((glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC) niaGetProcAddress("glGenVertexArrays")) && extensionLoadCheck);
    extensionLoadCheck = ((glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC) niaGetProcAddress("glVertexAttribPointer")) && extensionLoadCheck);
    extensionLoadCheck = ((glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC) niaGetProcAddress("glGetAttribLocation")) && extensionLoadCheck);
    extensionLoadCheck = ((glDrawElements = (PFNGLDRAWELEMENTSPROC) niaGetProcAddress("glDrawElements")) && extensionLoadCheck);

    extensionLoadCheck = ((glGenBuffers = (PFNGLGENBUFFERSPROC) niaGetProcAddress("glGenBuffers")) && extensionLoadCheck);
    extensionLoadCheck = ((glBindBuffer = (PFNGLBINDBUFFERPROC) niaGetProcAddress("glBindBuffer")) && extensionLoadCheck);
    extensionLoadCheck = ((glBufferData = (PFNGLBUFFERDATAPROC) niaGetProcAddress("glBufferData")) && extensionLoadCheck);
    extensionLoadCheck = ((glMapBuffer = (PFNGLMAPBUFFERPROC) niaGetProcAddress("glMapBuffer")) && extensionLoadCheck);
    extensionLoadCheck = ((glUnmapBuffer = (PFNGLUNMAPBUFFERPROC) niaGetProcAddress("glUnmapBuffer")) && extensionLoadCheck);

    extensionLoadCheck = ((glCreateProgram = (PFNGLCREATEPROGRAMPROC) niaGetProcAddress("glCreateProgram")) && extensionLoadCheck);
    extensionLoadCheck = ((glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC) niaGetProcAddress("glGetProgramInfoLog")) && extensionLoadCheck);
    extensionLoadCheck = ((glGetProgramiv = (PFNGLGETPROGRAMIVPROC) niaGetProcAddress("glGetProgramiv")) && extensionLoadCheck);
    extensionLoadCheck = ((glLinkProgram = (PFNGLLINKPROGRAMPROC) niaGetProcAddress("glLinkProgram")) && extensionLoadCheck);
    extensionLoadCheck = ((glUseProgram = (PFNGLUSEPROGRAMPROC) niaGetProcAddress("glUseProgram")) && extensionLoadCheck);
    extensionLoadCheck = ((glDeleteProgram = (PFNGLDELETEPROGRAMPROC) niaGetProcAddress("glDeleteProgram")) && extensionLoadCheck);
    extensionLoadCheck = ((glValidateProgram = (PFNGLVALIDATEPROGRAMPROC) niaGetProcAddress("glValidateProgram")) && extensionLoadCheck);
    
    extensionLoadCheck = ((glAttachShader = (PFNGLATTACHSHADERPROC) niaGetProcAddress("glAttachShader")) && extensionLoadCheck);
    extensionLoadCheck = ((glCompileShader = (PFNGLCOMPILESHADERPROC) niaGetProcAddress("glCompileShader")) && extensionLoadCheck);
    extensionLoadCheck = ((glCreateShader = (PFNGLCREATESHADERPROC) niaGetProcAddress("glCreateShader")) && extensionLoadCheck);
    extensionLoadCheck = ((glDeleteShader = (PFNGLDELETESHADERPROC) niaGetProcAddress("glDeleteShader")) && extensionLoadCheck);
    extensionLoadCheck = ((glDetachShader = (PFNGLDETACHSHADERPROC) niaGetProcAddress("glDetachShader")) && extensionLoadCheck);
    extensionLoadCheck = ((glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC) niaGetProcAddress("glGetShaderInfoLog")) && extensionLoadCheck);
    extensionLoadCheck = ((glGetShaderiv = (PFNGLGETSHADERIVPROC) niaGetProcAddress("glGetShaderiv")) && extensionLoadCheck);
    extensionLoadCheck = ((glShaderSource = (PFNGLSHADERSOURCEPROC) niaGetProcAddress("glShaderSource")) && extensionLoadCheck);

    return extensionLoadCheck;
}