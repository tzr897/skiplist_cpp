#include <iostream>
#include "node.h"
#include "skiplist.h"

#define STORE_FILE "store/dumpFile"

Skiplist<int, string> test(0);


int main() {
    
    Skiplist<int, string> sl(10);
    sl.insert_element(1, "one");
    sl.insert_element(3, "last");
    sl.insert_element(5, "kiss");
    sl.insert_element(6, "beautiful");
    sl.insert_element(8, "world");
    sl.insert_element(10, "fly");
    sl.insert_element(17, "me");
    sl.insert_element(20, "to");
    sl.insert_element(21, "the");
    sl.insert_element(56, "moon");
    sl.insert_element(33, "eva");
    sl.insert_element(33, "ave");

    cout << "skiplist size is: " << sl.get_size() << endl;

    sl.search_element(10);
    sl.search_element(11);
    
    sl.display_list();

    sl.delete_element(56);
    sl.delete_element(3);
    sl.delete_element(34);

    cout << "skiplist size is: " << sl.get_size() << endl;

    sl.display_list();

    return 0;
}
