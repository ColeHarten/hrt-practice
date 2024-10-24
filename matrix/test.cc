#include "matrix.hh"
#include <cassert>
#include <iostream>

using namespace std;

int main() {
    matrix<int> m1(100,100);
    m1.randomize();
    // m1.print();

    std::cout<<"\n";   

    matrix<int> m2(100,100);
    m2.randomize();
    // m2.print();
    std::cout<<"\n";  
    
    matrix<int> m3(100,100);

    matrix<int>::matmul(m1,m2,m3);
    m3.print();

    return 0;
}