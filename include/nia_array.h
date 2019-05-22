#ifndef _NIA_ARRAY_H_
#define _NIA_ARRAY_H_

#include "nia_general.h"

template<typename T>
NIA_CLASS niaArray {
private:
    u32 size;
    i32 preallocatedLimit;
    T* data;

public:
    niaArray();
    niaArray(u32 limit);
    ~niaArray();

    void add(T element);
    void remove(u32 index);
    
    void clean();
    T* getData();
    u32 getSize() const;
    
    T& operator[](u32 index) const;
};

template class niaArray<int>;
template class niaArray<unsigned char>;
template class niaArray<unsigned int>;
template class niaArray<unsigned short>;

#include "nia_vector.h"
template class niaArray<niaVec2i>;
template class niaArray<niaVec2f>;
template class niaArray<niaVec2d>;

template class niaArray<niaVec3i>;
template class niaArray<niaVec3f>;
template class niaArray<niaVec3d>;

template class niaArray<niaVector3<float> >;
template class niaArray<niaVector2<float> >;

template class niaArray<niaVector2<int> >;
template class niaArray<niaVector2<unsigned int> >;

template class niaArray<niaVector3<int> >;
template class niaArray<niaVector3<unsigned int> >;

#include "nia_filter.h"
template class niaArray<niaFilter*>;

#endif // _NIA_ARRAY_H_