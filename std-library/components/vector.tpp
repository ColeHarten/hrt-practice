#include "vector.hh"

using my_std::vector;

template <typename T>
vector<T>::vector() {
    arr = new T[default_size_];
    size_ = 0;
    len_ = default_size_;
}

template <typename T>
vector<T>::vector(size_t len) {
    arr = new T[len*2];
    size_ = len;
    len_ = len*2;
}

template <typename T>
vector<T>::vector(size_t len, T val) {
    arr = new T[len*2];
    size_ = len;
    len_ = len*2;

    for(size_t i = 0; i < len; i++) arr[i] = val;
}

template <typename T>
vector<T>::~vector() {
    delete arr;
}

template <typename T>
void vector<T>::try_shrink_() {
    if(size_ * 2 < len_) {
        len_ = std::max(size_ * 2, default_size_);
        T* tmp = new T[len_];

        memcpy(tmp, arr, sizeof(T) * size_);
        delete arr;
        arr = tmp;
    }
}

template <typename T>
T& vector<T>::operator[](size_t i) const {
    assert(i < size_);
    return arr[i];
}

template <typename T>
void vector<T>::push_back(T val) {
    if(size_ == len_){
        len_ *= 2;
        T* tmp = new T[len_];
        memcpy(tmp, arr, size_*sizeof(T));
        delete arr;
        arr = tmp;
    }
    arr[size_] = val;
    ++size_;
}

template <typename T>
void vector<T>::erase(size_t i) {
    assert(i < size_);

    memmove(&arr[i], &arr[i+1], sizeof(T) * (size_ - i - 1));

    --size_;

    try_shrink_();
}

template <typename T>
void vector<T>::erase(size_t l, size_t r) {
    assert(r >= l);
    assert(r < size_);

    memmove(&arr[l], &arr[r+1], sizeof(T) * (size_ - r - 1));

    size_ -= (r - l + 1);

    try_shrink_();
}

template <typename T>
void vector<T>::empty() {
    size_ = 0;
    
    try_shrink_();

    memset(arr, 0, sizeof(T) * len_);
}

template <typename T>
void vector<T>::insert(size_t at, T val) {
    if(size_ == len_){
        len_ *= 2;
        T* tmp = new T[len_];
        memcpy(tmp, arr, size_*sizeof(T));
        delete arr;
        arr = tmp;
    }

    memmove(&arr[at+1], &arr[at], sizeof(T)*(size_ - at));
    arr[at] = val;
    ++size_;
}