#include <stdio.h>

template <class T>
class PrimeFunction {
    inline int operator()(T value) {
        // A simple prime hash function
        return (value * 2654435761U) % 4294967296U; // 2^32
    }
};