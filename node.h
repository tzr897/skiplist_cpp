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

#endif