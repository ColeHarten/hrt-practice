#include "matrix.hh"

template <typename T>
matrix<T>::matrix(int m, int n) {
    assert(m > 0 && n > 0);

    r = m;
    c = n;
    transposed = false;

    mat = new T[r*c];
    assert(mat);

    memset(mat, 0, r*c*sizeof(T));
}

template <typename T> 
matrix<T>::matrix(matrix<T>& other) {   
    r = other.r;
    c = other.c;
    transposed = other.transposed;

    mat = new T[r*c];
    assert(mat);
    
    memcpy(mat, other.mat, r*c*sizeof(T));
}

template <typename T>
matrix<T>::~matrix() {
    delete[] mat;
}

template <typename T>
T& matrix<T>::get(int i, int j) {
    assert(i >= 0 && j >= 0);
    assert((unsigned)i < r && (unsigned)j < c);

    if(transposed) {
        return mat[(unsigned)j * r + (unsigned)i];
    } else {
        return mat[(unsigned)i * c + (unsigned)j];
    }
}

template <typename T>
void matrix<T>::transpose() {
    transposed = !transposed;
    std::swap(r,c);
}

template <typename T>
void matrix<T>::print() {
    unsigned stride = transposed ? r : 1;
    unsigned index = 0;
    for (unsigned i = 0; i < r * c; ++i, index+=stride) {
        unsigned j = index == stride*(c*r-1) ? r*c-1 : index % (r*c -1);
        std::cout<<mat[j]<<", ";

        if((i+1) % c == 0) {
            std::cout<<std::endl;
        }
    }
}

template <typename T>
void matrix<T>::randomize(int max) {
    std::mt19937 rng (std::random_device{}());
    std::uniform_int_distribution<> dist (0, max);

    for(unsigned k = 0; k < r*c; k++){
        mat[k] = dist(rng);
    }
}

template <typename T>
bool matrix<T>::operator==(matrix<T>& other) {
    if(other.r != r || other.c != c) return false; 

    for(int i = 0; (unsigned)i < r; ++i) {
        for (int j = 0; (unsigned)j < c; ++j) {
            if(this->get(i,j) != other.get(i,j)) return false;
        }
    }

    return true;
}

template <typename T>
void matrix<T>::matmul(matrix<T>& a, matrix<T>& b, matrix<T>& ret) {
    assert(a.c == b.r);
    assert(a.r == ret.r && b.c == ret.c);

    for(int k = 0; (unsigned)k < a.c; ++k) {
        for(int i = 0; (unsigned)i < a.r; ++i) {
            for(int j = 0; (unsigned)j < b.c; ++j) {
                ret.get(i,j) += a.get(i,k) * b.get(k,j);
            }
        }
    }
}
