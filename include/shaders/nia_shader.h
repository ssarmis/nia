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
    NIA_CALL niaShader(const char* vertexShaderFilename, const char* fragmentShaderFilename);
    NIA_CALL niaShader();
    NIA_CALL ~niaShader();

    NIA_CALL GLuint getProgram() const;
    NIA_CALL GLuint getVertexShader() const;
    NIA_CALL GLuint getFragmentShader() const;

    NIA_CALL void setUniformVec3(const char* name, const niaVector3<r32>& vec);
    NIA_CALL void setUniformMat4(const char* name, const niaMatrix4& mat, bool transpose = true);

    NIA_CALL void useShader();
    NIA_CALL void unuseShader();
};

#endif // _NIA_BATCH_RENDERER_H_