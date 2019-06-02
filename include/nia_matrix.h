/*
 * File: nia_matrix.h
 * Project: Nia
 * File created on March-01-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef _NIA_MATRIX_H_
#define _NIA_MATRIX_H_

#include "nia_general.h"
#include "nia_vector.h"

// #include <xmmintrin.h>
#include <smmintrin.h>

#define MAT_3_SIZE 9 // 3 * 3
#define MAT_4_SIZE 16 // 3 * 3

typedef enum niaAxis {
    NIA_AXIS_X = 0,
    NIA_AXIS_Y,
    NIA_AXIS_Z
} niaAxis;

// mat 3
NIA_STRUCT niaMat3i {
    union {
        i32 data[MAT_3_SIZE];
        i32 m[MAT_3_SIZE];
    };
} niaMat3i;

NIA_STRUCT niaMat3f {
    union {
        r32 data[MAT_3_SIZE];
        r32 m[MAT_3_SIZE];
    };
} niaMat3r;

NIA_STRUCT niaMat3d {
    union {
        r64 data[MAT_3_SIZE];
        r64 m[MAT_3_SIZE];
    };
} niaMat3d;

// mat 4
NIA_STRUCT niaMat4i {
    union {
        i32 data[MAT_4_SIZE];
        i32 m[MAT_4_SIZE];
    };
} niaMat4i;

NIA_STRUCT niaMat4f {
    union {
        r32 data[MAT_4_SIZE];
        r32 m[MAT_4_SIZE];
    };
} niaMat4f;

NIA_STRUCT niaMat4d {
    union {
        r64 data[MAT_4_SIZE];
        r64 m[MAT_4_SIZE];
    };
} niaMat4d;

NIA_STRUCT nia__m128 {
    union {
        __m128 m;
        float data[4];
    };
} nia__m128;

NIA_CLASS niaMatrix4 {
public:
	
    NIA_ALIGN(16) r32 m[MAT_4_SIZE];

    NIA_CALL niaMatrix4(r32 data = 1);
    NIA_CALL ~niaMatrix4();

    void NIA_CALL add_self(r32 number); // to avoind copying when using it for current matrix
    niaMatrix4 NIA_CALL add(r32 number) const;
    niaMatrix4 NIA_CALL add(const niaMatrix4& other) const;

    void NIA_CALL sub_self(r32 number);
    niaMatrix4 NIA_CALL sub(r32 number) const;
    niaMatrix4 NIA_CALL sub(const niaMatrix4& other) const;

    void NIA_CALL mul_self(r32 number);
    niaMatrix4 NIA_CALL mul(r32 number) const;
    niaMatrix4 NIA_CALL mul(const niaMatrix4& other) const;

    void NIA_CALL scale_self(r32 amount);
    void NIA_CALL rotate_self(r32 angle, niaAxis axis);

    static bool NIA_CALL compareMat4(const niaMatrix4& left, const niaMatrix4& right);
    static niaMatrix4 NIA_CALL identity();
    static niaMatrix4 NIA_CALL rotate(r32 angle, niaAxis axis);
    static niaMatrix4 NIA_CALL scale(r32 amount);
    static niaMatrix4 NIA_CALL translate(r32 x, r32 y, r32 z);
    static niaMatrix4 NIA_CALL orthographic(r32 left, r32 right, r32 top, r32 bottom, r32 near, r32 far);
    static niaMatrix4 NIA_CALL perspective(r32 fov, r32 aspectRatio, r32 near, r32 far);
    static niaMatrix4 NIA_CALL lookAt(const niaVector3<r32>& position, const niaVector3<r32>& target, const niaVector3<r32>& up);

    static void NIA_CALL printMat4(const niaMatrix4& mat);
};

#endif// _NIA_MATRIX_H_