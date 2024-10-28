#include "my_stdlib.hh"
#include <iostream>

using std::cout, std::endl;
using namespace my_std;

int main() {

    my_std::vector<int> arr(10, 10);
    arr.push_back(11);
    cout<<arr.max_size()<<endl;

    arr.erase(2, arr.size()-1);

    cout<<arr.max_size()<<endl;

    for(unsigned i = 0; i < arr.size(); ++i) {
        cout<<arr[i]<<endl;
    }

    return 0;
}