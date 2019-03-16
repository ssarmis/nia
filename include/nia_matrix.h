#ifndef _NIA_MATRIX_H_
#define _NIA_MATRIX_H_

#include "nia_general.h"

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

NIA_CLASS niaMatrix4 {
public:
    union {
        r32 m[MAT_4_SIZE] __attribute__((aligned(16))); // for faster SSE
    };

    NIA_CALL niaMatrix4(r32 data = 1);
    NIA_CALL ~niaMatrix4();

    NIA_CALL void add_self(r32 number); // to avoind copying when using it for current matrix
    NIA_CALL niaMatrix4 add(r32 number) const;
    NIA_CALL niaMatrix4 add(const niaMatrix4& other) const;

    NIA_CALL void sub_self(r32 number);
    NIA_CALL niaMatrix4 sub(r32 number) const;
    NIA_CALL niaMatrix4 sub(const niaMatrix4& other) const;

    NIA_CALL void mul_self(r32 number);
    NIA_CALL niaMatrix4 mul(r32 number) const;
    NIA_CALL niaMatrix4 mul(const niaMatrix4& other) const;

    NIA_CALL void scale_self(r32 amount);
    NIA_CALL void rotate_self(r32 angle, niaAxis axis);

    static NIA_CALL niaMatrix4 identity();
    static NIA_CALL niaMatrix4 rotate(r32 angle, niaAxis axis);
    static NIA_CALL niaMatrix4 scale(r32 amount);
    static NIA_CALL niaMatrix4 translate(r32 x, r32 y, r32 z);
    static NIA_CALL niaMatrix4 orthographic(r32 left, r32 right, r32 top, r32 bottom, r32 near, r32 far);
    static NIA_CALL niaMatrix4 perspective(r32 fov, r32 aspectRatio, r32 near, r32 far);

    static NIA_CALL void printMat4(const niaMatrix4& mat);
};

#endif// _NIA_MATRIX_H_