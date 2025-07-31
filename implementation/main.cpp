#include "../interface/grafo.h"
#include <iostream>

int main() {
    Grafo g(5);
    g.insertarNodo(1, 100, 200);
    g.insertarNodo(2, 150, 250);

    g.conectar(1, 2);

    std::cout << "¿Existe nodo 1? " << (g.existeNodo(1) ? "Sí" : "No") << "\n";
    std::cout << "¿Existe nodo 5? " << (g.existeNodo(5) ? "Sí" : "No") << "\n";

    return 0;
}
