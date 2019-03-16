#include "nia_matrix.h"

#include <stdio.h>

#include <xmmintrin.h>

// mat 4
NIA_CALL niaMatrix4::niaMatrix4(r32 data){
    // nia_memset((u8*)m, 0, MAT_4_SIZE * sizeof(r32));
    // unrolling this is faster

    _mm_store_ps(m, _mm_set1_ps(0));
    _mm_store_ps(m + 4, _mm_set1_ps(0));
    _mm_store_ps(m + 8, _mm_set1_ps(0));
    _mm_store_ps(m + 12, _mm_set1_ps(0));

    m[0] = data;
    m[5] = data;
    m[10] = data;
    m[15] = data;
}

NIA_CALL niaMatrix4::~niaMatrix4(){
}

NIA_CALL niaMatrix4 niaMatrix4::add(r32 number) const{
    niaMatrix4 result = *this;

    _mm_store_ps(result.m, _mm_add_ps(_mm_load_ps(result.m), _mm_set1_ps(number)));
    _mm_store_ps(result.m + 4, _mm_add_ps(_mm_load_ps(result.m + 4), _mm_set1_ps(number)));
    _mm_store_ps(result.m + 8, _mm_add_ps(_mm_load_ps(result.m + 8), _mm_set1_ps(number)));
    _mm_store_ps(result.m + 12, _mm_add_ps(_mm_load_ps(result.m + 12), _mm_set1_ps(number)));

    return result;
}

NIA_CALL niaMatrix4 niaMatrix4::add(const niaMatrix4& other) const{
    niaMatrix4 result = *this;

    _mm_store_ps(result.m, _mm_add_ps(_mm_load_ps(result.m), _mm_load_ps(other.m)));
    _mm_store_ps(result.m + 4, _mm_add_ps(_mm_load_ps(result.m + 4), _mm_load_ps(other.m + 4)));
    _mm_store_ps(result.m + 8, _mm_add_ps(_mm_load_ps(result.m + 8), _mm_load_ps(other.m + 8)));
    _mm_store_ps(result.m + 12, _mm_add_ps(_mm_load_ps(result.m + 12), _mm_load_ps(other.m + 12)));

    return result;
}

NIA_CALL niaMatrix4 niaMatrix4::sub(r32 number) const{
    niaMatrix4 result = *this;

    _mm_store_ps(result.m, _mm_sub_ps(_mm_load_ps(result.m), _mm_set1_ps(number)));
    _mm_store_ps(result.m + 4, _mm_sub_ps(_mm_load_ps(result.m + 4), _mm_set1_ps(number)));
    _mm_store_ps(result.m + 8, _mm_sub_ps(_mm_load_ps(result.m + 8), _mm_set1_ps(number)));
    _mm_store_ps(result.m + 12, _mm_sub_ps(_mm_load_ps(result.m + 12), _mm_set1_ps(number)));

    return result;
}

NIA_CALL niaMatrix4 niaMatrix4::sub(const niaMatrix4& other) const{
    niaMatrix4 result = *this;

    _mm_store_ps(result.m, _mm_sub_ps(_mm_load_ps(result.m), _mm_load_ps(other.m)));
    _mm_store_ps(result.m + 4, _mm_sub_ps(_mm_load_ps(result.m + 4), _mm_load_ps(other.m + 4)));
    _mm_store_ps(result.m + 8, _mm_sub_ps(_mm_load_ps(result.m + 8), _mm_load_ps(other.m + 8)));
    _mm_store_ps(result.m + 12, _mm_sub_ps(_mm_load_ps(result.m + 12), _mm_load_ps(other.m + 12)));

    return result;
}

NIA_CALL niaMatrix4 niaMatrix4::mul(r32 number) const {
    niaMatrix4 result = *this;

    _mm_store_ps(result.m, _mm_mul_ps(_mm_load_ps(result.m), _mm_set1_ps(number)));
    _mm_store_ps(result.m + 4, _mm_mul_ps(_mm_load_ps(result.m + 4), _mm_set1_ps(number)));
    _mm_store_ps(result.m + 8, _mm_mul_ps(_mm_load_ps(result.m + 8), _mm_set1_ps(number)));
    _mm_store_ps(result.m + 12, _mm_mul_ps(_mm_load_ps(result.m + 12), _mm_set1_ps(number)));

    return result;
}

NIA_CALL niaMatrix4 niaMatrix4::mul(const niaMatrix4& other) const{
    niaMatrix4 result = niaMatrix4(0);

    for(int i = 0; i < 4; ++i){ // TODO unroll this for eficiency
        const __m128 r0 = {m[0 + i * 4], m[1 + i * 4], m[2 + i * 4], m[3 + i * 4]};
        const __m128 c0 = {other.m[0 + 0 * 4], other.m[0 + 1 * 4], other.m[0 + 2 * 4], other.m[0 + 3 * 4]};
        const __m128 c1 = {other.m[1 + 0 * 4], other.m[1 + 1 * 4], other.m[1 + 2 * 4], other.m[1 + 3 * 4]};
        const __m128 c2 = {other.m[2 + 0 * 4], other.m[2 + 1 * 4], other.m[2 + 2 * 4], other.m[2 + 3 * 4]};
        const __m128 c3 = {other.m[3 + 0 * 4], other.m[3 + 1 * 4], other.m[3 + 2 * 4], other.m[3 + 3 * 4]};
        
        const __m128 f0 = r0 * c0;
        const __m128 f1 = r0 * c1;
        const __m128 f2 = r0 * c2;
        const __m128 f3 = r0 * c3;

        r32 f00 = f0[0] + f0[1] + f0[2] + f0[3];
        r32 f10 = f1[0] + f1[1] + f1[2] + f1[3];
        r32 f20 = f2[0] + f2[1] + f2[2] + f2[3];
        r32 f30 = f3[0] + f3[1] + f3[2] + f3[3];

        const __m128 tmp = {f00, f10, f20, f30};

        _mm_store_ps(result.m + i * sizeof(r32), tmp);
    }

    return result;
}

