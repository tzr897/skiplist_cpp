#ifndef __NODE__H
#define __NODE__H

template<typename K, typename V>
class Node {
public:
    Node();

    Node(K k, V v, int level);

    ~Node();

    K get_key() const;

    V get_val() const;

    void set_val(V v);

    int node_level;

    Node<K, V> **forward;

private:
    K _key;

    V _val;
    
};

template<typename K, typename V>
Node<K, V>::Node() {}

template<typename K, typename V>
Node<K, V>::Node(const K k, const V v, int level) {
    this->_key = k;
    this->_val = v;
    this->node_level = level;

    this->forward = new Node<K, V>*[level + 1];

    memset(this->forward, 0, sizeof(Node<K, V>*) * (level + 1));
}

template<typename K, typename V>
Node<K, V>::~Node() {
    delete []forward;
}

template<typename K, typename V>
K Node<K, V>::get_key() const {
    return _key;
}

template<typename K, typename V>
V Node<K, V>::get_val() const {
    return _val;
}

template<typename K, typename V>
void Node<K, V>::set_val(V v) {
    this->_val = v;
}

#endif