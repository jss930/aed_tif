#ifndef __HASH_TABLE__
#define __HASH_TABLE__

#include <stdio.h>
#include "../_node.h"
#include <vector>

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

    int hashFunction(T key) {
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

    T* get(T value) {
        int index = hashFunction(value);
        typename ::_Node<T>* current = table[index].getHead();

        while (current) {
            if (current->value == value) {
                return &(current->value);
            }
            current = current->next;
        }
        return nullptr;
    }

    std::vector<T> getAllElements() const {
        std::vector<T> elements;
        for (unsigned i = 0; i < S; i++) {
            typename ::_Node<T>* current = table[i].getHead();
            while (current) {
                elements.push_back(current->value);
                current = current->next;
            }
        }
        return elements;
    }
};

#endif
