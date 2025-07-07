// #include "general.h"
#include "grafo.h"
#include <time.h>
#include <random>

int general_nodes_count = 0;

Grafo::Grafo(const int number_nodes)
{
    int coalition_count = 0;
    this->num_nodes = number_nodes;
    this->max_x = WINDOW_WIDTH;
    float factor = (float)number_nodes / WINDOW_WIDTH;
    if (factor > 0.75 || RADIUS * num_nodes > WINDOW_WIDTH * 0.8)
        this->max_x += WINDOW_WIDTH * 0.3 * factor / 0.75;

    this->max_y = WINDOW_HEIGHT;
    factor = (float)number_nodes / WINDOW_HEIGHT;
    if (factor > 0.98 || RADIUS * num_nodes > WINDOW_HEIGHT * 0.8)
        this->max_y += WINDOW_HEIGHT * 0.1 * factor / 0.98;
    //! editado para testear
    max_x *= 1.2;
    max_y *= 1;

    srand(time(0));
    this->map = new Node[number_nodes];
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
        printf("nodo %d: x= %2.f  y=%2.f\n", i, map[i].shape.x, map[i].shape.y);
    }

    printf("number_nodos : %d\nwidth : %1.f\tmax_x : %2.f\nheigth : %1.f\tmax_y : %2.f\n", number_nodes, WINDOW_WIDTH, max_x, WINDOW_HEIGHT, max_y);
    printf("coalitions : %d\n", coalition_count);
    //! order array, to improve generation of array of frustrum
    printf("[+] Grafo creado...\n");
}

Grafo::~Grafo()
{
    if (map)
        delete[] map;
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
            (node.shape.y + py - PADDING >= cam.y) && (node.shape.y + py - PADDING <= (cam.y + cam.h)))
               ? true
               : false;
    return (SDL_PointInRectFloat(&node.shape, &cam));

    //! Add process to aplicate frustum culling technique
}

void Grafo::render(SDL_Renderer *renderer, float &pos_x, float &pos_y)
{
    int count_nodes_renderer = 0;
    SDL_FRect cam = {-PADDING - RADIUS - GAP, -PADDING - RADIUS - GAP, WINDOW_WIDTH + PADDING + RADIUS + GAP, WINDOW_HEIGHT + PADDING + RADIUS + GAP};

    SDL_SetRenderDrawColor(renderer, COLOR_BLUE, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &cam);
    SDL_SetRenderDrawColor(renderer, COLOR_RED, SDL_ALPHA_OPAQUE);

    // printf("renderizando nodos..\n");
    float radius_from = SDL_pow(RADIUS, 2);
    for (int i = 0; i < num_nodes; i++)
    {
        if (frustum(map[i], cam, pos_x, pos_y))
        {
            count_nodes_renderer++;
            //  renderizamos el circulo
            for (float x = map[i].shape.x - RADIUS; x < map[i].shape.x + RADIUS; x++)
                for (float y = map[i].shape.y - RADIUS; y < map[i].shape.y + RADIUS; y++)
                {
                    // c2=a2+b2
                    float distance = SDL_pow((x - map[i].shape.x), 2) + SDL_pow((y - map[i].shape.y), 2);
                    if (distance < radius_from)
                    {
                        SDL_FRect pixel = {x + pos_x, y + pos_y, 1, 1};
                        SDL_RenderFillRect(renderer, &pixel);
                    }
                }
        }
    }
    if (general_nodes_count != count_nodes_renderer)
    {
        printf("%d nodos renderizados\n", count_nodes_renderer);
        general_nodes_count = count_nodes_renderer;
    }

    /* SDL_FRect rect = {150, 50, 50, 50};
    // SDL_RenderRect(renderer, &rect);
    SDL_RenderFillRect(renderer, &rect); */
}
