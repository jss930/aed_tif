#ifndef __HASH_TABLE__
#define __HASH_TABLE__

#include <stdio.h>

template <class T, class F, class C, unsigned S>
class HashTable {
private:
    F hash;
    C *table;

public:
    HashTable() {
        table = new C[S];
        for (unsigned i = 0; i < S; i++) {
            table[i] = C();
        }
    }

    ~HashTable() {
        delete[] table;
    }

    int hashFunction(int key) {
        return hash(key) % S;
    }

    void insert(T value) {
        int index = hashFunction(value);
        table[index].insert(value);
    }

    void remove(T value) {
        int index = hashFunction(value);
        table[index].remove(value);
    }

    bool find(T value) {
        int index = hashFunction(value);
        return table[index].find(value);
    }
};

#endif
