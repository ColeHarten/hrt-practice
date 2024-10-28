#include "array.hh"

template <typename T>
array<T>::array(size_t len) {
    assert(len > 0);

    len_ = len;
    arr_ = new T[len];
}


template <typename T>
array<T>::~array() {
    delete[] arr_;
}

template <typename T>
T& array<T>::operator[](size_t i) {
    assert(i < len_);
    
    return arr_[i];
}