#include "../interface/grafo.h"
#include <iostream>

Grafo::Grafo(int num_nodes) {
    this->num_nodes = num_nodes;
}

void Grafo::insertarNodo(int id, float x, float y) {
    NodeElement nuevo(id, x, y);
    nodos.insert(nuevo);
}

void Grafo::conectar(int id1, int id2) {
    if (!nodos.find(NodeElement{id1})) return;
    if (!nodos.find(NodeElement{id2})) return;

    // Aquí necesitaríamos una función de acceso para modificar nodos
    // Alternativa: mejorar HashTable para permitir obtener referencias
    std::cout << "Conectando " << id1 << " <-> " << id2 << std::endl;
}

bool Grafo::existeNodo(int id) {
    return nodos.find(NodeElement{id});
}

void Grafo::imprimirVecinos(int id) {
    if (!nodos.find(NodeElement{id})) {
        std::cout << "Nodo " << id << " no encontrado\n";
        return;
    }

    // De nuevo, necesitas poder acceder al objeto completo
}

void Grafo::render(SDL_Renderer* renderer, float& pos_x, float& pos_y) {
    int count_nodes_renderer = 0;
    SDL_FRect cam = {-PADDING - RADIUS - GAP, -PADDING - RADIUS - GAP,
                      WINDOW_WIDTH + PADDING + RADIUS + GAP,
                      WINDOW_HEIGHT + PADDING + RADIUS + GAP};

    SDL_RenderFillRect(renderer, &cam);

    float radius_from = SDL_pow(RADIUS, 2);

    std::vector<NodeElement> lista = nodos.getAllElements();

    for (auto& nodo : lista) {
        if (frustum(nodo.position, cam, pos_x, pos_y)) {
            // Renderizar conexiones
            for (int vecino_id : nodo.vecinos) {
                NodeElement* vecino = nodos.get(NodeElement{vecino_id});
                if (vecino) {
                    renderAristas(renderer, nodo.position, vecino->position, pos_x, pos_y);
                }
            }

            // Renderizar nodo como círculo
            SDL_SetRenderDrawColor(renderer, COLOR_GREY, SDL_ALPHA_OPAQUE);
            for (float x = nodo.position.x - RADIUS; x < nodo.position.x + RADIUS; x++) {
                for (float y = nodo.position.y - RADIUS; y < nodo.position.y + RADIUS; y++) {
                    float distance = SDL_pow((x - nodo.position.x), 2) + SDL_pow((y - nodo.position.y), 2);
                    if (distance < radius_from) {
                        SDL_FRect pixel = {x + pos_x, y + pos_y, 1, 1};
                        SDL_RenderFillRect(renderer, &pixel);
                    }
                }
            }

            count_nodes_renderer++;
        }
    }

    if (general_nodes_count != count_nodes_renderer) {
        printf("%d nodos renderizados\n", count_nodes_renderer);
        general_nodes_count = count_nodes_renderer;
    }
}

