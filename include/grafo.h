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
    Node *mapy;
    int num_nodes;

public:
    float max_x, max_y;
    Grafo(int number_nodes);
    ~Grafo();
    Node *order_nodes(Node *lista, int tam, char type);
    void create_children();
    bool circuleCoalition(Node &tajet, Node *list, int idx, float gap);
    bool frustum(Node &node, SDL_FRect &cam, float px, float py);
    void render(SDL_Renderer *renderer, float &pos_x, float &pos_y);
    void renderAristas(SDL_Renderer *renderer, int indx, int pos_x, int pos_y);
    void renderAristas(SDL_Renderer *renderer, Node *n1, Node *n2, int pos_x, int pos_y);
};

#endif