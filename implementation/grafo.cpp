// #include "general.h"

#include "../interface/grafo.h"
#include "../interface/node_element.h"
#include "../interface/ADT/node_hash.h"
#include <iostream>
#include <algorithm>
#include <time.h>
#include <cstdlib>
#include <ctime>
#include <random>

int general_nodes_count = 0;

// creates num_nodes nodes in a random position, avoiding coalitions
Grafo::Grafo(const int number_nodes)
{
    int coalition_count = 0;
    this->num_nodes = number_nodes;
    this->max_x = WINDOW_WIDTH;
    float factor = (float)(number_nodes) / WINDOW_WIDTH;
    float porcentaje = factor * (RADIUS + GAP);
    if (porcentaje > 0.50 || RADIUS * num_nodes > WINDOW_WIDTH * 0.8)
        this->max_x += WINDOW_WIDTH * factor / 0.70;
    // this->max_x += WINDOW_WIDTH * 0.3;

    this->max_y = WINDOW_HEIGHT;
    factor = (float)(number_nodes) / WINDOW_HEIGHT;
    porcentaje = factor * (RADIUS + GAP);
    if (porcentaje > 0.75 || RADIUS * num_nodes > WINDOW_HEIGHT * 0.8)
        this->max_y += WINDOW_HEIGHT * factor / 0.8;
    //! editado para testear
    // max_x *= 1.2;
    // max_y *= 1;

    srand(time(0));
    SDL_srand(time(0));
    this->map = new Node[number_nodes];
    this->mapy = new Node[number_nodes];
    for (int i = 0; i < number_nodes; i++)
    {
        // map[i] = Node(SDL_randf() * max_x, SDL_randf() * max_y);

        map[i].shape.x = SDL_randf() * (max_x - RADIUS) + RADIUS;
        map[i].shape.y = SDL_randf() * (max_y - RADIUS) + RADIUS;

        if (coalition_count > 10)

        {
            printf("redimencionando area...\n");
            coalition_count = 0;
            max_x += WINDOW_WIDTH * 0.3 * (float)(i) / num_nodes;
            max_y += WINDOW_HEIGHT * 0.1 * (float)(i) / num_nodes;
        }
        if (circuleCoalition(map[i], map, i, GAP))
        {
            coalition_count++;
            i--;
            continue;
        }
        // printf("nodo %d: x= %2.f  y=%2.f\n", i, map[i].shape.x, map[i].shape.y);
    }

    //! order array, to improve generation of array of frustrum and children
    map = order_nodes(map, num_nodes, 'x');
    mapy = order_nodes(map, num_nodes, 'y');
    create_children();
    /*     for (int i = 0; i < num_nodes; i++)
        {
            printf("nodo %d: x= %2.f  y=%2.f \t==\t %2.f\n", i, map[i].shape.x, map[i].shape.y, map[i].shape.x + map[i].shape.y);
            // for (int j = 0; j < map[i].childrens.size(); j++)
            //{
            //     printf("\t x: %2.f   y: %2.f\n", map[i].childrens[j]->shape.x, map[i].childrens[j]->shape.y);
            // }
        } */

    printf("[+] Grafo creado...\n");
    printf("number_nodos : %d\nwidth : %1.f\tmax_x : %2.f\nheigth : %1.f\tmax_y : %2.f\n", number_nodes, WINDOW_WIDTH, max_x, WINDOW_HEIGHT, max_y);
    printf("coalitions : %d\n", coalition_count);
}

