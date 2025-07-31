#ifndef __GRAFO__
#define __GRAFO__

#include "Node.h"
#include "general.h"
#include "ADT/hash_table.h"
#include "ADT/linked_list.h"
#include "ADT/node_hash.h"
#include "node_element.h"

#define MAX_SIZE 2000000

class Grafo
{
private:
    Node *start;
    Node *tarjet;
    Node *map;  // nodos ordenados en base X
    Node *mapy; // nodos ordenados en base Y
    HashTable<NodeElement, NodeHash, LinkedList<NodeElement>, MAX_SIZE> nodos;
    int num_nodes;
public:
    float max_x, max_y;
    Grafo(int number_nodes);
    ~Grafo();
    Node *order_nodes(Node *lista, int tam, char type);
    void insertarNodo(int id, float x, float y);
    void conectar(int id1, int id2);
    bool existeNodo(int id);
    void imprimirVecinos(int id);
    bool colisionaConNodosExistentes(float x, float y, float radio);
    void create_children();
    bool circuleCoalition(Node &tajet, Node *list, int idx, float gap);
    bool frustum(SDL_FPoint position, SDL_FRect &cam, float px, float py);
    void render(SDL_Renderer *renderer, float &pos_x, float &pos_y);
    void renderAristas(SDL_Renderer *renderer, int indx, int pos_x, int pos_y);
    void renderAristas(SDL_Renderer *renderer, Node *n1, Node *n2, int pos_x, int pos_y);
    void renderAristas(SDL_Renderer *renderer, SDL_FPoint p1, SDL_FPoint p2, int pos_x, int pos_y);
    void generarNodosAleatorios(int cantidad, float max_x, float max_y);
};

#endif