#include "vector.hh"

using my_std::vector;

template <typename T>
vector<T>::vector() {
    arr = new T[defaultSize_];
    size_ = 0;
    len_ = defaultSize_;
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
}

template <typename T>
void vector<T>::erase(size_t l, size_t r) {
    assert(r >= l);
    assert(r < size_);

    memmove(&arr[l], &arr[r+1], sizeof(T) * (size_ - r - 1));

    size_ -= (r - l + 1);

    if(size_ * 2 < len_ && size_ > defaultSize_) {
        T* tmp = new T[size_ * 2];
        memcpy(tmp, arr, sizeof(T) * size_);
        len_ = size_ * 2;
        delete arr;
        arr = tmp;
    }
}
