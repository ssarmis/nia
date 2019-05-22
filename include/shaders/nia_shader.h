#ifndef _NIA_SHADER_H_
#define _NIA_SHADER_H_

#include "nia_constants.h"

#include "nia_gl.h"

#include "nia_general.h"
#include "nia_vector.h"
#include "nia_matrix.h"

NIA_CLASS niaShader {
protected:
    GLuint vertexShader; 
    GLuint fragmentShader;
    GLuint program;

    NIA_INTERNAL GLuint loadBufferToShader(const char* source, GLenum type);
    NIA_INTERNAL GLuint loadSourceToShader(const char* filename, GLenum type);
    NIA_INTERNAL void compileShader(GLuint shader);
    NIA_INTERNAL void buildProgram(GLuint vertexShader, GLuint fragmentShader);

public:
    niaShader(const char* vertexShaderFilename, const char* fragmentShaderFilename);
    niaShader();
    ~niaShader();

    GLuint getProgram() const;
    GLuint getVertexShader() const;
    GLuint getFragmentShader() const;

    void setUniformVec3(const char* name, const niaVector3<r32>& vec);
    void setUniformMat4(const char* name, const niaMatrix4& mat, bool transpose = true);

    void useShader();
    void unuseShader();
};

#endif // _NIA_BATCH_RENDERER_H_