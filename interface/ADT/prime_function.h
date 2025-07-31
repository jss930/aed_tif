#ifndef __PRIME_FUNCTION__
#define __PRIME_FUNCTION__

template <class T>
class PrimeFunction {
public:
    int operator()(T value) const {
        return (value * 2654435761U) % 4294967296U;
    }
};

#endif
