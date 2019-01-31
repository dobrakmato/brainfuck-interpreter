#ifndef BRAINFUCK_INTERPRETER_HASHTABLE_H
#define BRAINFUCK_INTERPRETER_HASHTABLE_H


#include <stdexcept>

class KeyError : std::exception {

};

template<typename K, typename V>
class Hashmap {

    class Entry {
    public:
        K key;
        V value;
        Entry *next = nullptr;

    public:
        Entry(K key, V value, Entry *next) : key(key), value(value), next(next) {}
    };

private:
    Entry **m_buckets;
    uint32_t m_size = 0;
    uint32_t m_entries = 0;
    float m_loadFactor = 0.8f;

    uint32_t hash(K key) {
        return key * 2654435761 % 2 ^ 32;
    }

    void resize() {
        auto oldSize = m_size;
        auto oldBuckets = m_buckets;

        m_size = 2 * m_size;
        m_entries = 0;
        initializeBuckets();

        for (uint32_t i = 0; i < oldSize; i++) {
            auto entry = oldBuckets[i];
            if (entry == nullptr) continue;
            put(entry->key, entry->value);

            while (entry->next != nullptr) {
                auto nextEntry = entry->next;
                delete entry;
                entry = nextEntry;
                put(entry->key, entry->value);
            }
        }

        delete oldBuckets;
    }

    void initializeBuckets() {
        m_buckets = new Entry *[m_size];
        for (uint32_t i = 0; i < m_size; i++) {
            m_buckets[i] = nullptr;
        }
    }

public:
    explicit Hashmap(uint32_t size, float loadFactor = 0.75f) {
        this->m_size = size;
        this->m_loadFactor = loadFactor;
        initializeBuckets();
    }

    bool has(K key) {
        uint32_t hash = this->hash(key);
        auto entry = m_buckets[hash % m_size];

        if (entry == nullptr) return false;
        while (entry != nullptr) {
            if (entry->key == key) {
                return true;
            }

            entry = entry->next;
        }
        return false;
    }

    V get(K key) {
        uint32_t hash = this->hash(key);
        auto entry = m_buckets[hash % m_size];

        if (entry == nullptr) throw KeyError();
        while (entry != nullptr) {
            if (entry->key == key) {
                return entry->value;
            }

            entry = entry->next;
        }
        throw KeyError();
    }

    void put(const K key, const V value) {
        uint32_t hash = this->hash(key);
        auto entry = m_buckets[hash % m_size];

        if (entry == nullptr) {
            m_buckets[hash % m_size] = new Entry(key, value, nullptr);
            m_entries++;
            return;
        }

        while (entry->next != nullptr) {
            if (entry->key == key) {
                entry->value = value;
            }
            entry = entry->next;
        }

        entry->next = new Entry(key, value, nullptr);
        m_entries++;

        if ((float) m_entries / m_size > m_loadFactor) {
            resize();
        }
    }

    uint32_t size() const {
        return m_size;
    }

    uint32_t entries() const {
        return m_entries;
    }
};


#endif //BRAINFUCK_INTERPRETER_HASHTABLE_H
