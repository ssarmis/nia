#ifndef _NIA_VECTOR_H_
#define _NIA_VECTOR_H_

#include "nia_general.h"

// vector 2

NIA_STRUCT niaVec2i {
    union {
        i32 data[2];
        struct {
            i32 x;
            i32 y;
        };
        struct {
            i32 w;
            i32 h;
        };
    };
} niaVec2i;

NIA_STRUCT niaVec2f {
    union {
        r32 data[2];
        struct {
            r32 x;
            r32 y;
        };
        struct {
            r32 w;
            r32 h;
        };
    };
} niaVec2f;

NIA_STRUCT niaVec2d {
    union {
        r64 data[2];
        struct {
            r64 x;
            r64 y;
        };
        struct {
            r64 w;
            r64 h;
        };
    };
} niaVec2d;

// vector 3

NIA_STRUCT niaVec3i {
    union {
        i32 data[3];
        struct {
            i32 x;
            i32 y;
            i32 z;
        };
    };
} niaVec3i;

NIA_STRUCT niaVec3f {
    union {
        r32 data[3];
        struct {
            r32 x;
            r32 y;
            r32 z;
        };
    };
} niaVec3f;

NIA_STRUCT niaVec3d {
    union {
        r64 data[3];
        struct {
            r64 x;
            r64 y;
            r64 z;
        };
    };
} niaVec3d;

template <typename T>
NIA_CLASS niaVector2 {
public:
    union {
        T data[2];
        struct {
            T x;
            T y;
        };
    };
    
    NIA_CALL niaVector2(const T& x = 0, const T& y = 0);
    NIA_CALL niaVector2(const T* data);
    NIA_CALL ~niaVector2();

    NIA_CALL niaVector2 add(const niaVector2& other) const;
    NIA_CALL niaVector2 add(const T& t) const;

    NIA_CALL niaVector2 sub(const niaVector2& other) const;
    NIA_CALL niaVector2 sub(const T& t) const;

    NIA_CALL niaVector2 mul(const niaVector2& other) const;
    NIA_CALL niaVector2 mul(const T& t) const;
};

template <typename T>
NIA_CLASS niaVector3 {
public:
    union {
        T data[3];
        struct {
            T x;
            T y;
            T z;
        };
    };

    NIA_CALL niaVector3(const T& x = 0, const T& y = 0, const T& z = 0);
    NIA_CALL niaVector3(const T* data);
    NIA_CALL ~niaVector3();

    NIA_CALL niaVector3 add(const niaVector3& other) const;
    NIA_CALL niaVector3 add(const T& t) const;

    NIA_CALL niaVector3 sub(const niaVector3& other) const;
    NIA_CALL niaVector3 sub(const T& t) const;
    
    NIA_CALL niaVector3 mul(const niaVector3& other) const;
    NIA_CALL niaVector3 mul(const T& t) const;

    NIA_CALL niaVector3 normal();
    NIA_CALL r32 dot(const niaVector3& other) const;

    NIA_CALL niaVector3 cross(const niaVector3& other) const;
    static NIA_CALL r32 dot(const niaVector3& left, const niaVector3& right);

    NIA_CALL bool operator==(const niaVector3& other) const;
};

template <typename T>
NIA_CLASS niaVector4 {
public:
    T x;
    T y;
    T z;
    T w;
    
    NIA_CALL niaVector4(const T& x = 0, const T& y = 0, const T& z = 0, const T& w = 0);
    NIA_CALL ~niaVector4();

    NIA_CALL niaVector4 add(const niaVector4& other) const;
    NIA_CALL niaVector4 add(const T& t) const;

    NIA_CALL niaVector4 sub(const niaVector4& other) const;
    NIA_CALL niaVector4 sub(const T& t) const;

    NIA_CALL niaVector4 mul(const niaVector4& other) const;
    NIA_CALL niaVector4 mul(const T& t) const;
};

// vector 2
template class niaVector2<char>;
template class niaVector2<unsigned char>;

template class niaVector2<short>;
template class niaVector2<unsigned short>;

template class niaVector2<int>;
template class niaVector2<unsigned int>;

template class niaVector2<long>;
template class niaVector2<unsigned long>;
template class niaVector2<long long>;
template class niaVector2<unsigned long long>;

template class niaVector2<float>;
template class niaVector2<double>;
template class niaVector2<long double>;

// vector 3
template class niaVector3<char>;
template class niaVector3<unsigned char>;

template class niaVector3<short>;
template class niaVector3<unsigned short>;

template class niaVector3<int>;
template class niaVector3<unsigned int>;

template class niaVector3<long>;
template class niaVector3<unsigned long>;
template class niaVector3<long long>;
template class niaVector3<unsigned long long>;

template class niaVector3<float>;
template class niaVector3<double>;
template class niaVector3<long double>;

// vector 4
template class niaVector4<char>;
template class niaVector4<unsigned char>;

template class niaVector4<short>;
template class niaVector4<unsigned short>;

template class niaVector4<int>;
template class niaVector4<unsigned int>;

template class niaVector4<long>;
template class niaVector4<unsigned long>;
template class niaVector4<long long>;
template class niaVector4<unsigned long long>;

template class niaVector4<float>;
template class niaVector4<double>;
template class niaVector4<long double>;

#endif