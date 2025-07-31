// #include "general.h"
#include "grafo.h"
#include "distancia_indice.h"
#include "sort.h"
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

bool esta(int val, Vector<int> &lista) {
    for (size_t i = 0; i < lista.getSize(); i++) {
        if (val == lista[i])
            return true;
    }
    return false;
}

void Grafo::create_children() {
    const int VENTANA = 15 + (num_nodes / 10) % 50;
    const int NUM_CONEXIONES = 3;

    for (int i = 0; i < num_nodes; i++) {
        Vector<DistanciaIndice> distancias;
        Vector<int> insertados;

        for (int j = (i - VENTANA > 0 ? i - VENTANA : 0); j <= (i + VENTANA < num_nodes ? i + VENTANA : num_nodes - 1); j++) {
            if (i == j) continue;
            float dx = map[i].shape.x - map[j].shape.x;
            float dy = map[i].shape.y - map[j].shape.y;
            float dist_squared = dx * dx + dy * dy;

            DistanciaIndice di = {dist_squared, j};
            distancias.push_back(di);
            insertados.push_back(j);
        }

        int pos_en_mapy = -1;
        for (int k = 0; k < num_nodes; k++) {
            if (mapy[k].shape.x == map[i].shape.x && mapy[k].shape.y == map[i].shape.y) {
                pos_en_mapy = k;
                break;
            }
        }

        if (pos_en_mapy != -1) {
            for (int j = (pos_en_mapy - VENTANA > 0 ? pos_en_mapy - VENTANA : 0);
                 j <= (pos_en_mapy + VENTANA < num_nodes ? pos_en_mapy + VENTANA : num_nodes - 1); j++) {
                int idx_en_map = -1;
                for (int k = 0; k < num_nodes; k++) {
                    if (map[k].shape.x == mapy[j].shape.x && map[k].shape.y == mapy[j].shape.y) {
                        idx_en_map = k;
                        break;
                    }
                }

                if (idx_en_map == -1 || idx_en_map == i || esta(idx_en_map, insertados)) continue;

                float dx = map[i].shape.x - map[idx_en_map].shape.x;
                float dy = map[i].shape.y - map[idx_en_map].shape.y;
                float dist_squared = dx * dx + dy * dy;

                DistanciaIndice di = {dist_squared, idx_en_map};
                distancias.push_back(di);
                insertados.push_back(idx_en_map);
            }
        }

        sort(distancias, compararDistancia);

        for (int k = 0; k < NUM_CONEXIONES && k < distancias.getSize(); k++) {
            int idx = distancias[k].idx;
            bool ya_conectado = false;
            for (size_t c = 0; c < map[i].childrens.getSize(); ++c) {
                if (map[i].childrens[c] == &map[idx]) {
                    ya_conectado = true;
                    break;
                }
            }

            if (!ya_conectado) {
                map[i].childrens.push_back(&map[idx]);
                map[idx].childrens.push_back(&map[i]);
            }
        }

        if (map[i].childrens.getSize() < NUM_CONEXIONES) {
            i--;  // reintentar nodo
        }
    }
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