NIA_CALL niaMatrix4 niaMatrix4::scale(r32 amount){
    niaMatrix4 result(amount);
    result.m[15] = 1;
    return result;
}

NIA_CALL niaMatrix4 niaMatrix4::rotate(r32 angle, niaAxis axis){
    niaMatrix4 result(1);

    switch (axis) {
        case NIA_AXIS_X:{
                result.m[5] = cos(angle);
                result.m[6] = sin(angle);
                result.m[9] = -sin(angle);
                result.m[10] = cos(angle);
            }
            break;

        case NIA_AXIS_Y:{
                result.m[0] = cos(angle);
                result.m[2] = -sin(angle);
                result.m[8] = sin(angle);
                result.m[10] = cos(angle);
            }
            break;

        case NIA_AXIS_Z:{
                result.m[0] = cos(angle);
                result.m[1] = -sin(angle);
                result.m[4] = sin(angle);
                result.m[5] = cos(angle);
            }
            break;
    
        default:
            break;
    }


    return result;
}

NIA_CALL niaMatrix4 niaMatrix4::perspective(r32 fov, r32 aspectRatio, r32 near, r32 far){
    niaMatrix4 result(0.0);
    result.m[0] = nia_cot(fov / 2) / aspectRatio;
    result.m[5] = nia_cot(fov / 2);
    
    result.m[10] = far / (far - near);
    result.m[11] = - ((far * near) / (far - near));

    result.m[14] = 1;

    return result;
}

NIA_CALL niaMatrix4 niaMatrix4::orthographic(r32 left, r32 right, r32 top, r32 bottom, r32 near, r32 far){
    niaMatrix4 result(1.0);

    result.m[0] = 2.0 / (right - left);
    result.m[5] = 2.0 / (top - bottom);
    result.m[10] = -2.0 / (far - near);
    result.m[11] = -(far + near) / (far - near);
    
    result.m[7] = -(top + bottom) / (top - bottom);
    result.m[3] = -(right + left) / (right - left);
    
    return result;
}

NIA_CALL niaMatrix4 niaMatrix4::identity(){
    return niaMatrix4(1.0);
}

NIA_CALL void niaMatrix4::add_self(r32 number){
    _mm_store_ps(m, _mm_add_ps(_mm_load_ps(m), _mm_set1_ps(number)));
    _mm_store_ps(m + 4, _mm_add_ps(_mm_load_ps(m + 4), _mm_set1_ps(number)));
    _mm_store_ps(m + 8, _mm_add_ps(_mm_load_ps(m + 8), _mm_set1_ps(number)));
    _mm_store_ps(m + 12, _mm_add_ps(_mm_load_ps(m + 12), _mm_set1_ps(number)));
}

NIA_CALL void niaMatrix4::sub_self(r32 number){
    _mm_store_ps(m, _mm_sub_ps(_mm_load_ps(m), _mm_set1_ps(number)));
    _mm_store_ps(m + 4, _mm_sub_ps(_mm_load_ps(m + 4), _mm_set1_ps(number)));
    _mm_store_ps(m + 8, _mm_sub_ps(_mm_load_ps(m + 8), _mm_set1_ps(number)));
    _mm_store_ps(m + 12, _mm_sub_ps(_mm_load_ps(m + 12), _mm_set1_ps(number)));
}

NIA_CALL void niaMatrix4::mul_self(r32 number){
    _mm_store_ps(m, _mm_mul_ps(_mm_load_ps(m), _mm_set1_ps(number)));
    _mm_store_ps(m + 4, _mm_mul_ps(_mm_load_ps(m + 4), _mm_set1_ps(number)));
    _mm_store_ps(m + 8, _mm_mul_ps(_mm_load_ps(m + 8), _mm_set1_ps(number)));
    _mm_store_ps(m + 12, _mm_mul_ps(_mm_load_ps(m + 12), _mm_set1_ps(number)));
}

NIA_CALL void niaMatrix4::scale_self(r32 amount){
    // niaMatrix4 old = *this;

    m[0] = amount;
    m[5] = amount;
    m[10] = amount;

    // *this = old.mul(*this);
}

NIA_CALL void niaMatrix4::rotate_self(r32 angle, niaAxis axis){
    switch (axis) {
        case NIA_AXIS_X:{
                m[5] = cos(angle);
                m[6] = sin(angle);
                m[9] = -sin(angle);
                m[10] = cos(angle);
            }
            break;

        case NIA_AXIS_Y:{
                m[0] = cos(angle);
                m[2] = -sin(angle);
                m[8] = sin(angle);
                m[10] = cos(angle);
            }
            break;

        case NIA_AXIS_Z:{
                m[0] = cos(angle);
                m[1] = -sin(angle);
                m[4] = sin(angle);
                m[5] = cos(angle);
            }
            break;
    
        default:
            break;
    }
}

NIA_CALL niaMatrix4 niaMatrix4::translate(r32 x, r32 y, r32 z){
    niaMatrix4 result;

    result.m[3] = x;
    result.m[7] = y;
    result.m[11] = z;

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
