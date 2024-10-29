#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <cstring>
#include <algorithm>

namespace my_std {

template <typename T>
class vector {
private:
    static constexpr size_t default_size_ = 10;
    size_t len_;
    size_t size_;
    T* arr;

    void try_shrink_();
public:
    vector();
    vector(size_t len);
    vector(size_t len, T val);

    ~vector();
    
    inline size_t size() const { return size_; }
    inline size_t max_size() const { return len_; }
    inline const T* data() const { return arr; }
    T& operator[](size_t i) const;

    void push_back(T val);
    void erase(size_t i);
    void erase(size_t l, size_t r);

    void empty();

    void insert(size_t at, T val);
};

#include "vector.tpp"

};

#endif