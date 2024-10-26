#ifndef HEADER_H
#define HEADER_H

#include <assert.h>
#include <cstring>
#include <iostream>
#include <random>

template <typename T>
class matrix {
private:
    T* mat;
    unsigned r, c;
    bool transposed;
public:
    matrix(int m, int n);
    matrix(matrix<T>& other);
    ~matrix();

    T& get(int i, int j);

    std::pair<int,int> shape();
    void reshape(int rows, int cols);

    void transpose();
    void print();
    void randomize(int max = 100);

    bool operator==(matrix<T>& other);

    static void matmul(matrix<T>& a, matrix<T>& b, matrix<T>& ret);
};

#include "matrix.tpp"
#endif