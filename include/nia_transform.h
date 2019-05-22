#ifndef _NIA_TRANSFORM_H_
#define _NIA_TRANSFORM_H_

#include "nia_general.h"

#include "nia_vector.h"
#include "nia_matrix.h"

NIA_CLASS niaTransform {
private:
    niaMatrix4 transform;

public:
    niaTransform();
    ~niaTransform();

    void clear();

    void scale(r32 amount);
    void translate(const niaVector3<r32>& position);
    void rotate(r32 amount, niaAxis axis);

    niaMatrix4 getTransformation() const;
};

#endif // _NIA_TRANSFORM_H_