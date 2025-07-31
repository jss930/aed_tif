// #include "general.h"
#include "grafo.h"
#include "distancia_indice.h"
#include "sort.h"
#include "grid.h"
#include <algorithm>
#include <time.h>
#include <random>

int general_nodes_count = 0;

Grafo::Grafo(const int number_nodes) {
    int coalition_count = 0;
    this->num_nodes = number_nodes;
    this->max_x = WINDOW_WIDTH;
    float factor = (float)(number_nodes) / WINDOW_WIDTH;
    float porcentaje = factor * (RADIUS + GAP);
    if (porcentaje > 0.50 || RADIUS * num_nodes > WINDOW_WIDTH * 0.8)
        this->max_x += WINDOW_WIDTH * factor / 0.70;

    this->max_y = WINDOW_HEIGHT;
    factor = (float)(number_nodes) / WINDOW_HEIGHT;
    porcentaje = factor * (RADIUS + GAP);
    if (porcentaje > 0.75 || RADIUS * num_nodes > WINDOW_HEIGHT * 0.8)
        this->max_y += WINDOW_HEIGHT * factor / 0.8;

    srand(time(0));
    SDL_srand(time(0));
    this->map = new Node[number_nodes];
    this->mapy = new Node[number_nodes];

    for (int i = 0; i < number_nodes; i++) {
        map[i].shape.x = SDL_randf() * (max_x - RADIUS) + RADIUS;
        map[i].shape.y = SDL_randf() * (max_y - RADIUS) + RADIUS;

        if (coalition_count > 10) {
            printf("redimensionando área...\n");
            coalition_count = 0;
            max_x += WINDOW_WIDTH * 0.3f * (float)(i) / num_nodes;
            max_y += WINDOW_HEIGHT * 0.1f * (float)(i) / num_nodes;
        }

        if (circuleCoalition(map[i], map, i, GAP)) {
            coalition_count++;
            i--;
            continue;
        }
    }

    map = order_nodes(map, num_nodes, 'x');
    mapy = order_nodes(map, num_nodes, 'y');
    create_children();

    printf("[+] Grafo creado...\n");
    printf("number_nodos : %d\nwidth : %1.f\tmax_x : %2.f\nheigth : %1.f\tmax_y : %2.f\n", number_nodes, WINDOW_WIDTH, max_x, WINDOW_HEIGHT, max_y);
    printf("coalitions : %d\n", coalition_count);
}

Node *Grafo::order_nodes(Node *lista, int tam, char type) {
    if (tam <= 1) {
        Node *resultado = new Node[tam];
        if (tam == 1)
            resultado[0] = lista[0];
        return resultado;
    }

    int mitad = tam / 2;
    Node *temp1 = order_nodes(lista, mitad, type);
    Node *temp2 = order_nodes(lista + mitad, tam - mitad, type);

    Node *resultado = new Node[tam];
    int i = 0, j = 0, k = 0;
    while (i < mitad && j < tam - mitad) {
        float dist1 = (type == 'x') ? temp1[i].shape.x : temp1[i].shape.y;
        float dist2 = (type == 'x') ? temp2[j].shape.x : temp2[j].shape.y;

        if (dist1 < dist2)
            resultado[k++] = temp1[i++];
        else
            resultado[k++] = temp2[j++];
    }
    while (i < mitad)
        resultado[k++] = temp1[i++];
    while (j < tam - mitad)
        resultado[k++] = temp2[j++];

    delete[] temp1;
    delete[] temp2;
    return resultado;
}

int *Grafo::order_nodes_indices(Node *lista, int tam, char type) {
    if (tam <= 1) {
        int *res = new int[tam];
        if (tam == 1) res[0] = 0;
        return res;
    }

    int mitad = tam / 2;
    int *left = order_nodes_indices(lista, mitad, type);
    int *right = order_nodes_indices(lista + mitad, tam - mitad, type);

    int *res = new int[tam];
    int i = 0, j = 0, k = 0;

    while (i < mitad && j < tam - mitad) {
        float a = (type == 'x') ? lista[left[i]].shape.x : lista[left[i]].shape.y;
        float b = (type == 'x') ? lista[mitad + right[j]].shape.x : lista[mitad + right[j]].shape.y;

        if (a < b)
            res[k++] = left[i++];
        else
            res[k++] = mitad + right[j++];
    }

    while (i < mitad) res[k++] = left[i++];
    while (j < tam - mitad) res[k++] = mitad + right[j++];

    delete[] left;
    delete[] right;

    return res;
}

bool esta(int val, Vector<int> &lista) {
    for (size_t i = 0; i < lista.getSize(); i++) {
        if (val == lista[i])
            return true;
    }
    return false;
}

