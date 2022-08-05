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

    // now we reach level 0
    // forward pointer to right node, which is desired to insert key
    cur = cur->forward[0];

    // if current node's key is equal to the searched key, it means we get it
    if (cur && cur->get_key() == k) {
        cout << "Key: " << k << " exists." << endl;
        mtx.unlock();
        return 1; 
    }

    // now cur == nullptr, or the key is different from the searched key
    int random_level = get_random_level();
    
    // if the random_level we get is greater than current level
    // initialize update value with pointer to header
    if (random_level > _skiplist_level) {
        for (int i = _skiplist_level + 1; i <= random_level; ++i) {
            update[i] = _head;
        }
        _skiplist_level = random_level;
    }

    // create the new node with random_level
    Node<K, V> *new_node = create_node(k, v, random_level);

    // insert the new node
    for (int i = 0; i <= random_level; ++i) {
        new_node->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = new_node;
    }

    cout << "key: " << k << " value: " << v << " , has been inserted successfully" << endl;
    ++_element_count;

    mtx.unlock();
    return 0; 

}

// display the skiplist
template<typename K, typename V>
void Skiplist<K, V>::display_list() {
    
}
