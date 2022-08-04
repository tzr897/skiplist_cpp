#ifndef __SKIPLIST__H
#define __SKIPLIST__H

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <mutex>
#include <fstream>

#include "node.h"


using std::mutex;
using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::ofstream;
using std::ifstream;


template<typename K, typename V>
class Skiplist {
public:
    Skiplist(int n);

    ~Skiplist();

    int get_random_level();

    Node<K, V>* create_node(K k, V v, int l);

    int insert_element(K, V);

    void display_list();

    bool search_element(K k);

    void delete_element(K k);

    void dump_file();

    void load_file();

    int get_size() const; // t

private:
    // maximum level of the skiplist
    int _max_level;

    // current level of the skiplist
    int _skiplist_level;

    // pointer to the head node
    Node<K, V> *_head;

    // file operator
    ifstream _file_reader;
    ofstream _file_writer;

    // current element count of the skiplist
    int _element_count;

    void get_key_val_from_string(const string& str, string *key, string *val);

    bool is_valid_string(const string& str);

};

#endif