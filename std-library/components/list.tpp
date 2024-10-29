#include "list.hh"

using my_std::list;

template <typename T>
list<T>::list() {
    size_ = 0;
    head_ = nullptr;
    tail_ = nullptr;
}

template <typename T>
list<T>::~list() {
    while (head_ != nullptr) {
        node<T> *tmp = head_;
        head_ = head_->next;
        delete tmp;
    }
}

template <typename T>
void list<T>::push_back(T val) {
    node<T> *n = new node<T>;
    n->val = val;
    n->next = nullptr;
    n->prev = tail_;

    if (!head_) {
        head_ = n;
        tail_ = n;
    } else {
        tail_->next = n;
        tail_ = n;
    }

    ++size_;
}


template <typename T>
T& list<T>::operator[](size_t index) const {
    assert(index < size_);
    
    if(index <= size_ - index) {

        node<T> *n = head_;

        for(size_t j = 0; j < index; ++j) {
            n = n->next;
        }

        return n->val;
    } else {
        node<T> *n = tail_;

        for(size_t j = size_-1; j != index; j--) {
            n = n->prev;
        }
        
        return n->val;
    }
}

template <typename T>
void list<T>::insert(size_t at, T val) {
    assert(at <= size_);

    if (at == size_) {  // Insert at the end
        node<T> *n = new node<T>;
        n->next = nullptr;
        n->prev = tail_;
        n->val = val;

        if (tail_) {
            tail_->next = n;
        } else {  // If list was empty, set head_
            head_ = n;
        }
        tail_ = n;
    } else if (at == 0) {  // Insert at the beginning
        node<T> *n = new node<T>;
        n->val = val;
        n->prev = nullptr;
        n->next = head_;

        if (head_) {
            head_->prev = n;
        } else {  // If list was empty, set tail_
            tail_ = n;
        }
        head_ = n;
    } else if (at <= size_ / 2) {  // Insert closer to the head
        node<T> *n = head_;
        for (size_t j = 0; j < at; ++j) {
            n = n->next;
        }

        node<T> *tmp = new node<T>;
        tmp->val = val;
        tmp->prev = n->prev;
        tmp->next = n;
        n->prev->next = tmp;
        n->prev = tmp;
    } else {  // Insert closer to the tail
        node<T> *n = tail_;
        for (size_t j = size_ - 1; j > at; --j) {
            n = n->prev;
        }

        node<T> *tmp = new node<T>;
        tmp->val = val;
        tmp->prev = n->prev;
        tmp->next = n;
        n->prev->next = tmp;
        n->prev = tmp;
    }

    ++size_;
}


template <typename T>
T list<T>::pop_front() {
    assert(size_ > 0);

    if(size_ == 1) {
        T val = head_->val;

        delete head_;
        head_ = nullptr;
        tail_ = nullptr;
        size_ = 0;

        return val;
    }

    node<T>* newHead = head_->next;
    newHead->prev = nullptr;

    T val = head_->val;
    
    delete head_;
    head_ = newHead;

    --size_;
    return val;
}

template <typename T>
T list<T>::pop_back() {
    assert(size_ > 0);

    if(size_ == 1) {
        T val = head_->val;

        delete head_;
        head_ = nullptr;
        tail_ = nullptr;
        size_ = 0;

        return val;
    }

    node<T>* newTail = tail_->prev;
    newTail->next = nullptr;

    T val = tail_->val;

    delete tail_;
    tail_ = newTail;

    --size_;
    return val;
}