#include "my_stdlib.hh"
#include <cassert>
#include <iostream>

void test_constructor_and_size() {
    my_std::list<int> myList;
    assert(myList.size() == 0);
}

void test_push_back() {
    my_std::list<int> myList;
    myList.push_back(1);
    myList.push_back(2);
    myList.push_back(3);
    assert(myList.size() == 3);
    assert(myList[0] == 1);
    assert(myList[1] == 2);
    assert(myList[2] == 3);
}

void test_insert_beginning() {
    my_std::list<int> myList;
    myList.push_back(1);
    myList.push_back(2);

    myList.insert(0, 10); // Insert at the beginning
    assert(myList.size() == 3);
    assert(myList[0] == 10);
    assert(myList[1] == 1);
    assert(myList[2] == 2);
}

void test_insert_middle() {
    my_std::list<int> myList;
    myList.push_back(1);
    myList.push_back(3);

    myList.insert(1, 2); // Insert in the middle
    assert(myList.size() == 3);
    assert(myList[0] == 1);
    assert(myList[1] == 2);
    assert(myList[2] == 3);
}

void test_insert_end() {
    my_std::list<int> myList;
    myList.push_back(1);
    myList.push_back(2);

    myList.insert(2, 3); // Insert at the end
    assert(myList.size() == 3);
    assert(myList[0] == 1);
    assert(myList[1] == 2);
    assert(myList[2] == 3);
}

void test_pop_front() {
    my_std::list<int> myList;
    myList.push_back(1);
    myList.push_back(2);
    myList.push_back(3);

    int frontVal = myList.pop_front();
    assert(frontVal == 1);
    assert(myList.size() == 2);
    assert(myList[0] == 2);
    assert(myList[1] == 3);
}

void test_pop_back() {
    my_std::list<int> myList;
    myList.push_back(1);
    myList.push_back(2);
    myList.push_back(3);

    int backVal = myList.pop_back();
    assert(backVal == 3);
    assert(myList.size() == 2);
    assert(myList[0] == 1);
    assert(myList[1] == 2);
}

void test_combined_operations() {
    my_std::list<int> myList;
    
    // Insert and remove in various positions
    myList.push_back(5);  // List: [5]
    myList.insert(1, 10); // List: [5, 10]
    myList.insert(1, 7);  // List: [5, 7, 10]
    assert(myList.size() == 3);
    assert(myList[0] == 5);
    assert(myList[1] == 7);
    assert(myList[2] == 10);

    // Remove from front, middle, and back
    assert(myList.pop_front() == 5); // List: [7, 10]
    assert(myList.pop_back() == 10); // List: [7]
    assert(myList.size() == 1);
    assert(myList[0] == 7);

    // Insert multiple times at different positions
    myList.insert(0, 3);  // List: [3, 7]
    myList.insert(1, 4);  // List: [3, 4, 7]
    myList.insert(3, 8);  // List: [3, 4, 7, 8]
    assert(myList.size() == 4);
    assert(myList[0] == 3);
    assert(myList[1] == 4);
    assert(myList[2] == 7);
    assert(myList[3] == 8);
}

void test_edge_cases() {
    my_std::list<int> myList;

    // Insert into an empty list
    myList.insert(0, 100); // List: [100]
    assert(myList.size() == 1);
    assert(myList[0] == 100);

    // Pop from single-element list
    assert(myList.pop_back() == 100); // List: []
    assert(myList.size() == 0);

    // Insert at the beginning repeatedly
    myList.insert(0, 1); // List: [1]
    myList.insert(0, 2); // List: [2, 1]
    myList.insert(0, 3); // List: [3, 2, 1]
    assert(myList.size() == 3);
    assert(myList[0] == 3);
    assert(myList[1] == 2);
    assert(myList[2] == 1);

    // Remove all elements to return to an empty state
    myList.pop_front();
    myList.pop_front();
    myList.pop_front();
    assert(myList.size() == 0);
}

int main() {
    test_constructor_and_size();
    test_push_back();
    test_insert_beginning();
    test_insert_middle();
    test_insert_end();
    test_pop_front();
    test_pop_back();
    test_combined_operations();
    test_edge_cases();

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
