#include "grafo.h"
#include "distancia_indice.h"
#include "sort.h"
#include "grid.h"
#include "myvector.h"
#include <time.h>
#include <random>
#include <cmath>
#include <thread>
#include <future>
#include <mutex>
// #include <vector>
#include <SDL2/SDL.h>
#include "delaunator.hpp"

int general_nodes_count = 0;

Grafo::Grafo(const int number_nodes)
{
    int coalition_count = 0;
    this->num_nodes = number_nodes;
<<<<<<< HEAD
    this->max_x = WINDOW_WIDTH;
    this->nodos_renderizados = new Vector<Node *>;
    float factor = (float)(number_nodes) / WINDOW_WIDTH;
    float porcentaje = factor * (RADIUS + GAP);
    if (porcentaje > 0.50 || RADIUS * num_nodes > WINDOW_WIDTH * 0.8)
        this->max_x += WINDOW_WIDTH * factor / 0.70;
=======
>>>>>>> 00b7f84d6b2267a77abdb0b3f2f1ae996cfb6dfc

    // Área inicial más conservadora
    this->max_x = WINDOW_WIDTH;
    this->max_y = WINDOW_HEIGHT;

    // Solo expandir si realmente es necesario
    float density_factor = (float)(number_nodes) / (WINDOW_WIDTH * WINDOW_HEIGHT) * (RADIUS + GAP) * (RADIUS + GAP);
    if (density_factor > 0.3)
    {
        float expansion = sqrt(density_factor / 0.3);
        this->max_x *= expansion;
        this->max_y *= expansion;
    }

    srand(time(0));
    SDL_srand(time(0));
    this->map = new Node[number_nodes];
    this->mapy = new Node[number_nodes];

    std::mt19937 gen((unsigned)time(0));
    std::uniform_real_distribution<float> dist_x(RADIUS, max_x - RADIUS);
    std::uniform_real_distribution<float> dist_y(RADIUS, max_y - RADIUS);

    float cantidad_colision = 1;
    for (int i = 0; i < number_nodes; i++)
    {
        bool placed = false;
        int attempts = 0;
        const int max_attempts = 100;

        while (!placed && attempts < max_attempts)
        {
            map[i].shape.x = dist_x(gen);
            map[i].shape.y = dist_y(gen);

            if (!circuleCoalition(map[i], map, i, GAP))
            {
                placed = true;
            }
            else
            {
                attempts++;
                coalition_count++;
            }
        }

        // Si no se pudo colocar después de muchos intentos, expandir área MÍNIMAMENTE
        if (!placed)
        {
            cantidad_colision++;
            printf("redimensionando área... (nodo %d)\n", i);
            coalition_count = 0;

            // Expansión mucho más pequeña y progresiva
            max_x += WINDOW_WIDTH * 0.05f;  // 5% en lugar de 30%
            max_y += WINDOW_HEIGHT * 0.05f; // 5% en lugar de 20%

            // Actualizar la distribución con el nuevo área
            dist_x = std::uniform_real_distribution<float>(RADIUS, max_x - RADIUS);
            dist_y = std::uniform_real_distribution<float>(RADIUS, max_y - RADIUS);

            // Intentar colocar el nodo en el área expandida
            i--; // Reintentar este nodo
            continue;
        }
    }

    this->map = order_nodes(this->map, num_nodes, 'x');
    this->mapy = order_nodes(this->map, num_nodes, 'y');
    create_children_parallel();

    printf("[+] Grafo creado...\n");
    printf("number_nodos : %d\nwidth : %.1f\tmax_x : %.2f\nheight : %.1f\tmax_y : %.2f\n",
           number_nodes, WINDOW_WIDTH, max_x, WINDOW_HEIGHT, max_y);
    printf("coalitions : %d\n", coalition_count);
}

