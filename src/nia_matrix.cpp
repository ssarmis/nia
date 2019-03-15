#include "nia_matrix.h"

#include <stdio.h>

// mat 3
NIA_CALL niaMatrix3::niaMatrix3(r32 data){
    nia_memset((u8*)m, 0, MAT_3_SIZE * sizeof(r32));

    m[0 + 0 * 3] = data;
    m[1 + 1 * 3] = data;
    m[2 + 2 * 3] = data;
}

NIA_CALL niaMatrix3::~niaMatrix3(){
}

NIA_CALL niaMatrix3 niaMatrix3::add(r32 number) const{
    niaMatrix3 result = *this;

    for(u8 i = 0; i < MAT_4_SIZE; ++i){
        result.m[i] += number;
    }

    return result;
}

NIA_CALL niaMatrix3 niaMatrix3::add(const niaMatrix3& other) const{
    niaMatrix3 result = *this;

    for(u8 i = 0; i < MAT_4_SIZE; ++i){
        result.m[i] += other.m[i];
    }

    return result;
}

NIA_CALL niaMatrix3 niaMatrix3::sub(r32 number) const{
    niaMatrix3 result = *this;

    for(u8 i = 0; i < MAT_4_SIZE; ++i){
        result.m[i] -= number;
    }

    return result;
}

NIA_CALL niaMatrix3 niaMatrix3::sub(const niaMatrix3& other) const{
    niaMatrix3 result = *this;

    for(u8 i = 0; i < MAT_4_SIZE; ++i){
        result.m[i] -= other.m[i];
    }

    return result;
}

NIA_CALL niaMatrix3 niaMatrix3::mul(r32 number) const{
    niaMatrix3 result = *this;

    for(u8 i = 0; i < MAT_4_SIZE; ++i){
        result.m[i] *= number;
    }

    return result;
}

NIA_CALL niaMatrix3 niaMatrix3::mul(const niaMatrix3& other) const{
    niaMatrix3 result = *this;
    
    for(u8 i = 0; i < 3; ++i){
        for(u8 ii = 0; ii < 3; ++ii){
            for(u8 iii = 0; iii < 3; ++iii){
                result.m[i + ii * 3] = m[iii + i * 3] * other.m[ii + iii * 3];
            }
        }
    }

    return result;
}

NIA_CALL niaMatrix3 niaMatrix3::identity(){
    return niaMatrix3(1.0);
}

// mat 4
NIA_CALL niaMatrix4::niaMatrix4(r32 data){
    nia_memset((u8*)m, 0, MAT_4_SIZE * sizeof(r32));

    m[0 + 0 * 4] = data;
    m[1 + 1 * 4] = data;
    m[2 + 2 * 4] = data;
    m[3 + 3 * 4] = data;
}

NIA_CALL niaMatrix4::~niaMatrix4(){
}

NIA_CALL niaMatrix4 niaMatrix4::add(r32 number) const{
    niaMatrix4 result = *this;

    for(u8 i = 0; i < MAT_4_SIZE; ++i){
        result.m[i] += number;
    }

    return result;
}

NIA_CALL niaMatrix4 niaMatrix4::add(const niaMatrix4& other) const{
    niaMatrix4 result = *this;

    for(u8 i = 0; i < MAT_4_SIZE; ++i){
        result.m[i] += other.m[i];
    }

    return result;
}

NIA_CALL niaMatrix4 niaMatrix4::sub(r32 number) const{
    niaMatrix4 result = *this;

    for(u8 i = 0; i < MAT_4_SIZE; ++i){
        result.m[i] -= number;
    }

    return result;
}

NIA_CALL niaMatrix4 niaMatrix4::sub(const niaMatrix4& other) const{
    niaMatrix4 result = *this;

    for(u8 i = 0; i < MAT_4_SIZE; ++i){
        result.m[i] -= other.m[i];
    }

    return result;
}

NIA_CALL niaMatrix4 niaMatrix4::mul(r32 number) const {
    niaMatrix4 result = *this;

    for(u8 i = 0; i < MAT_4_SIZE; ++i){
        result.m[i] *= number;
    }

    return result;
}

NIA_CALL niaMatrix4 niaMatrix4::mul(const niaMatrix4& other) const{
    niaMatrix4 result = niaMatrix4(0);
    
    for(u8 i = 0; i < 4; ++i){
        for(u8 ii = 0; ii < 4; ++ii){
            for(u8 iii = 0; iii < 4; ++iii){
                result.m[i + ii * 4] += m[iii + ii * 4] * other.m[i + iii * 4];
                // printf("%f = %f * %f\n", result.m[i + ii * 4], m[iii + i * 4], other.m[ii + iii * 4]);
            }
        }
    }

    return result;
}

NIA_CALL niaMatrix4 niaMatrix4::scale(r32 amount){
    niaMatrix4 result(amount);
    result.m[3 + 3 * 4] = 1;
    return result;
}

