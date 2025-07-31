#include "hash_table.h"
#include "general.h"

template <class T, class F, class C, unsigned S>
HashTable<T, F, C, S>::HashTable() {
    table = new C[S];
    for (unsigned i = 0; i < S; i++) {
        table[i] = C();
    }
}

template <class T, class F, class C, unsigned S>
HashTable<T, F, C, S>::~HashTable() {
    delete[] table;
}

template <class T, class F, class C, unsigned S>
int HashTable<T, F, C, S>::hashFunction(int key) {
    return hash(key) % S;
}

template <class T, class F, class C, unsigned S>
void HashTable<T, F, C, S>::insert(T value) {
    int index = hashFunction(value);
    table[index].insert(value);
}

template <class T, class F, class C, unsigned S>
void HashTable<T, F, C, S>::remove(T value) {
    int index = hashFunction(value);
    table[index].remove(value);
}

template <class T, class F, class C, unsigned S>
bool HashTable<T, F, C, S>::find(T value) {
    int index = hashFunction(value);
    return table[index].find(value);
}