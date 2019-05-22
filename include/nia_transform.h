#ifndef _NIA_TRANSFORM_H_
#define _NIA_TRANSFORM_H_

#include "nia_general.h"

#include "nia_vector.h"
#include "nia_matrix.h"

NIA_CLASS niaTransform {
private:
    niaMatrix4 transform;

public:
    NIA_CALL niaTransform();
    NIA_CALL ~niaTransform();

    void NIA_CALL clear();

    void NIA_CALL scale(r32 amount);
    void NIA_CALL translate(const niaVector3<r32>& position);
    void NIA_CALL rotate(r32 amount, niaAxis axis);

    niaMatrix4 NIA_CALL getTransformation() const;
};

#endif // _NIA_TRANSFORM_H_