NIA_CALL niaMatrix4 niaMatrix4::rotate(r32 angle, niaAxis axis){
    niaMatrix4 result(1);

    switch (axis) {
        case NIA_AXIS_X:{
                result.m[1 + 1 * 4] = cos(angle);
                result.m[2 + 1 * 4] = sin(angle);
                result.m[1 + 2 * 4] = -sin(angle);
                result.m[2 + 2 * 4] = cos(angle);
            }
            break;

        case NIA_AXIS_Y:{
                result.m[0 + 0 * 4] = cos(angle);
                result.m[2 + 0 * 4] = -sin(angle);
                result.m[0 + 2 * 4] = sin(angle);
                result.m[2 + 2 * 4] = cos(angle);
            }
            break;

        case NIA_AXIS_Z:{
                result.m[0 + 0 * 4] = cos(angle);
                result.m[1 + 0 * 4] = -sin(angle);
                result.m[0 + 1 * 4] = sin(angle);
                result.m[1 + 1 * 4] = cos(angle);
            }
            break;
    
        default:
            break;
    }


    return result;
}

NIA_CALL niaMatrix4 niaMatrix4::perspective(r32 fov, r32 aspectRatio, r32 near, r32 far){
    niaMatrix4 result(0.0);
#ifdef NIA_NO_TRANSPOSE
    result.m[0 + 0 * 4] = nia_cot(fov / 2) / aspectRatio;
    result.m[1 + 1 * 4] = nia_cot(fov / 2);
    result.m[2 + 2 * 4] = far / (far - near);

    result.m[3 + 2 * 4] = 1;
    result.m[2 + 3 * 4] = - ((far * near) / (far - near));
#else
    result.m[0 + 0 * 4] = nia_cot(fov / 2) / aspectRatio;
    result.m[1 + 1 * 4] = nia_cot(fov / 2);
    
    result.m[2 + 2 * 4] = far / (far - near);
    result.m[3 + 2 * 4] = - ((far * near) / (far - near));

    result.m[2 + 3 * 4] = 1;
#endif
    return result;
}

NIA_CALL niaMatrix4 niaMatrix4::orthographic(r32 left, r32 right, r32 top, r32 bottom, r32 near, r32 far){
    niaMatrix4 result(1.0);

    result.m[0 + 0 * 4] = 2.0 / (right - left);
    result.m[1 + 1 * 4] = 2.0 / (top - bottom);
    result.m[2 + 2 * 4] = -2.0 / (far - near);
    result.m[3 + 2 * 4] = -(far + near) / (far - near);
    
    result.m[3 + 1 * 4] = -(top + bottom) / (top - bottom);
    result.m[3 + 0 * 4] = -(right + left) / (right - left);
    
    return result;
}

NIA_CALL niaMatrix4 niaMatrix4::identity(){
    return niaMatrix4(1.0);
}

NIA_CALL void niaMatrix4::add_self(r32 number){
    for(u8 i = 0; i < MAT_4_SIZE; ++i){
        m[i] += number;
    }
}

NIA_CALL void niaMatrix4::sub_self(r32 number){
    for(u8 i = 0; i < MAT_4_SIZE; ++i){
        m[i] -= number;
    }
}

NIA_CALL void niaMatrix4::mul_self(r32 number){
    for(u8 i = 0; i < MAT_4_SIZE; ++i){
        m[i] *= number;
    }
}

NIA_CALL void niaMatrix4::scale_self(r32 amount){
    // niaMatrix4 old = *this;

    m[0 + 0 * 4] = amount;
    m[1 + 1 * 4] = amount;
    m[2 + 2 * 4] = amount;

    // *this = old.mul(*this);
}

NIA_CALL void niaMatrix4::rotate_self(r32 angle, niaAxis axis){
    niaMatrix4 old = *this;

    switch (axis) {
        case NIA_AXIS_X:{
                m[1 + 1 * 4] = cos(angle);
                m[2 + 1 * 4] = sin(angle);
                m[1 + 2 * 4] = -sin(angle);
                m[2 + 2 * 4] = cos(angle);
            }
            break;

        case NIA_AXIS_Y:{
                m[0 + 0 * 4] = cos(angle);
                m[2 + 0 * 4] = -sin(angle);
                m[0 + 2 * 4] = sin(angle);
                m[2 + 2 * 4] = cos(angle);
            }
            break;

        case NIA_AXIS_Z:{
                m[0 + 0 * 4] = cos(angle);
                m[1 + 0 * 4] = -sin(angle);
                m[0 + 1 * 4] = sin(angle);
                m[1 + 1 * 4] = cos(angle);
            }
            break;
    
        default:
            break;
    }

    *this = old.mul(*this);
}

NIA_CALL niaMatrix4 niaMatrix4::translate(r32 x, r32 y, r32 z){
    niaMatrix4 result;

    result.m[3 + 0 * 4] = x;
    result.m[3 + 1 * 4] = y;
    result.m[3 + 2 * 4] = z;

    return result;
}

NIA_CALL void niaMatrix4::printMat4(const niaMatrix4& mat) {
    for(u8 i = 0; i < 4; ++i){
        for(u8 ii = 0; ii < 4; ++ii){
            printf("%f ", mat.m[ii + i * 4]);
        }
        printf("\n");
    }
}
