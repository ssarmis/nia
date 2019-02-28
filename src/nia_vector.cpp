#include "nia_vector.h"

// TODO watch out over those copies
// vector 2

template<typename T>
NIA_CALL niaVector2<T>::niaVector2(const T& x, const T& y):
x(x), y(y){
}

template<typename T>
NIA_CALL niaVector2<T>::~niaVector2(){
}

template<typename T>
NIA_CALL niaVector2<T> niaVector2<T>::add(const niaVector2& other) const{
    return niaVector2<T>(x + other.x, y + other.y);
}

template<typename T>
NIA_CALL niaVector2<T> niaVector2<T>::add(const T& t) const{
    return niaVector2<T>(x + t, y + t);
}

template<typename T>
NIA_CALL niaVector2<T> niaVector2<T>::sub(const niaVector2& other) const{
    return niaVector2<T>(x - other.x, y - other.y);
}

template<typename T>
NIA_CALL niaVector2<T> niaVector2<T>::sub(const T& t) const{
    return niaVector2<T>(x - t, y - t);
}


template<typename T>
NIA_CALL niaVector2<T> niaVector2<T>::mul(const niaVector2& other) const{
    return niaVector2<T>(x * other.x, y * other.y);
}

template<typename T>
NIA_CALL niaVector2<T> niaVector2<T>::mul(const T& t) const{
    return niaVector2<T>(x * t, y * t);
}

// vector 3

template<typename T>
NIA_CALL niaVector3<T>::niaVector3(const T& x, const T& y, const T& z):
x(x), y(y), z(z){
}

template<typename T>
NIA_CALL niaVector3<T>::~niaVector3(){
}

template<typename T>
NIA_CALL niaVector3<T> niaVector3<T>::add(const niaVector3& other) const{
    return niaVector3<T>(x + other.x, y + other.y, z + other.z);
}

template<typename T>
NIA_CALL niaVector3<T> niaVector3<T>::add(const T& t) const{
    return niaVector3<T>(x + t, y + t, z + t);
}

template<typename T>
NIA_CALL niaVector3<T> niaVector3<T>::sub(const niaVector3& other) const{
    return niaVector3<T>(x - other.x, y - other.y, z - other.z);
}

template<typename T>
NIA_CALL niaVector3<T> niaVector3<T>::sub(const T& t) const{
    return niaVector3<T>(x - t, y - t, z - t);
}

template<typename T>
NIA_CALL niaVector3<T> niaVector3<T>::mul(const niaVector3& other) const{
    return niaVector3<T>(x * other.x, y * other.y, z * other.z);
}

template<typename T>
NIA_CALL niaVector3<T> niaVector3<T>::mul(const T& t) const{
    return niaVector3<T>(x * t, y * t, z * t);
}

// vector 4

template<typename T>
NIA_CALL niaVector4<T>::niaVector4(const T& x, const T& y, const T& z, const T& w):
x(x), y(y), z(z), w(w){
}

template<typename T>
NIA_CALL niaVector4<T>::~niaVector4(){
}

template<typename T>
NIA_CALL niaVector4<T> niaVector4<T>::add(const niaVector4& other) const{
    return niaVector4<T>(x + other.x, y + other.y, z + other.z, w + other.w);
}

template<typename T>
NIA_CALL niaVector4<T> niaVector4<T>::add(const T& t) const{
    return niaVector4<T>(x + t, y + t, z + t, w + t);
}

template<typename T>
NIA_CALL niaVector4<T> niaVector4<T>::sub(const niaVector4& other) const{
    return niaVector4<T>(x - other.x, y - other.y, z - other.z, w - other.w);
}

template<typename T>
NIA_CALL niaVector4<T> niaVector4<T>::sub(const T& t) const{
    return niaVector4<T>(x - t, y - t, z - t, w - t);
}

template<typename T>
NIA_CALL niaVector4<T> niaVector4<T>::mul(const niaVector4& other) const{
    return niaVector4<T>(x * other.x, y * other.y, z * other.z, w * other.w);
}

template<typename T>
NIA_CALL niaVector4<T> niaVector4<T>::mul(const T& t) const{
    return niaVector4<T>(x * t, y * t, z * t, w * t);
}
