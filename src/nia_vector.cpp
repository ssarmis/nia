/*
 * File: nia_vector.cpp
 * Project: Nia
 * File created on February-28-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "nia_vector.h"

// TODO watch out over those copies
// vector 2

template<typename T>
niaVector2<T>::niaVector2(const T& x, const T& y):
x(x), y(y){
}

template<typename T>
niaVector2<T>::niaVector2(const T* data):
x(data[0]), y(data[1]){
}

template<typename T>
niaVector2<T>::~niaVector2(){
}

template<typename T>
niaVector2<T> niaVector2<T>::add(const niaVector2& other) const{
    return niaVector2<T>(x + other.x, y + other.y);
}

template<typename T>
niaVector2<T> niaVector2<T>::add(const T& t) const{
    return niaVector2<T>(x + t, y + t);
}

template<typename T>
niaVector2<T> niaVector2<T>::sub(const niaVector2& other) const{
    return niaVector2<T>(x - other.x, y - other.y);
}

template<typename T>
niaVector2<T> niaVector2<T>::sub(const T& t) const{
    return niaVector2<T>(x - t, y - t);
}


template<typename T>
niaVector2<T> niaVector2<T>::mul(const niaVector2& other) const{
    return niaVector2<T>(x * other.x, y * other.y);
}

template<typename T>
niaVector2<T> niaVector2<T>::mul(const T& t) const{
    return niaVector2<T>(x * t, y * t);
}

// vector 3

template<typename T>
niaVector3<T>::niaVector3(const T& x, const T& y, const T& z):
x(x), y(y), z(z){
}

template<typename T>
niaVector3<T>::niaVector3(const T* data):
x(data[0]), y(data[1]), z(data[2]){
}

template<typename T>
niaVector3<T>::~niaVector3(){
}

template<typename T>
niaVector3<T> niaVector3<T>::add(const niaVector3& other) const{
    return niaVector3<T>(x + other.x, y + other.y, z + other.z);
}

template<typename T>
niaVector3<T> niaVector3<T>::add(const T& t) const{
    return niaVector3<T>(x + t, y + t, z + t);
}

template<typename T>
niaVector3<T> niaVector3<T>::sub(const niaVector3& other) const{
    return niaVector3<T>(x - other.x, y - other.y, z - other.z);
}

template<typename T>
niaVector3<T> niaVector3<T>::sub(const T& t) const{
    return niaVector3<T>(x - t, y - t, z - t);
}

template<typename T>
niaVector3<T> niaVector3<T>::mul(const niaVector3& other) const{
    return niaVector3<T>(x * other.x, y * other.y, z * other.z);
}

template<typename T>
niaVector3<T> niaVector3<T>::mul(const T& t) const{
    return niaVector3<T>(x * t, y * t, z * t);
}

template<typename T>
niaVector3<T> niaVector3<T>::normal() {
    r32 length = sqrtf(x * x + y * y + z * z);
    if(!length){
        length = 1;
    }
    return niaVector3<T>(x / length, y / length, z / length);
}

template<typename T>
niaVector3<T> niaVector3<T>::cross(const niaVector3& other) const {
    niaVector3<T> result;
    result.x = y * other.z - z * other.y;
    result.y = z * other.x - x * other.z;
    result.z = x * other.y - y * other.x;
    return result;
}

template<typename T>
r32 niaVector3<T>::dot(const niaVector3& other) const{
    r32 leftMagnitute = sqrtf(x * x + y * y + z * z);
    r32 otherMagnitute = sqrtf(other.x * other.x + other.y * other.y + other.z * other.z);

    if(!leftMagnitute){
        leftMagnitute = 1;
    }

    if(!otherMagnitute){
        otherMagnitute = 1;
    }

    r32 product = x * other.x + y * other.y + z * other.z;
    
    r32 cosAngle = product / (leftMagnitute * otherMagnitute);

    return leftMagnitute * otherMagnitute * cosAngle;
}

template<typename T>
r32 niaVector3<T>::dot(const niaVector3& left, const niaVector3& right) {
    return left.dot(right);
}

template<typename T>
bool niaVector3<T>::operator==(const niaVector3& other) const {
    return (x == other.x && y == other.y && z == other.z);
}

// vector 4

template<typename T>
niaVector4<T>::niaVector4(const T& x, const T& y, const T& z, const T& w):
x(x), y(y), z(z), w(w){
}

template<typename T>
niaVector4<T>::niaVector4(const niaVector3<T>& other):
x(other.x), y(other.y), z(other.z), w(1){
}

template<typename T>
niaVector4<T>::~niaVector4(){
}

template<typename T>
niaVector4<T> niaVector4<T>::add(const niaVector4& other) const{
    return niaVector4<T>(x + other.x, y + other.y, z + other.z, w + other.w);
}

template<typename T>
niaVector4<T> niaVector4<T>::add(const T& t) const{
    return niaVector4<T>(x + t, y + t, z + t, w + t);
}

template<typename T>
niaVector4<T> niaVector4<T>::sub(const niaVector4& other) const{
    return niaVector4<T>(x - other.x, y - other.y, z - other.z, w - other.w);
}

template<typename T>
niaVector4<T> niaVector4<T>::sub(const T& t) const{
    return niaVector4<T>(x - t, y - t, z - t, w - t);
}

template<typename T>
niaVector4<T> niaVector4<T>::mul(const niaVector4& other) const{
    return niaVector4<T>(x * other.x, y * other.y, z * other.z, w * other.w);
}

template<typename T>
niaVector4<T> niaVector4<T>::mul(const T& t) const{
    return niaVector4<T>(x * t, y * t, z * t, w * t);
}
