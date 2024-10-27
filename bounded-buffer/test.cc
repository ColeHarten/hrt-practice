#include "buffer.hh"
#include <iostream>
#include <thread>
#include <vector>

using namespace std;

#define BUFFSIZE 200

void printBuffer(const buffer& buff, bool asChar = true, size_t sz = BUFFSIZE) {
    for(size_t i = 0 ; i < sz; i++){
        if(asChar)
            cout<<buff[i]<<" ";
        else 
            cout<<to_string(buff[i])<<" ";
    }
    cout<<endl;
}

// Test basic read and write to fill up buffer
void test1(buffer& buff) {
    for(int i = 0; i < BUFFSIZE; ++i) {
        unsigned char lsb = i & 0xFF;

        size_t nwritten = buff.write(&lsb, 1);
        assert(nwritten);
    }

    assert(buff.full());


    for(int i = 0; i < BUFFSIZE; ++i) {
        unsigned char ch;
        size_t nread = buff.read(&ch, 1);

        assert(nread);

        unsigned char lsb = i & 0xFF;

        assert(ch == lsb);
    }   
    

    assert(buff.empty());
}

// Test looping read and write by writing 1 byte and immediately reading it 0xFF times
void test2(buffer& buff) {
    for(int i = 0; i < 0xFF; ++i) {
        unsigned char lsb = i & 0xFF;
        size_t nwritten = buff.write(&lsb, 1);
        assert(nwritten);

        unsigned char ch;
        buff.read(&ch, 1);
        
        assert(lsb == ch);
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

// Test writing variable number of bytes
void test4(buffer& buff) {
    for(unsigned char i = 0; i < 10; ++i) {
        unsigned char* arr = new unsigned char[i];
        memset(arr, i, i);

        size_t nwrite = buff.write(arr, i);
        (void)nwrite;
        delete[] arr;
    }
    for(unsigned char i = 0; i < 10; ++i) {
        unsigned char* arr = new unsigned char[i];

        size_t nread = buff.read(arr, i);

        assert(nread == i);

        for(int j = 0; j < i; j++) {
            assert(arr[j] == i);
        }

        delete[] arr;
    }
}

void foo(buffer* buff, unsigned char* ch, const size_t buff_size) {
    for(size_t i = 0; i < buff_size / 10; ++i) {
        buff->block_write(ch, 5);
        this_thread::sleep_for(5ms);
    }
}

// test basic mutual exclusion with concurrency
void test5() {
    const size_t buff_size = 200;
    buffer buf(buff_size);

    unsigned char* a = new unsigned char[5];
    memset(a, 'a', 5);
    unsigned char* b = new unsigned char[5];
    memset(b, 'b',5);

    thread th1(foo, &buf, a, buff_size);
    thread th2(foo, &buf, b, buff_size);

    th1.join();
    th2.join();

    int packsA = 0;
    int packsB = 0;

    for(unsigned i = 0; i < buff_size / 5; i++) {
        unsigned char arr[5];

        assert(buf.block_read(arr, 5));

        if(memcmp(arr, "aaaaa", 5) == 0) {
            packsA++;
        } else if(memcmp(arr, "bbbbb", 5) == 0) {
            packsB++;
        } else assert(false);
    }

    assert(packsA == packsB && packsB == buff_size/10);

    delete[] a;
    delete[] b;
}

void producer(buffer* buf, const std::vector<unsigned char>& data, size_t chunk_size) {
    size_t total_written = 0;
    while (total_written < data.size()) {
        size_t write_size = std::min(chunk_size, data.size() - total_written);
        buf->block_write(data.data() + total_written, write_size);
        total_written += write_size;
        // std::cout << "Produced " << write_size << " bytes." << std::endl;
    }
}

void consumer(buffer* buf, size_t total_size, size_t chunk_size) {
    std::vector<unsigned char> output(total_size, 0);
    size_t total_read = 0;
    while (total_read < total_size) {
        size_t read_size = std::min(chunk_size, total_size - total_read);
        buf->block_read(output.data() + total_read, read_size);
        total_read += read_size;
        // std::cout << "Consumed " << read_size << " bytes." << std::endl;
    }
    
    // Verify the data
    for (size_t i = 0; i < total_size; ++i) {
        assert(output[i] == (i % 256));  // Expected value from producer initialization
    }
    // std::cout << "Data verified successfully." << std::endl;
}

void test6() {
    buffer buf(512);

    size_t data_size = 4096;        // Total data size for testing
    size_t chunk_size = 256;        // Size of each read/write operation


    // Prepare data for producer to write
    std::vector<unsigned char> data(data_size);
    for (size_t i = 0; i < data_size; ++i) {
        data[i] = static_cast<unsigned char>(i % 256);  // Fill with predictable data
    }

    // Launch producer and consumer threads
    std::thread prod_thread(producer, &buf, std::ref(data), chunk_size);
    std::thread cons_thread(consumer, &buf, data_size, chunk_size);

    // Wait for threads to finish
    prod_thread.join();
    cons_thread.join();
}

const size_t BUFFER_SIZE = 256;
const size_t CHUNK_SIZE = 100;

// Function for producers to write data to the buffer
void producer_many(buffer* buf, unsigned char* test_data, std::atomic<bool>& run_flag) {
    while (run_flag.load()) {
        // Attempt to write the data to the buffer
        if (buf->block_write(test_data, CHUNK_SIZE)) {
            std::this_thread::sleep_for(50ms);
        } else assert(false);
    }
}

// Function for producers to read data to the buffer
void consumer_many(buffer* buf, unsigned char* test_data, std::atomic<bool>& run_flag) {
    unsigned char read_data[CHUNK_SIZE];

    while (run_flag.load()) {
        // Attempt to read the data from the buffer
        if (buf->block_read(read_data, CHUNK_SIZE)) {
            assert(memcmp(read_data, test_data, CHUNK_SIZE) == 0);
            std::this_thread::sleep_for(50ms);
        } else assert(false);
    }
}

// Main test function with initialization for test data
void test7() {
    buffer buf(BUFFER_SIZE);
    std::atomic<bool> run_flag(true);

    // Data arrays for producers and consumers
    unsigned char test_data[CHUNK_SIZE];

    for (unsigned char i = 0; i < CHUNK_SIZE; ++i) {
        test_data[i] = i;
    }

    int num_producers = 3;
    int num_consumers = 3;
    std::vector<std::thread> producers, consumers;

    // Start producer threads
    for (int i = 0; i < num_producers; ++i) {
        producers.emplace_back(producer_many, &buf, test_data, std::ref(run_flag));
    }

    // Start consumer threads
    for (int i = 0; i < num_consumers; ++i) {
        consumers.emplace_back(consumer_many, &buf, test_data, std::ref(run_flag));
    }

    // Run the test for a longer time to allow buffer interactions
    std::this_thread::sleep_for(100ms);
    run_flag.store(false);

    for (auto& prod : producers) {
        if (prod.joinable()) {
            prod.join();
        }
    }

    for (auto& cons : consumers) {
        if (cons.joinable()) {
            cons.join();
        }
    }
}


int main() {    
    buffer buff(BUFFSIZE);
    
    for(int k = 0; k < 1; ++k) {
        test1(buff);
        std::cout << "\033[32m" << "Passed test 1." << "\033[0m" << std::endl;
        test2(buff);
        std::cout << "\033[32m" << "Passed test 2." << "\033[0m" << std::endl;
        test3(buff);
        std::cout << "\033[32m" << "Passed test 3." << "\033[0m" << std::endl;
        test4(buff);
        std::cout << "\033[32m" << "Passed test 4." << "\033[0m" << std::endl;
        test5();
        std::cout << "\033[32m" << "Passed test 5." << "\033[0m" << std::endl;
        test6();
        std::cout << "\033[32m" << "Passed test 6." << "\033[0m" << std::endl;
        test7();
        std::cout << "\033[32m" << "Passed test 7." << "\033[0m" << std::endl;
    }

    std::cout << endl<<"\033[32m" << "Passed all tests!" << "\033[0m" << std::endl;

    return 0;
} 