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
    return wglGetProcAddress(name);
#elif defined __unix__
    return glXGetProcAddress(name);
#endif
}

NIA_CALL bool niaInitGL(){
    bool extensionLoadCheck = true; 

    extensionLoadCheck = ((glDrawBuffer = (PFNGLDRAWBUFFERPROC) niaGetProcAddress("glDrawBuffer")) && extensionLoadCheck);

    extensionLoadCheck = ((glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC) niaGetProcAddress("glGenFramebuffers")) && extensionLoadCheck);
    extensionLoadCheck = ((glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC) niaGetProcAddress("glBindFramebuffer")) && extensionLoadCheck);
    extensionLoadCheck = ((glFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC) niaGetProcAddress("glFramebufferTexture")) && extensionLoadCheck);
    extensionLoadCheck = ((glDeleteFramebuffers = (PFNGLDELETEBUFFERSPROC) niaGetProcAddress("glDeleteFramebuffers")) && extensionLoadCheck);
    extensionLoadCheck = ((glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC) niaGetProcAddress("glCheckFramebufferStatus")) && extensionLoadCheck);
    extensionLoadCheck = ((glDrawBuffers = (PFNGLDRAWBUFFERPROC) niaGetProcAddress("glDrawBuffers")) && extensionLoadCheck);

    extensionLoadCheck = ((glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC) niaGetProcAddress("glGetUniformLocation")) && extensionLoadCheck);
    extensionLoadCheck = ((glUniform1d = (PFNGLUNIFORM1DPROC) niaGetProcAddress("glUniform1d")) && extensionLoadCheck);
    extensionLoadCheck = ((glUniform1f = (PFNGLUNIFORM1FPROC) niaGetProcAddress("glUniform1f")) && extensionLoadCheck);
    extensionLoadCheck = ((glUniform3f = (PFNGLUNIFORM3FPROC) niaGetProcAddress("glUniform3f")) && extensionLoadCheck);
    extensionLoadCheck = ((glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC) niaGetProcAddress("glUniformMatrix4fv")) && extensionLoadCheck);

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
    extensionLoadCheck = ((glBufferSubData = (PFNGLBUFFERSUBDATAPROC) niaGetProcAddress("glBufferSubData")) && extensionLoadCheck);
    extensionLoadCheck = ((glNamedBufferSubData = (PFNGLNAMEDBUFFERSUBDATAPROC) niaGetProcAddress("glNamedBufferSubData")) && extensionLoadCheck);

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

    extensionLoadCheck = ((glGenTextures = (PFNGLGENTEXTURESPROC) niaGetProcAddress("glGenTextures")) && extensionLoadCheck);
    extensionLoadCheck = ((glBindTexture = (PFNGLBINDTEXTUREPROC) niaGetProcAddress("glBindTexture")) && extensionLoadCheck);
    extensionLoadCheck = ((glTexParameteri = (PFNGLTEXPARAMETERIPROC) niaGetProcAddress("glTexParameteri")) && extensionLoadCheck);
    extensionLoadCheck = ((glTexParameterfv = (PFNGLTEXPARAMETERFVPROC) niaGetProcAddress("glTexParameterfv")) && extensionLoadCheck);
    extensionLoadCheck = ((glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC) niaGetProcAddress("glGenerateMipmap")) && extensionLoadCheck);
    extensionLoadCheck = ((glTexImage2D = (PFNGLTEXIMAGE2DPROC) niaGetProcAddress("glTexImage2D")) && extensionLoadCheck);

    return extensionLoadCheck;
}