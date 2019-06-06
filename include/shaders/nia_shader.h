/*
 * File: nia_shader.h
 * Project: Nia
 * File created on April-29-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

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

    GLuint NIA_CALL getProgram() const;
    GLuint NIA_CALL getVertexShader() const;
    GLuint NIA_CALL getFragmentShader() const;

    void NIA_CALL setUniform1f(const char* name, r32 value);
    void NIA_CALL setUniformU32(const char* name, u32 value);
    void NIA_CALL setUniformVec3(const char* name, const niaVector3<r32>& vec);
    void NIA_CALL setUniformMat4(const char* name, const niaMatrix4& mat, bool transpose = true);

    void NIA_CALL useShader();
    void NIA_CALL unuseShader();
};

#endif // _NIA_BATCH_RENDERER_H_