#include "skiplist.h"

template<typename K, typename V>
Skiplist<K, V>::Skiplist(int max_level) {
    this->_max_level = max_level;
    this->_skiplist_level = 0;
    this->_element_count = 0;

    K new_k;
    V new_v;
    this->_head = new Node<K, V>(new_k, new_v, _max_level);
}

template<typename K, typename V>
Skiplist<K, V>::~Skiplist() {
    if (_file_writer.is_open()) {
        _file_writer.close();
    }

    if (_file_reader.is_open()) {
        _file_reader.close();
    }

    delete _head;
}


template<typename K, typename V>
int Skiplist<K, V>::get_random_level() {
    int k = 1;
    while (rand() % 2) {
        ++k;
    }
    if (k > _max_level) {
        k = _max_level;
    }
    return k;
}



