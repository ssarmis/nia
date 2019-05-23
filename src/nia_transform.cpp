#include "nia_transform.h"

niaTransform::niaTransform(){
}

niaTransform::~niaTransform(){
}

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