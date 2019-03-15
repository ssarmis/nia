#ifndef _NIA_TRANSFORM_H_
#define _NIA_TRANSFORM_H_

#include "nia_general.h"

#include "nia_vector.h"
#include "nia_matrix.h"

NIA_CLASS niaTransform {
private:

public:
    niaMatrix4 transform;//TODO make private
    NIA_CALL niaTransform();
    NIA_CALL ~niaTransform();

    NIA_CALL void scale(r32 amount);
    NIA_CALL void translate(const niaVector3<r32>& position);
    NIA_CALL void rotate(r32 amount, niaAxis axis);
};

#endif // _NIA_TRANSFORM_H_