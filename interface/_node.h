#ifndef ___NODE__
#define ___NODE__

#include <stdio.h>

template <class T>
class _Node {
public:
    T value;
    _Node *next;

    _Node(T val) : value(val), next(nullptr) {}
    
    _Node() : next(nullptr) {}
    
    ~_Node() =default;
};

#endif