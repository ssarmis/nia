#include "nia_transform.h"

NIA_CALL niaTransform::niaTransform(){
}

NIA_CALL niaTransform::~niaTransform(){
}

NIA_CALL void niaTransform::scale(r32 amount){
    transform = transform.mul(niaMatrix4::scale(amount));
}

NIA_CALL void niaTransform::translate(const niaVector3<r32>& position){
    transform = transform.mul(niaMatrix4::translate(position.x, position.y, position.z));
}

NIA_CALL void niaTransform::rotate(r32 amount, niaAxis axis){
    transform = transform.mul(niaMatrix4::rotate(amount, axis));
}