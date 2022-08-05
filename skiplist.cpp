#include "skiplist.h"

#define STORE_FILE "store/dumpFile"

mutex mtx;
string delimiter = ":";

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
        cout << "key: " << key << ", exists" << endl;
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

    cout << "Successfully inserted key:" << key << ", value:" << value << endl;
    ++_element_count;

    mtx.unlock();
    return 0; 

}

// display the skiplist
template<typename K, typename V>
void Skiplist<K, V>::display_list() const {
    cout << endl << "***** SKIP LIST*****" << endl;
    for (int i = 0; i <= _skiplist_level; ++i) {
        Node<K, V> *cur = this->_head->forward[i];
        cout << "Level " << i << ": ";
        while (cur) {
            cout << cur->get_key() << ":" << cur->get_val() << ";";
            cur = cur->forward[i];
        }
        cout << endl;
    }
}

// dump the data in the memory to file
template<typename K, typename V>
void Skiplist<K, V>::dump_file() {
    cout << "----------dump file----------" << endl;
    _file_writer.open(STORE_FILE);
    
    Node<K, V> *cur = this->_head->forward[0];

    while (cur) {
        _file_writer << cur->get_key() << ":" << cur->get_val() << "\n";
        cout << cur->get_key() << ":" << cur->get_val() << ";\n";
        cur = cur->forward[0];
    }

    _file_writer.flush();
    _file_writer.close();
    return ;
}

// load the data from the disk
template<typename K, typename V>
void Skiplist<K, V>::load_file() {
    _file_reader.open(STORE_FILE);
    cout << "----------load file----------" << endl;
    string line;
    string *key = new string();
    string *val = new string();
    while (getline(_file_reader, line)) {
        get_key_val_from_string(line, key, val);
        
        if (key->empty() || val->empty()) {
            continue;
        }

        insert_element(*key, *val);
        cout << "key:" << *key << "value:" << *val << endl;
    }
    _file_reader.close();
}

// get current size of the skiplist
template<typename K, typename V>
int Skiplist<K, V>::get_size() const {
    return _element_count;
}

template<typename K, typename V>
void Skiplist<K, V>::get_key_val_from_string(const string& str, string *key, string *val) {
    if (!is_valid_string(str)) {
        return ;
    }
    auto idx = str.find(delimiter);
    *key = str.substr(0, idx);
    *val = str.substr(idx + 1, str.size());
}

template<typename K, typename V>
bool Skiplist<K, V>::is_valid_string(const string& str) {
    if (str.empty() || str.find(delimiter) == string::npos) {
        return false;
    }
    return true;
}


// delete element from the skiplist
template<typename K, typename V>
void Skiplist<K, V>::delete_element(K key) {
    mtx.lock();

    Node<K, V> *cur = this->_head;
    Node<K, V> *update[_max_level + 1];
    memset(update, 0, sizeof(Node<K, V>*) * (_max_level + 1));

    // start searching from the highest level of the skiplist
    for (int i = _skiplist_level; i >= 0; --i) {
        while (cur->forward[i] && cur->forward[i]->get_key() < key) {
            cur = cur->forward[i];
        }
        update[i] = cur;
    }

    cur = cur->forward[0];

    if (cur && cur->get_key() == key) {
        // start from the lowest level, and delete the current node of each level
        for (int i = 0; i <= _skiplist_level; ++i) {
            // if at level i, and next node is not the target node
            // break
            if (update[i]->forward[i] != cur) {
                break;
            }
            update[i]->forward[i] = cur->forward[i];
        }

        // remove level that have no elements
        while (_skiplist_level > 0 && _head->forward[_skiplist_level] == 0) {
            --_skiplist_level;
        }

        cout << "delete key " << key << "successfully" << endl;
        --_element_count;
    }
    mtx.unclock();
    return ;
}

template<typename K, typename V>
bool Skiplist<K, V>::search_element(K key) const {
    cout << "----------search element----------" << endl;
    Node<K, V> *cur = _head;

    // start searching from the highest level of the skiplist
    for (int i = _skiplist_level; i >= 0; --i) {
        while (cur->forward[i] && cur->forward[i]->get_key() < key) {
            cur = cur->forward[i];
        }
    }

    // reach level 0
    cur = cur->forward[0];

    // if current node's key is equal to the searched key, it means we get it
    if (cur && cur->get_key() == key) {
        return true;
    }

    cout << "key " << key << " not found" << endl;
    return false;
}