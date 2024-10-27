#ifndef HEADER_H
#define HEADER_H

#include <stddef.h>
#include <cassert>
#include <cstring>
#include <algorithm>
#include <iostream>

#include <mutex>
#include <atomic>
#include <condition_variable>

class buffer {
private:
    std::mutex m_;
    std::condition_variable_any full_cv_;
    std::condition_variable_any empty_cv_;

    size_t buff_size;
    size_t head_tag;
    size_t tail_tag;

    std::atomic<size_t> size_;

    unsigned char* buf;

    // private functions that don't claim mutex
    size_t read_(void* dst, const size_t sz);
    size_t write_(const void* src, const size_t sz);

public:
    buffer(size_t len);
    buffer(const buffer& other);
    ~buffer();

    bool empty() const {return size_ == 0; }
    bool full() const { return size_ == buff_size; }
    size_t len() const {  return size_; }

    // read/write up to `sz` bytes and return number read
    size_t read(void* dst, const size_t sz);
    size_t write(const void* src, const size_t sz);

    // block until able to read/write `sz` bytes
    bool block_read(void* dst, const size_t sz);
    bool block_write(const void* src, const size_t sz);

    unsigned char operator[](int i) const {return buf[i];}
};

#endif