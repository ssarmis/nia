#include <stdio.h>

#include "nia.h"

int main(){
    niaVector3<float> v(3, 2, -2);
    niaVector3<float> w(1, 0, -5);

    printf("%f \n", v.dot(w));
    printf("%f \n", niaVector3<float>::dot(v, w));

    printf("%f %f %f \n",
     v.cross(w).x,
     v.cross(w).y,
     v.cross(w).z);

    printf("%f %f %f \n",
     w.cross(v).x,
     w.cross(v).y,
     w.cross(v).z);

    niaVector3<float> a(90.235, 60.214, 10.148);
    niaVector3<float> b(48987.016478, 12354.1, 50.0002);
    niaVector3<float> c(3, 1, 2);

    printf("%f %f %f \n",
     a.normal().x,
     a.normal().y,
     a.normal().z);

    printf("%f %f %f \n",
     b.normal().x,
     b.normal().y,
     b.normal().z);

    printf("%f %f %f \n",
     c.normal().x,
     c.normal().y,
     c.normal().z);

    niaVector3<r32> vertex(0, 2, 0);
    niaVector3<r32> lightPosition(0, 0, 0);

    niaVector3<r32> lightVector = vertex.sub(lightPosition);
    niaVector3<r32> normal(0, 1, 0);

    float cosTheta = niaVector3<r32>::dot(lightVector.normal(), normal);

    printf("%f \n", cosTheta);
    
    return 0;
}