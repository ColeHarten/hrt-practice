#ifndef ARRAY_H
#define ARRAY_H

#include <cassert>
#include <stdlib.h>

namespace my_std{
template <typename T>
class array {
private:
    T* arr_;
    size_t len_;
public:
    array(size_t len);
    ~array();

    T& operator[](size_t i);

    inline size_t max_size() const { return len_; }
};

#include "array.tpp"

};

#endif