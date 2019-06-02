/*
 * File: nia_transform.cpp
 * Project: Nia
 * File created on March-15-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "nia_transform.h"

niaTransform::niaTransform(){
}

niaTransform::~niaTransform(){
}

// TODO add scale on each axis
void niaTransform::scale(r32 amount){
    transform = transform.mul(niaMatrix4::scale(amount));
}

void niaTransform::translate(const niaVector3<r32>& position){
    transform = transform.mul(niaMatrix4::translate(position.x, position.y, position.z));
}

void niaTransform::rotate(r32 amount, niaAxis axis){
    transform = transform.mul(niaMatrix4::rotate(amount, axis));
}

void niaTransform::clear(){
    transform = niaMatrix4();
}

niaMatrix4 niaTransform::getTransformation() const {
    return transform;
}