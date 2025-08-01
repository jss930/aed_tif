#ifndef __GRAFO__
#define __GRAFO__

#include <string>
#include "Node.h"
#include "general.h"
#include "myvector.h"

class Grafo
{
private:
    Node *start;
    Node *tarjet;
    Node *map;  // nodos ordenados por X
    Node *mapy; // nodos ordenados por Y
    int num_nodes;

    Vector<Node *> *nodos_renderizados = nullptr;

public:
    float max_x, max_y;

    Grafo(int number_nodes);
    ~Grafo();

    Node *order_nodes(Node *lista, int tam, char type);
    int *order_nodes_indices(Node *lista, int tam, char type);
    void create_children_parallel();
    void create_children();
    void generateNodesPoisson();
    void addConnection(int i, int j);
    bool circuleCoalition(Node &tajet, Node *list, int idx, float gap);
    bool frustum(Node &node, SDL_FRect &cam, float px, float py);

    void render(SDL_Renderer *renderer, float &pos_x, float &pos_y);
    void renderAristas(SDL_Renderer *renderer, int indx, int pos_x, int pos_y);
    void renderAristas(SDL_Renderer *renderer, Node *n1, Node *n2, int pos_x, int pos_y);

    void selectNodo(float posx, float posY, std::string tipo, int pos_x, int pos_y);

    bool buscarRutaBFS(); 
    bool buscarRutaAEstrella();
};

#endif
