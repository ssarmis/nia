#include "nia_matrix.h"

#include <stdio.h>

#include <xmmintrin.h>

#include "nia_constants.h"

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
        
        const __m128 f0 = _mm_mul_ps(r0, c0);
        const __m128 f1 = _mm_mul_ps(r0, c1);
        const __m128 f2 = _mm_mul_ps(r0, c2);
        const __m128 f3 = _mm_mul_ps(r0, c3);

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

    r32 fovRad = fov * 180.0 / NIA_PI;

    result.m[0] = nia_cot(fovRad / 2.0) / aspectRatio;
    result.m[5] = nia_cot(fovRad / 2.0);
    
    result.m[10] = (far + near) / (far - near);
    result.m[11] = - ((2 * far * near) / (far - near));

    result.m[14] = 1;

    // float d2r = NIA_PI / 180.0;
    // float ys = 1.0 / tanf(d2r * fov / 2);
    // float xs = ys / aspectRatio;
    // result.m[0 + 0 * 4] = xs;
    // result.m[1 + 1 * 4] = ys;
    // result.m[2 + 2 * 4] = (far + near) / (near - far);
    // result.m[3 + 2 * 4] = 1;
    // result.m[2 + 3 * 4] = (2 * far * near) / (near - far);

    // result.m[0 + 0 * 4] = 1.0 / (aspectRatio * tanf(((fov / 2.0) * 180 / NIA_PI)));
    // result.m[1 + 1 * 4] = 1.0 / tanf(((fov / 2.0) * 180 / NIA_PI));
    // result.m[2 + 2 * 4] = (-(near - far)) / (near - far);
    // result.m[3 + 2 * 4] = (2.0 * near * far) / (near - far);
    // result.m[2 + 3 * 4] = 1.0;

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

NIA_CALL bool niaMatrix4::compareMat4(const niaMatrix4& left, const niaMatrix4& right){
    for(u8 i = 0; i < MAT_4_SIZE; ++i){
        if (left.m[i] != right.m[i]){
            return false;
        }
    }
#if 0
    __m128 r0 = _mm_load_ps(left.m);
    __m128 r1 = _mm_load_ps(left.m + 4);
    __m128 r2 = _mm_load_ps(left.m + 8);
    __m128 r3 = _mm_load_ps(left.m + 12);

    __m128 o0 = _mm_load_ps(right.m);
    __m128 o1 = _mm_load_ps(right.m + 4);
    __m128 o2 = _mm_load_ps(right.m + 8);
    __m128 o3 = _mm_load_ps(right.m + 12);
#endif

#if 0
    __m128i tmp = _mm_cmpeq_epi32((__m128i)r0, (__m128i)o0);
    result = (tmp[0] != 0) && (tmp[1] != 0) && (tmp[2] != 0) && (tmp[3] != 0) && result;
    printf("%d\n", result);
    printf("%ul %ul %ul %ul\n", tmp[0], tmp[1], tmp[2], tmp[3]);

    tmp =_mm_cmpeq_epi32((__m128i)r1, (__m128i)o1);
    result = (tmp[0] != 0) && (tmp[1] != 0) && (tmp[2] != 0) && (tmp[3] != 0) && result;
    printf("%d\n", result);
    printf("%ul %ul %ul %ul\n", tmp[0], tmp[1], tmp[2], tmp[3]);

    tmp =_mm_cmpeq_epi32((__m128i)r2, (__m128i)o2);
    result = (tmp[0] != 0) && (tmp[1] != 0) && (tmp[2] != 0) && (tmp[3] != 0) && result;
    printf("%d\n", result);
    printf("%ul %ul %ul %ul\n", tmp[0], tmp[1], tmp[2], tmp[3]);

    tmp =_mm_cmpeq_epi32((__m128i)r3, (__m128i)o3);
    result = (tmp[0] != 0) && (tmp[1] != 0) && (tmp[2] != 0) && (tmp[3] != 0) && result;
    printf("%d\n", result);
    printf("%ul %ul %ul %ul\n", tmp[0], tmp[1], tmp[2], tmp[3]);
#endif

#if 0
    __m128i tmp = _mm_cmpeq_epi32((__m128i)r0, (__m128i)o0);
    result = ((tmp[0] != 0) && (tmp[1] != 0) && (tmp[2] != 0) && (tmp[3] != 0)) && result;

    tmp = _mm_cmpeq_epi32((__m128i)r1, (__m128i)o1);
    result = ((tmp[0] != 0) && (tmp[1] != 0) && (tmp[2] != 0) && (tmp[3] != 0)) && result;

    tmp = _mm_cmpeq_epi32((__m128i)r2, (__m128i)o3);
    result = ((tmp[0] != 0) && (tmp[1] != 0) && (tmp[2] != 0) && (tmp[3] != 0)) && result;

    tmp = _mm_cmpeq_epi32((__m128i)r3, (__m128i)o3);
    result = ((tmp[0] != 0) && (tmp[1] != 0) && (tmp[2] != 0) && (tmp[3] != 0)) && result;
#endif
    return true;
}

NIA_CALL void niaMatrix4::printMat4(const niaMatrix4& mat) {
    for(u8 i = 0; i < 4; ++i){
        for(u8 ii = 0; ii < 4; ++ii){
            printf("%f ", mat.m[ii + i * 4]);
        }
        printf("\n");
    }
}

NIA_CALL niaMatrix4 niaMatrix4::lookAt(const niaVector3<r32>& position, const niaVector3<r32>& target, const niaVector3<r32>& up){
    niaMatrix4 result(0.0);

    niaVector3<r32> zx = position.sub(target).normal();
    niaVector3<r32> xx = up.cross(zx).normal();
    niaVector3<r32> yx = zx.cross(xx);

    result.m[0] = xx.x;
    result.m[4] = xx.y;
    result.m[8] = xx.z;

    result.m[1] = yx.x;
    result.m[5] = yx.y;
    result.m[9] = yx.z;

    result.m[2] = zx.x;
    result.m[6] = zx.y;
    result.m[10] = zx.z;

    result.m[12] = -niaVector3<r32>::dot(xx, position);
    result.m[13] = -niaVector3<r32>::dot(yx, position);
    result.m[14] = -niaVector3<r32>::dot(zx, position);

    result.m[15] = 1.0;

    return result;
}
