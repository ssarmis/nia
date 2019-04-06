#include "nia_array.h"

#include <malloc.h>
// TODO add asserts and stuff event if this will not be exposed to the user
template<typename T>
NIA_CALL niaArray<T>::niaArray(){
    preallocatedLimit = 0;
    size = 0;
    data = NULL;
}

template<typename T>
NIA_CALL niaArray<T>::niaArray(u32 limit){
    size = limit;
    data = new T[limit];
    printf("%d %d\n", limit, preallocatedLimit);
    preallocatedLimit = limit;
    printf("%d %d\n", limit, preallocatedLimit);
}

template<typename T>
NIA_CALL niaArray<T>::~niaArray(){
    if(data){
        delete[] data;
    }
}

template<typename T>
NIA_CALL void niaArray<T>::add(T element){
    if(preallocatedLimit){
        data[size - preallocatedLimit] = element;
        --preallocatedLimit;
        return;
    }

    T* tmp;
    tmp = data;
    data = new T[size + 1];
    if(size){
        for(int i = 0; i <= size - 1; ++i){
            data[i] = tmp[i];
        }
        delete[] tmp;
    }
    
    data[size] = element;
    ++size;
}

template<typename T>
NIA_CALL void niaArray<T>::remove(u32 index){ // TODO add handling for a bunch of cases, but I will do that later...
    if(!size || index >= size){
        // ignore it, for now I guess
        return;
    }
    // add case for already reserved space

    T* tmp = data;
    data = new T[size - 1];

    u32 oi = 0;
    u32 ni = 0;

    for(int i = 0; i < size - 1; ++i){
        if(i == index){
            oi++;
        }

        data[ni++] = tmp[oi++];
    }

    --size;
}

template<typename T>
NIA_CALL void niaArray<T>::clean(){
    // TODO have a clean for pointer values, I guess ?
    if(data){
        delete[] data;
        data = NULL;
        size = 0;
    }
}

template<typename T>
NIA_CALL T* niaArray<T>::getData(){
    return data;
}

template<typename T>
NIA_CALL T& niaArray<T>::operator[](u32 index) const {
    // TODO maybe add some handling
    return data[index];
}

template<typename T>
NIA_CALL u32 niaArray<T>::getSize() const{
    return size;
}