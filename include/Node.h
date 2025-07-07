#ifndef __NODE__
#define __NODE__
#include <SDL3/SDL.h>
#include <vector>
class Node
{
public:
    SDL_FPoint shape;
    std::vector<Node *> childrens;
    Node(float x, float y);
    Node();
    ~Node();
    // bool isInCircule(float x, float y);
    // void print(SDL_Renderer *renderer);
};

#endif