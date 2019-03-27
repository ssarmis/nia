#include "nia_array.h"

#include <malloc.h>
// TODO add asserts and stuff event if this will not be exposed to the user
template<typename T>
NIA_CALL niaArray<T>::niaArray(){
    size = 0;
    data = NULL;
}

template<typename T>
NIA_CALL niaArray<T>::~niaArray(){
    if(data){
        delete[] data;
    }
}

template<typename T>
NIA_CALL void niaArray<T>::add(T element){
    T* tmp;
    if(data){
        tmp = data;
    
        data = new T[size + 1];

        for(int i = 0; i <= size - 1; ++i){
            data[i] = tmp[i];
        }
        delete[] tmp;
    } else {
        data = new T[size + 1];
    }

    data[size] = element;
    ++size;
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