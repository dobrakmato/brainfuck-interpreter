//
// Created by matej on 11/30/18.
//

#ifndef BRAINFUCK_INTERPRETER_HASHTABLE_H
#define BRAINFUCK_INTERPRETER_HASHTABLE_H


#include "types.h"

template<class K, class V>
class Hashmap {

    class Entry {
    public:
        K key;
        V value;
        Entry *next = nullptr;

    public:
        Entry(K key, V value, Entry* next) : key(key), value(value), next(next) {}
    };

    class Bucket {

    public:
        Entry *first = nullptr;
    };

private:
    Bucket **buckets;
    uint32 size = 0;
    uint32 entries = 0;

    uint32 hash(K key) {
        return 0;
    }

    void resize();

public:
    explicit Hashmap(uint32 size) {
        this->buckets = new Bucket*[size];
        this->size = size;
    }

public:
    bool has(K key);

    V get(K key);

    void put(const K key, const V value) {
        uint32 hash = this->hash(key);
        auto bucket = buckets[hash % size];

        if (bucket->first == nullptr) {
            bucket->first = new Entry(key, value, nullptr);
            return;
        }

        auto entry = bucket->first;
        while (entry->next != nullptr) entry = entry->next;
        entry->next = new Entry(key, value, nullptr);
    }
};



/**
 * Returns the value stored in this hashmap for specified key. If there is
 * not value for specified key an exception is thrown.
 *
 * @tparam K
 * @tparam V
 * @param key
 * @return
 */
template<class K, class V>
V Hashmap<K, V>::get(K key) {
    return 0;
}

/**
 * Checks if this hashmap contains specified key.
 *
 * @tparam K
 * @tparam V
 * @param key
 * @return
 */
template<class K, class V>
bool Hashmap<K, V>::has(K key) {
    return true;
}

#endif //BRAINFUCK_INTERPRETER_HASHTABLE_H
