#ifndef __GRAFO__
#define __GRAFO__
#include "Node.h"
#include "general.h"
class Grafo
{
private:
    Node *start;
    Node *tarjet;
    Node *map;
    int num_nodes;

public:
    float max_x, max_y;
    Grafo(int number_nodes);
    ~Grafo();
    bool circuleCoalition(Node &tajet, Node *list, int idx, float gap);
    bool frustum(Node &node, SDL_FRect &cam, float px, float py);
    void render(SDL_Renderer *renderer, float &pos_x, float &pos_y);
};

#endif