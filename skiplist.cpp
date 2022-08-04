#include "skiplist.h"

mutex mtx;

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

// create a new node for skiplist
template<typename K, typename V>
Node<K, V>* Skiplist<K, V>::create_node(K k, V v, int l) {
    Node<K, V> *new_node = new Node<K, V>(k, v, l);
    return new_node;
}

// insert a (key, val) into skiplist
// return 1: element exists
// return 0: insert successfully
template<typename K, typename V>
int Skiplist<K, V>::insert_element(const K k, const V v) {
    mtx.lock();
    Node<K, V> *cur = this->_head;

    Node<K, V> *update[_max_level + 1];
    memset(update, 0, sizeof(Node<K, V>*) * (_max_level + 1));

    // start searching from the highest level
    for (int i = _skiplist_level; i >= 0; --i) {
        while (cur->forward[i] && cur->forward[i]->get_key() < k) {
            cur = cur->forward[i];
        }
        update[i] = cur;
    }

    

}
