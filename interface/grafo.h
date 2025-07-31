#ifndef __GRAFO__
#define __GRAFO__

#include "ADT/hash_table.h"
#include "ADT/linked_list.h"
#include "ADT/node_hash.h"
#include "node_element.h"

#define MAX_SIZE 2000000

class Grafo {
private:
    HashTable<NodeElement, NodeHash, LinkedList<NodeElement>, MAX_SIZE> nodos;
    int num_nodes;

public:
    Grafo(int num_nodes);
    void insertarNodo(int id, float x, float y);
    void conectar(int id1, int id2);
    bool existeNodo(int id);
    void imprimirVecinos(int id);
};

#endif