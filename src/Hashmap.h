//
// Created by matej on 11/30/18.
//

#ifndef BRAINFUCK_INTERPRETER_HASHTABLE_H
#define BRAINFUCK_INTERPRETER_HASHTABLE_H


#include "types.h"

template<class K, class V>
class Hashmap {

    class Entry {
        K key;
        V value;
    };

private:
    Entry *buckets;
    uint32 size = 0;
    uint32 entries = 0;

    int hash(K key);
    int resize();

public:
    bool has(K key);
    V get(K key);
    void put(K key, V value);
};

#endif //BRAINFUCK_INTERPRETER_HASHTABLE_H
