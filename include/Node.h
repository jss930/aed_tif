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
    ~Node();
};

#endif
