/*
 * File: nia_array.h
 * Project: Nia
 * File created on March-16-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

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
    NIA_CALL niaArray();
    NIA_CALL niaArray(u32 limit);
    NIA_CALL ~niaArray();

    void NIA_CALL add(T element);
    void NIA_CALL remove(u32 index);
    
    void NIA_CALL clean();
    T* NIA_CALL getData();
    u32 NIA_CALL getSize() const;
    
    T& NIA_CALL operator[](u32 index) const;
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

#include "nia_light.h"
template class niaArray<niaLight>;

#include "nia_vertex.h"
template class niaArray<niaVertex>;

#endif // _NIA_ARRAY_H_