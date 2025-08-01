#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stdio.h>

template <class T>
class Vector
{
private:
    T *data;
    size_t capacity;
    size_t size;

public:
    Vector(size_t initial_capacity = 10) : capacity(initial_capacity), size(0)
    {
        data = new T[capacity];
    }

    ~Vector()
    {
        delete[] data;
    }

    void push_back(const T &value)
    {
        if (size >= capacity)
        {
            capacity *= 2;
            T *new_data = new T[capacity];
            for (size_t i = 0; i < size; i++)
            {
                new_data[i] = data[i];
            }
            delete[] data;
            data = new_data;
        }
        data[size++] = value;
    }

    void pop_back()
    {
        if (size > 0)
        {
            size--;
        }
    }

    T &operator[](size_t index)
    {
        return data[index];
    }

    const T &operator[](size_t index) const
    {
        return data[index];
    }

    size_t getSize() const
    {
        return size;
    }

    size_t getCapacity() const
    {
        return capacity;
    }

    void clear()
    {
        size = 0;
    }
};

#endif