void Grafo::create_children_parallel()
{
    std::vector<double> coords;
    for (int i = 0; i < num_nodes; ++i)
    {
        coords.push_back(map[i].shape.x);
        coords.push_back(map[i].shape.y);
    }

    delaunator::Delaunator d(coords);

    const int NUM_CONEXIONES = 3;
    std::mutex mtx;
    int nthreads = std::min(unsigned(std::thread::hardware_concurrency()), unsigned(num_nodes));
    int chunk = num_nodes / nthreads;

    std::vector<std::future<void>> workers;

    for (int t = 0; t < nthreads; ++t)
    {
        int start = t * chunk;
        int end = (t == nthreads - 1) ? num_nodes : start + chunk;

        workers.push_back(std::async(std::launch::async, [=, &mtx, this, &d]()
                                     {
            for (int i = start; i < end; ++i)
            {
                Vector<Node *> vecinos;
                for (size_t k = 0; k < d.triangles.size(); k += 3)
                {
                    int i0 = d.triangles[k];
                    int i1 = d.triangles[k + 1];
                    int i2 = d.triangles[k + 2];
                    if (i0 == i || i1 == i || i2 == i)
                    {
                        if (i0 != i)
                            vecinos.push_back(&map[i0]);
                        if (i1 != i)
                            vecinos.push_back(&map[i1]);
                        if (i2 != i)
                            vecinos.push_back(&map[i2]);
                    }
                }

                Vector<DistanciaIndice> distancias;
                for (size_t j = 0; j < vecinos.getSize(); ++j)
                {
                    Node *otro = vecinos[j];
                    float dx = map[i].shape.x - otro->shape.x;
                    float dy = map[i].shape.y - otro->shape.y;
                    float dist_squared = dx * dx + dy * dy;
                    int idx = (int)(otro - map);
                    distancias.push_back({dist_squared, idx});
                }

                sort(distancias, compararDistancia);
                int conexiones = 0;

                std::lock_guard<std::mutex> lock(mtx);
                for (int k = 0; k < distancias.getSize() && conexiones < NUM_CONEXIONES; ++k)
                {
                    int idx = distancias[k].idx;
                    Node *otro = &map[idx];

                    bool ya_conectado = false;
                    for (size_t m = 0; m < map[i].childrens.getSize(); ++m)
                    {
                        if (map[i].childrens[m] == otro)
                        {
                            ya_conectado = true;
                            break;
                        }
                    }

                    if (!ya_conectado)
                    {
                        map[i].childrens.push_back(otro);
                        otro->childrens.push_back(&map[i]);
                        conexiones++;
                    }
                }
            } }));
    }

    for (auto &fut : workers)
        fut.get();

    printf("Conexiones completadas (paralelo).\n");
}