void Grafo::create_children() {
    const int NUM_CONEXIONES = 3;
    Grid2D grid(max_x, max_y);
    printf("Creando grid para max_x=%.2f, max_y=%.2f...\n", max_x, max_y);

    // Paso 1: Insertar todos los nodos en el grid
    int insertados_ok = 0;
    for (int i = 0; i < num_nodes; ++i) {
        float x = map[i].shape.x;
        float y = map[i].shape.y;

        if (x < 0 || y < 0 || x >= max_x || y >= max_y) {
            printf("[!] Nodo %d fuera de rango: x=%.2f y=%.2f\n", i, x, y);
        }

        int cx = (int)(x / CELL_SIZE);
        int cy = (int)(y / CELL_SIZE);

        printf("Nodo %d: (%.1f, %.1f) -> celda (%d,%d)\n", i, x, y, cx, cy);

        grid.insert(&map[i]);
        insertados_ok++;
    }

    printf("[✓] Nodos insertados: %d\n", insertados_ok);
    printf("Iniciando conexión de vecinos...\n");

    // Paso 2: Conectar cada nodo con sus vecinos más cercanos
    for (int i = 0; i < num_nodes; ++i) {
        Vector<Node*> vecinos;
        grid.getNearby(&map[i], vecinos);

        Vector<DistanciaIndice> distancias;

        for (size_t j = 0; j < vecinos.getSize(); ++j) {
            Node *otro = vecinos[j];
            if (otro == &map[i]) continue;

            float dx = map[i].shape.x - otro->shape.x;
            float dy = map[i].shape.y - otro->shape.y;
            float dist_squared = dx * dx + dy * dy;

            int idx = (int)(otro - map);  // índice desde puntero
            DistanciaIndice di = {dist_squared, idx};
            distancias.push_back(di);
        }

        if (distancias.getSize() == 0) {
            printf("[!] Nodo %d no tiene vecinos cercanos\n", i);
            continue;
        }

        sort(distancias, compararDistancia);

        int conexiones = 0;
        for (int k = 0; k < distancias.getSize() && conexiones < NUM_CONEXIONES; ++k) {
            int idx = distancias[k].idx;
            Node *otro = &map[idx];

            bool ya_conectado = false;
            for (size_t m = 0; m < map[i].childrens.getSize(); ++m) {
                if (map[i].childrens[m] == otro) {
                    ya_conectado = true;
                    break;
                }
            }

            if (!ya_conectado) {
                map[i].childrens.push_back(otro);
                otro->childrens.push_back(&map[i]);
                conexiones++;
            }
        }

        if (map[i].childrens.getSize() < NUM_CONEXIONES) {
            printf("[!] Nodo %d quedó con menos de %d conexiones\n", i, NUM_CONEXIONES);
        }

        if (i % 1000 == 0)
            printf("Procesando nodo %d/%d\n", i, num_nodes);
    }

    printf("[✓] Conexiones completadas.\n");
}

bool Grafo::circuleCoalition(Node &tarjet, Node *list, int idx, float gap) {
    float radius_from = SDL_pow(RADIUS * 2 + gap, 2);
    for (int i = 0; i < num_nodes; i++) {
        if (&tarjet == &list[i] || i == idx)
            continue;
        float distance = SDL_pow(tarjet.shape.x - list[i].shape.x, 2) +
                         SDL_pow(tarjet.shape.y - list[i].shape.y, 2);
        if (distance < radius_from)
            return true;
    }
    return false;
}

bool Grafo::frustum(Node &node, SDL_FRect &cam, float px, float py) {
    return ((node.shape.x + px - PADDING >= cam.x) && (node.shape.x + px - PADDING <= (cam.x + cam.w)) &&
            (node.shape.y + py - PADDING >= cam.y) && (node.shape.y + py - PADDING <= (cam.y + cam.h)));
}

void Grafo::render(SDL_Renderer *renderer, float &pos_x, float &pos_y) {
    int count_nodes_renderer = 0;
    SDL_FRect cam = {-PADDING - RADIUS - GAP, -PADDING - RADIUS - GAP,
                     WINDOW_WIDTH + PADDING + RADIUS + GAP, WINDOW_HEIGHT + PADDING + RADIUS + GAP};

    SDL_RenderFillRect(renderer, &cam);

    float radius_from = SDL_pow(RADIUS, 2);
    for (int i = 0; i < num_nodes; i++) {
        if (frustum(map[i], cam, pos_x, pos_y)) {
            for (size_t j = 0; j < map[i].childrens.getSize(); j++) {
                renderAristas(renderer, &map[i], map[i].childrens[j], pos_x, pos_y);
            }
            count_nodes_renderer++;

            SDL_SetRenderDrawColor(renderer, COLOR_GREY, SDL_ALPHA_OPAQUE);
            for (float x = map[i].shape.x - RADIUS; x < map[i].shape.x + RADIUS; x++)
                for (float y = map[i].shape.y - RADIUS; y < map[i].shape.y + RADIUS; y++) {
                    float distance = SDL_pow((x - map[i].shape.x), 2) + SDL_pow((y - map[i].shape.y), 2);
                    if (distance < radius_from) {
                        SDL_FRect pixel = {x + pos_x, y + pos_y, 1, 1};
                        SDL_RenderFillRect(renderer, &pixel);
                    }
                }
        }
    }

    if (general_nodes_count != count_nodes_renderer) {
        printf("%d nodos renderizados\n", count_nodes_renderer);
        general_nodes_count = count_nodes_renderer;
    }
}

void Grafo::renderAristas(SDL_Renderer *renderer, int idx, int pos_x, int pos_y) {
    for (size_t i = 0; i < map[idx].childrens.getSize(); i++) {
        SDL_SetRenderDrawColor(renderer, COLOR_BLACK, SDL_ALPHA_OPAQUE);
        SDL_RenderLine(renderer, map[idx].shape.x + pos_x, map[idx].shape.y + pos_y,
                       map[idx].childrens[i]->shape.x + pos_x, map[idx].childrens[i]->shape.y + pos_y);
    }
}

void Grafo::renderAristas(SDL_Renderer *renderer, Node *n1, Node *n2, int pos_x, int pos_y) {
    SDL_SetRenderDrawColor(renderer, COLOR_GREY, SDL_ALPHA_OPAQUE);
    SDL_RenderLine(renderer, n1->shape.x + pos_x, n1->shape.y + pos_y,
                   n2->shape.x + pos_x, n2->shape.y + pos_y);
}

Grafo::~Grafo() {
    if (map) delete[] map;
    if (mapy) delete[] mapy;
}