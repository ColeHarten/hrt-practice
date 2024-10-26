#include "buffer.hh"
#include <iostream>

using namespace std;

unsigned char BUFFSIZE;

void printBuffer(const buffer& buff) {
    for(int i = 0 ; i < BUFFSIZE; i++){
        cout<<to_string(buff.buf[i])<<" ";
    }
    cout<<endl;
}

// Test basic read and write to fill up buffer
void test1(buffer& buff) {
    for(unsigned char i = 0; i < BUFFSIZE; ++i) {
        size_t nwritten = buff.write(&i, 1);
        assert(nwritten);
    }

    assert(buff.full());


    for(unsigned char i = 0; i < BUFFSIZE; ++i) {
        unsigned char ch;
        size_t nread = buff.read(&ch, 1);

        assert(nread);
        assert(ch == i);
    }   
    

    assert(buff.empty());
}

// Test looping read and write by writing 1 byte and immediately reading it 0xFF times
void test2(buffer& buff) {
    for(unsigned char i = 0; i < ~(unsigned char)0; ++i) {
        size_t nwritten = buff.write(&i, 1);
        assert(nwritten);

        unsigned char ch;
        buff.read(&ch, 1);
        
        assert(i == ch);
    }

    assert(buff.empty());
}

// Test writing 4 bytes at a time
void test3(buffer& buff) {
    for(int i = 0; i < BUFFSIZE/4-1; i++) {
        size_t nwritten = buff.write(&i, 4);
        assert(nwritten == 4);
    }

    for(int i = 0; i < BUFFSIZE/4-1; ++i) {
        int val;
        size_t nread = buff.read(&val, 4);
        assert(nread == 4);

        assert(val == i);
    }   
}

int main() {    
    for(unsigned char j = 4; j < 255; j++){
        BUFFSIZE = j;
        buffer buff(BUFFSIZE);
        
        for(int i = 0; i < 2; i++){
            test1(buff);
            std::cout << "\033[32m" << "Passed test 1." << "\033[0m" << std::endl;
            test2(buff);
            std::cout << "\033[32m" << "Passed test 2." << "\033[0m" << std::endl;
            test3(buff);
            std::cout << "\033[32m" << "Passed test 3." << "\033[0m" << std::endl;
        }
    }

    std::cout <<endl<< "\033[32m" << "Passed all tests!" << "\033[0m" << std::endl;

    return 0;
} 