Node *Grafo::order_nodes(Node *lista, int tam, char type)
{
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

bool Grafo::frustum(Node &node, SDL_FRect &cam, float px, float py)
{
    return ((node.shape.x + px - PADDING >= cam.x) && (node.shape.x + px - PADDING <= (cam.x + cam.w)) &&
            (node.shape.y + py - PADDING >= cam.y) && (node.shape.y + py - PADDING <= (cam.y + cam.h)));
}

void Grafo::render(SDL_Renderer *renderer, float &pos_x, float &pos_y)
{
    Vector<Node *> *tmp_nodos_renderizados = new Vector<Node *>;
    int count_nodes_renderer = 0;
    SDL_FRect cam = {-PADDING - RADIUS - GAP, -PADDING - RADIUS - GAP,
                     WINDOW_WIDTH + PADDING + RADIUS + GAP, WINDOW_HEIGHT + PADDING + RADIUS + GAP};

    SDL_RenderFillRect(renderer, &cam);

    float radius_from = SDL_pow(RADIUS, 2);
    for (int i = 0; i < num_nodes; i++)
    {
        if (frustum(map[i], cam, pos_x, pos_y))
        {
            // Dibujar aristas
            for (size_t j = 0; j < map[i].childrens.getSize(); j++)
            {
                renderAristas(renderer, &map[i], map[i].childrens[j], pos_x, pos_y);
            }

            // Guardar nodo visible
            count_nodes_renderer++;
            tmp_nodos_renderizados->push_back(&map[i]);

            // Seleccionar color según tipo
            if (&map[i] == tarjet)
                SDL_SetRenderDrawColor(renderer, COLOR_RED, SDL_ALPHA_OPAQUE);
            else if (&map[i] == start)
                SDL_SetRenderDrawColor(renderer, COLOR_VERDE, SDL_ALPHA_OPAQUE);
            else
                SDL_SetRenderDrawColor(renderer, COLOR_GREY, SDL_ALPHA_OPAQUE);

            // Dibujar nodo como círculo
            for (float x = map[i].shape.x - RADIUS; x < map[i].shape.x + RADIUS; x++)
                for (float y = map[i].shape.y - RADIUS; y < map[i].shape.y + RADIUS; y++)
                {
                    float distance = SDL_pow((x - map[i].shape.x), 2) + SDL_pow((y - map[i].shape.y), 2);
                    if (distance < radius_from)
                    {
                        SDL_FRect pixel = {x + pos_x, y + pos_y, 1, 1};
                        SDL_RenderFillRect(renderer, &pixel);
                    }
                }
        }
    }

    // Dibujar ruta si existe
    if (start && tarjet && tarjet->parent)
    {
        SDL_SetRenderDrawColor(renderer, COLOR_BLUE, SDL_ALPHA_OPAQUE);
        for (Node *n = tarjet; n->parent != nullptr; n = n->parent)
        {
            SDL_RenderLine(renderer,
                           n->shape.x + pos_x, n->shape.y + pos_y,
                           n->parent->shape.x + pos_x, n->parent->shape.y + pos_y);
        }
    }

    // Actualizar nodos renderizados visibles
    if (general_nodes_count != count_nodes_renderer)
    {
        if (this->nodos_renderizados)
            delete this->nodos_renderizados;
        this->nodos_renderizados = tmp_nodos_renderizados;

        printf("%d nodos renderizados\n", count_nodes_renderer);
        general_nodes_count = count_nodes_renderer;
    }
    else
    {
        delete tmp_nodos_renderizados;
    }
}

void Grafo::renderAristas(SDL_Renderer *renderer, int idx, int pos_x, int pos_y)
{
    for (size_t i = 0; i < map[idx].childrens.getSize(); i++)
    {
        SDL_SetRenderDrawColor(renderer, COLOR_BLACK, SDL_ALPHA_OPAQUE);
        SDL_RenderLine(renderer, map[idx].shape.x + pos_x, map[idx].shape.y + pos_y,
                       map[idx].childrens[i]->shape.x + pos_x, map[idx].childrens[i]->shape.y + pos_y);
    }
}

void Grafo::renderAristas(SDL_Renderer *renderer, Node *n1, Node *n2, int pos_x, int pos_y)
{
    SDL_SetRenderDrawColor(renderer, COLOR_GREY, SDL_ALPHA_OPAQUE);
    SDL_RenderLine(renderer, n1->shape.x + pos_x, n1->shape.y + pos_y,
                   n2->shape.x + pos_x, n2->shape.y + pos_y);
}

void Grafo::selectNodo(float posX, float posY, std::string tipo, int pos_x, int pos_y)
{
    printf("%.2f x - %.2f y   ", posX, posY);

    float radius_squared = SDL_pow(RADIUS * 2, 2) + 3;
    if (!this->nodos_renderizados) return;

    for (int i = 0; i < this->nodos_renderizados->getSize(); i++) {
        Node *nodo = (*this->nodos_renderizados)[i];


        float dx = nodo->shape.x + pos_x - posX;
        float dy = nodo->shape.y + pos_y - posY;
        float distance_squared = dx * dx + dy * dy;

        if (distance_squared <= radius_squared)
        {
            if (tipo == "inicio")
            {
                this->start = nodo;
            }
            else if (tipo == "final")
            {
                this->tarjet = nodo;
            }
            if (this->tarjet == this->start)
                if (tipo == "final")
                    this->start = NULL;
                else
                    this->tarjet = NULL;
            printf("Nodo %s seleccionado en %.2f x - %.2f y\n", tipo.c_str(), nodo->shape.x, nodo->shape.y);
            return;
        }
    }
}

Grafo::~Grafo()
{
    if (map)
        delete[] map;
    if (mapy)
        delete[] mapy;
}

bool Grafo::buscarRutaBFS()
{
    if (!start || !tarjet)
        return false;

    // Resetear nodos
    for (int i = 0; i < num_nodes; i++) {
        map[i].visited = false;
        map[i].parent = nullptr;
    }

    Vector<Node*> queue;
    start->visited = true;
    queue.push_back(start);

    int front = 0;
    while (front < queue.getSize()) {
        Node* current = queue[front++];
        if (current == tarjet)
            break;

        for (size_t i = 0; i < current->childrens.getSize(); ++i) {
            Node* neighbor = current->childrens[i];
            if (!neighbor->visited) {
                neighbor->visited = true;
                neighbor->parent = current;
                queue.push_back(neighbor);
            }
        }
    }

    return tarjet->parent != nullptr;
}
