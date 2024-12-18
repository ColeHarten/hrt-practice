#include "buffer.hh"

buffer::buffer(size_t len) {
    assert(len > 0);

    buff_size = len;
    tail_tag = 0;
    head_tag = 0;

    size_ = 0;

    buf = new unsigned char[len];
}

// define copy constructor to prevent double free errors the underlying buffer is replicated
buffer::buffer(const buffer& other) {
    buff_size = other.buff_size;
    tail_tag = other.tail_tag;
    head_tag = other.head_tag;

    size_ = other.size_.load();

    buf = new unsigned char[other.buff_size];

    memcpy(buf, other.buf, buff_size);
}

buffer::~buffer() {
    delete[] buf;
}

size_t buffer::read_(void* dst, size_t sz) {
    assert(head_tag <= buff_size);
    assert(tail_tag <= buff_size);

    if(sz == 0 || this->empty()) {
        return 0;
    }

    size_t nread = 0;

    if(tail_tag < head_tag) {
        size_t numToRead = std::min(sz, head_tag-tail_tag);
        memcpy(dst, (unsigned char*)buf+tail_tag,numToRead);
        memset((unsigned char*)buf+tail_tag, 0xcc, numToRead);
        nread += numToRead;
        tail_tag = (tail_tag + numToRead) % buff_size;

    } else {
        size_t numToRead = std::min(sz, buff_size-tail_tag);
        if(numToRead > 0) {
            memcpy(dst, (unsigned char*)buf+tail_tag, numToRead);
            memset((unsigned char*)buf+tail_tag, 0xcc, numToRead);
            nread += numToRead;
            tail_tag = (tail_tag + numToRead) % buff_size;
        }

        if(nread < sz && head_tag > 0) {
            // if we have more bytes to read and that are available
            numToRead = std::min(head_tag, sz-nread);
            memcpy((unsigned char*)dst+nread, (unsigned char*)buf, numToRead);
            memset((unsigned char*)buf, 0xcc, numToRead);
            nread += numToRead;
            tail_tag = numToRead;
        }
    }

    assert(head_tag <= buff_size);
    assert(tail_tag <= buff_size);

    size_ -= nread;
    return nread;
}

size_t buffer::write_(const void* src, size_t sz) {
    assert(head_tag <= buff_size);
    assert(tail_tag <= buff_size);

    // buffer is full
    if(this->full()) {
        return 0;
    }

    size_t nwritten = 0;

    if(tail_tag <= head_tag){
        // first write from head_tag up to the tail of the array 
        size_t numToWrite = std::min(buff_size - head_tag, sz);
        if(numToWrite > 0){
            memcpy((unsigned char*)buf + head_tag, src, numToWrite);
            nwritten += numToWrite;
            head_tag = (head_tag + nwritten) % buff_size;
        }
    
        // if there is space being looped around to at the "beginning" of the circular buffer 
        if(nwritten < sz && tail_tag > 0) {
            numToWrite = std::min(sz-nwritten, tail_tag);
            memcpy(buf, (unsigned char*)src + nwritten, numToWrite);
            nwritten += numToWrite;
            head_tag = numToWrite;
        }
    } else {
        size_t numToWrite = std::min(sz, tail_tag-head_tag);
        memcpy((unsigned char*)buf+head_tag, src, numToWrite);
        nwritten += numToWrite;
        head_tag = (head_tag + numToWrite) % buff_size;
    }

    assert(head_tag <= buff_size);
    assert(tail_tag <= buff_size);   

    size_ += nwritten;

    return nwritten;
}

size_t buffer::read(void* dst, size_t sz) {
    std::unique_lock<std::mutex> guard(m_);
    return read_(dst, sz);
}

size_t buffer::write(const void* src, size_t sz) {
    std::unique_lock<std::mutex> guard(m_);
    return write_(src, sz);
}

bool buffer::block_read(void* dst, const size_t sz) {
    assert(sz < buff_size);

    std::unique_lock<std::mutex> guard(m_);

    // while there are not `sz` bytes in the buffer, sleep
    while(size_ < sz) {
        empty_cv_.wait(guard);
    }
    
    size_t nread = read_(dst, sz);
    assert(nread == sz);

    guard.unlock();
    full_cv_.notify_all();

    return true;
}

bool buffer::block_write(const void* src, const size_t sz) {
    assert(sz < buff_size);
    std::unique_lock<std::mutex> guard(m_);
    // while there is not enough space in buffer, sleep
    while(buff_size - size_ < sz) {
        full_cv_.wait(guard);
    }

    size_t nwrite = write_(src, sz);
    assert(nwrite == sz);

    guard.unlock();
    empty_cv_.notify_all();

    return true;
}