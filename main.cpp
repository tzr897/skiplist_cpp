#include <iostream>
#include "node.h"

#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#include "skiplist.h"

#define STORE_FILE "store/dumpFile"

Skiplist<int, string> test(0);


int main() {
    int times = 10000000;
    auto begin_t = std::chrono::system_clock::now();
    Skiplist<int, string> sl(20);
    for (int i = 0; i < times; ++i) {
        int write_idx = rand() % 100000;
        sl.insert_element(write_idx, "write");
        // int read_idx = rand() % 100000;
        // sl.search_element(read_idx);
    }
    auto end_t = std::chrono::system_clock::now();
    std::chrono::duration<double> diff = end_t - begin_t;
    double d = diff.count();
    double write_speed = double(times) / d;
    
//////////////////////////////////////////////////////////////////
    begin_t = std::chrono::system_clock::now();
    for (int i = 0; i < times; ++i) {
        // int write_idx = rand() % 100000;
        // sl.insert_element(write_idx, "write");
        int read_idx = rand() % 100000;
        sl.search_element(read_idx);
    }
    end_t = std::chrono::system_clock::now();
    diff = end_t - begin_t;
    d = diff.count();
    double read_speed = double(times) / d;
    cout << "write: " << write_speed << endl;
    cout << "read: " << read_speed << endl;
    return 0;
}
