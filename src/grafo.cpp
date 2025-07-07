// #include "general.h"
#include "grafo.h"
#include <algorithm> // std::sort
#include <time.h>
#include <random>

int general_nodes_count = 0;

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

int find(float val, Node *mapy, int tam)
{
    int left = 0;
    int right = tam - 1;
    int best = -1;
    float best_diff = std::numeric_limits<float>::max();

    while (left <= right)
    {
        int mid = (left + right) / 2;
        float key = mapy[mid].shape.x + mapy[mid].shape.y;
        float diff = std::abs(key - val);

        if (diff < best_diff)
        {
            best_diff = diff;
            best = mid;
        }

        if (key < val)
            left = mid + 1;
        else
            right = mid - 1;
    }

    return best;
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

void Grafo::create_children()
{
    const int VENTANA = 15;

    for (int i = 0; i < num_nodes; i++)
    {
        std::vector<std::pair<float, int>> distancias;
        std::vector<int> insertados;

        // Buscar en vecindad de i
        for (int j = std::max(0, i - VENTANA); j <= std::min(num_nodes - 1, i + VENTANA); j++)
        {
            if (i == j)
                continue;

            float dx = map[i].shape.x - map[j].shape.x;
            float dy = map[i].shape.y - map[j].shape.y;
            float dist_squared = SDL_powf(dx, 2) + SDL_powf(dy, 2);

            distancias.emplace_back(dist_squared, j);
            insertados.push_back(j);
        }

        // Buscar en vecindad desde mapy (ordenado por y)
        int new_i = find(map[i].shape.y, mapy, num_nodes);
        if (new_i != -1)
        {
            for (int j = std::max(0, new_i - VENTANA); j <= std::min(num_nodes - 1, new_i + VENTANA); j++)
            {
                if (i == j || esta(j, insertados))
                    continue;

                float dx = map[i].shape.x - mapy[j].shape.x;
                float dy = map[i].shape.y - mapy[j].shape.y;
                float dist_squared = SDL_powf(dx, 2) + SDL_powf(dy, 2);

                distancias.emplace_back(dist_squared, j);
                insertados.push_back(j);
            }
        }

        // Ordenar por distancia
        std::sort(distancias.begin(), distancias.end());

        // Agregar los 3 vecinos más cercanos
        for (int k = 0; k < 3 && k < distancias.size(); k++)
        {
            int idx = distancias[k].second;

            map[i].childrens.push_back(&map[idx]);
            map[idx].childrens.push_back(&map[i]);
        }
    }
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

    // SDL_SetRenderDrawColor(renderer, COLOR_BLUE, SDL_ALPHA_OPAQUE);
    SDL_RenderFillRect(renderer, &cam);
    // SDL_SetRenderDrawColor(renderer, COLOR_GREY, SDL_ALPHA_OPAQUE);

    // printf("renderizando nodos..\n");
    float radius_from = SDL_pow(RADIUS, 2);
    for (int i = 0; i < num_nodes; i++)
    {
        if (frustum(map[i], cam, pos_x, pos_y))
        {
            count_nodes_renderer++;
            //*  renderizamos el circulo
            for (int j = 0; j < map[i].childrens.size(); j++)
            {
                renderAristas(renderer, &map[i], map[i].childrens[j], pos_x, pos_y);
                /* if (frustum(map[i].childrens[j], cam, pos_x, pos_y))
                {
                    } */
            }

            SDL_SetRenderDrawColor(renderer, COLOR_GREY, SDL_ALPHA_OPAQUE);

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
