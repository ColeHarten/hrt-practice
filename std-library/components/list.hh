#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

template <typename T>
struct node {
    node *next;
    node *prev;
    T val;
};

namespace my_std {

template <typename T>
class list {
private:
    size_t size_;
    node<T> *head_;
    node<T> *tail_;

public:
    list();
    ~list();

    void push_back(T val);

    inline size_t size() const { return size_; } 
    T& operator[](size_t index) const;

    void insert(size_t at, T val);

    T pop_front();
    T pop_back();

};

#include "list.tpp"

};

#endif