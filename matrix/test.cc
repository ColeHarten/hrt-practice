#include "matrix.hh"
#include <cassert>
#include <iostream>

using namespace std;

int main() {
    matrix<int> m1(4,4);
    m1.randomize();
    m1.print();

    std::cout<<"\n";

    m1.reshape(8,2);
    m1.print();

    std::cout<<"\n";

    m1.transpose();
    m1.print();

    return 0;
}