// orders the nodes in a list based on the X or Y coordinate by consecutive mergesort
Node *Grafo::order_nodes(Node *lista, int tam, char type)
{
    // int *merge(int *lista, int tam)

    if (tam <= 1)
    {
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
    while (i < mitad && j < tam - mitad)
    {
        float dist1;
        float dist2;
        if (type == 'x')
        {
            dist1 = temp1[i].shape.x;
            dist2 = temp2[j].shape.x;
        }
        else if (type == 'y')
        {
            dist1 = temp1[i].shape.y;
            dist2 = temp2[j].shape.y;
        }
        // float dist1 = temp1[i].shape.x * temp1[i].shape.x + temp1[i].shape.y * temp1[i].shape.y;
        // float dist2 = temp2[j].shape.x * temp2[j].shape.x + temp2[j].shape.y * temp2[j].shape.y;

        // if (temp1[i].shape.x + temp1[i].shape.y < temp2[j].shape.x + temp2[j].shape.y)
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

Grafo::~Grafo()
{
    if (map)
        delete[] map;
    if (mapy)
        delete[] mapy;
}

/* void Grafo::create_children()
{
    printf("se crean hijos\n");
    std::vector<Node *> visitados;
    for (int i = 0; i < num_nodes; i++)
    {

        float min_distance = std::numeric_limits<float>::max();
        int idx[3] = {-1};
        for (int j = 0; j < num_nodes; j++)
        {
            if (i == j)
                continue;
            float dx = map[i].shape.x - map[j].shape.x;
            float dy = map[i].shape.y - map[j].shape.y;
            float dist_squared = SDL_powf(dx, 2) + SDL_powf(dy, 2);
            if (dist_squared < min_distance)
            {
                min_distance = dist_squared;
                idx = j;

                printf("distance %d - %d = %.2f\n", i, j, dist_squared);
            }


        }
        if (idx != -1)
        {
            map[i].childrens.push_back(map[idx]);
            map[idx].childrens.push_back(map[i]);
        }
    }
} */

void Grafo::insertarNodo(int id, float x, float y) {
    NodeElement nuevo(id, x, y);
    nodos.insert(nuevo);
}

void Grafo::conectar(int id1, int id2) {
    NodeElement* n1 = nodos.get(NodeElement{id1});
    NodeElement* n2 = nodos.get(NodeElement{id2});

    if (!n1 || !n2) return;

    // Evitar conexiones duplicadas
    if (std::find(n1->vecinos.begin(), n1->vecinos.end(), id2) == n1->vecinos.end())
        n1->vecinos.push_back(id2);

    if (std::find(n2->vecinos.begin(), n2->vecinos.end(), id1) == n2->vecinos.end())
        n2->vecinos.push_back(id1);
}


bool Grafo::existeNodo(int id) {
    return nodos.find(NodeElement{id});
}

void Grafo::imprimirVecinos(int id) {
    if (!nodos.find(NodeElement{id})) {
        std::cout << "Nodo " << id << " no encontrado\n";
        return;
    }
}

bool Grafo::colisionaConNodosExistentes(float x, float y, float radio) {
    std::vector<NodeElement> existentes = nodos.getAllElements();
    float r2 = (radio + GAP) * (radio + GAP);

    for (auto& nodo : existentes) {
        float dx = nodo.position.x - x;
        float dy = nodo.position.y - y;
        if (dx * dx + dy * dy < r2) return true;
    }
    return false;
}

bool esta(int val, std::vector<int> lista)
{
    for (int i : lista)
    {
        if (val == i)
            return true;
    }
    return false;
}
// creates children for each node based on nearby nodes (limited to NUM_CONEXIONES))
void Grafo::create_children()
{
    // const int VENTANA = 3;
    const int VENTANA = 15 + (num_nodes / 10) % 50;
    const int NUM_CONEXIONES = 3;
    for (int i = 0; i < num_nodes; i++)
    {
        std::vector<std::pair<float, int>> distancias;
        std::vector<int> insertados;
        // ! eliminar función STL
        for (int j = std::max(0, i - VENTANA); j <= std::min(num_nodes - 1, i + VENTANA); j++)
        {
            if (i == j)
                continue;

            float dx = map[i].shape.x - map[j].shape.x;
            float dy = map[i].shape.y - map[j].shape.y;
            float dist_squared = dx * dx + dy * dy;

            distancias.emplace_back(dist_squared, j);
            insertados.push_back(j);
        }

        int pos_en_mapy = -1;
        for (int k = 0; k < num_nodes; k++)
        {
            if (mapy[k].shape.x == map[i].shape.x && mapy[k].shape.y == map[i].shape.y)
            {
                pos_en_mapy = k;
                break;
            }
        }

        if (pos_en_mapy != -1)
        {
            // ! eliminar función STL
            for (int j = std::max(0, pos_en_mapy - VENTANA); j <= std::min(num_nodes - 1, pos_en_mapy + VENTANA); j++)
            {
                int idx_en_map = -1;
                for (int k = 0; k < num_nodes; k++)
                {
                    if (map[k].shape.x == mapy[j].shape.x && map[k].shape.y == mapy[j].shape.y)
                    {
                        idx_en_map = k;
                        break;
                    }
                }

                if (idx_en_map == -1 || idx_en_map == i || esta(idx_en_map, insertados))
                    continue;

                float dx = map[i].shape.x - map[idx_en_map].shape.x;
                float dy = map[i].shape.y - map[idx_en_map].shape.y;
                float dist_squared = dx * dx + dy * dy;

                distancias.emplace_back(dist_squared, idx_en_map);
                insertados.push_back(idx_en_map);
            }
        }
        // ! eliminar función STL
        std::sort(distancias.begin(), distancias.end());

        for (int k = 0; k < NUM_CONEXIONES && k < distancias.size(); k++)
        {
            int idx = distancias[k].second;

            bool ya_conectado = false;
            for (auto *child : map[i].childrens)
            {
                if (child == &map[idx])
                {
                    ya_conectado = true;
                    break;
                }
            }

            if (!ya_conectado)
            {
                map[i].childrens.push_back(&map[idx]);
                map[idx].childrens.push_back(&map[i]);
            }
        }
        if (map[i].childrens.size() < NUM_CONEXIONES)
        {
            i--;
        }
    }
}

// checks if the target node is in a coalition with other nodes
bool Grafo::circuleCoalition(Node &tarjet, Node *list, int idx, float gap)
{
    float radius_from = SDL_pow(RADIUS * 2 + gap, 2);
    for (int i = 0; i < num_nodes; i++)
    {
        if (&tarjet == &list[i] || i == idx)
            continue;
        float distance = SDL_pow(tarjet.shape.x - list[i].shape.x, 2) +
                         SDL_pow(tarjet.shape.y - list[i].shape.y, 2);
        if (distance < radius_from)
            return true;
    }
    return false;
}

// checks if the node is in the frustum of the camera
bool Grafo::frustum(SDL_FPoint position, SDL_FRect& cam, float px, float py) {
    return ((position.x + px - PADDING >= cam.x) && (position.x + px - PADDING <= (cam.x + cam.w)) &&
            (position.y + py - PADDING >= cam.y) && (position.y + py - PADDING <= (cam.y + cam.h)));
}


// renders the nodes and their connections
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


// renders the edges of the graph from a specific index
void Grafo::renderAristas(SDL_Renderer *renderer, int idx, int pos_x, int pos_y)
{
    for (int i = 0; i < map[idx].childrens.size(); i++)
    {
        SDL_SetRenderDrawColor(renderer, COLOR_BLACK, SDL_ALPHA_OPAQUE);
        SDL_RenderLine(renderer, map[idx].shape.x + pos_x, map[idx].shape.y + pos_y,
                       map[i].childrens[i]->shape.x + pos_x, map[i].childrens[i]->shape.y + pos_y);
    }
}

void Grafo::renderAristas(SDL_Renderer *renderer, Node *n1, Node *n2, int pos_x, int pos_y)
{
    SDL_SetRenderDrawColor(renderer, COLOR_GREY, SDL_ALPHA_OPAQUE);
    SDL_RenderLine(renderer, n1->shape.x + pos_x, n1->shape.y + pos_y,
                   n2->shape.x + pos_x, n2->shape.y + pos_y);
}

void Grafo::renderAristas(SDL_Renderer* renderer, SDL_FPoint p1, SDL_FPoint p2, int pos_x, int pos_y) {
    SDL_SetRenderDrawColor(renderer, COLOR_GREY, SDL_ALPHA_OPAQUE);
    SDL_RenderLine(renderer, p1.x + pos_x, p1.y + pos_y, p2.x + pos_x, p2.y + pos_y);
}

void Grafo::generarNodosAleatorios(int cantidad, float max_x, float max_y) {
    srand(time(nullptr));

    // Insertar nodos
    for (int i = 0; i < cantidad; i++) {
        float x, y;
        int intentos = 0;
        do {
            x = static_cast<float>(rand()) / RAND_MAX * (max_x - RADIUS) + RADIUS;
            y = static_cast<float>(rand()) / RAND_MAX * (max_y - RADIUS) + RADIUS;
            intentos++;
        } while (colisionaConNodosExistentes(x, y, RADIUS) && intentos < 100);

        insertarNodo(i, x, y);
    }

    // Conectar nodos a 2–3 vecinos al azar
    for (int i = 0; i < cantidad; i++) {
        std::vector<int> posibles_vecinos;
        for (int j = 0; j < cantidad; j++) {
            if (j != i) posibles_vecinos.push_back(j);
        }

        std::shuffle(posibles_vecinos.begin(), posibles_vecinos.end(), std::default_random_engine(std::random_device{}()));

        int conexiones = 2 + rand() % 2; // 2 o 3
        for (int k = 0; k < conexiones && k < posibles_vecinos.size(); k++) {
            conectar(i, posibles_vecinos[k]);
        }
    }

    printf("[+] %d nodos generados y conectados aleatoriamente.\n", cantidad);
}