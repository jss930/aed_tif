#ifndef __NODE__
#define __NODE__

#include <SDL3/SDL.h>
#include "myvector.h"

#define CUSTOM_INFINITY 1e30f

class Node
{
public:
    SDL_FPoint shape;
    Vector<Node *> childrens;

    bool visited = false;
    Node* parent = nullptr;

    float g = CUSTOM_INFINITY;  // costo desde inicio
    float h = 0;                // heurística
    float f = CUSTOM_INFINITY;  // g + h

    Node(float x, float y);
    Node();
    Node(const Node &other)
    {
        shape = other.shape;
    }
    ~Node();

    Node &operator=(const Node &other)
    {
        if (this != &other)
        {
            shape = other.shape;
            // NO copiamos `childrens`
        }
        return *this;
    }
};

#endif
