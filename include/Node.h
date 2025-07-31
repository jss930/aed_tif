#ifndef __NODE__
#define __NODE__

#include <SDL3/SDL.h>
#include "vector.h"

class Node {
public:
    SDL_FPoint shape;
    Vector<Node*> childrens;

    Node(float x, float y);
    Node();
    Node(const Node &other) {
        shape = other.shape;
    }
    ~Node();

    Node &operator=(const Node &other) {
        if (this != &other) {
            shape = other.shape;
            // NO copiamos `childrens`
        }
        return *this;
    }
};

#endif
