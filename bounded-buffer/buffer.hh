#ifndef HEADER_H
#define HEADER_H

#include <stddef.h>
#include <cassert>
#include <cstring>
#include <algorithm>
#include <iostream>


class buffer {
public:
    size_t buff_size;
    size_t head_tag;
    size_t tail_tag;

    size_t len_;

    unsigned char* buf;

public:
    buffer(size_t len);
    buffer(const buffer& other);
    ~buffer();

    inline bool empty() const { return len_ == 0; }
    inline bool full() const { return len_ == buff_size; }
    inline size_t len() const { return len_; }

    size_t read(void* dst, const size_t sz);
    size_t write(const void* src, const size_t sz);
};

#endif