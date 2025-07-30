#include "general.h"
#include "grafo.h"
#include <SDL3/SDL.h>
#include <iostream>

#define TOTAL_NODES 20
#define VELOCIDAD 1

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

float pos_x = RADIUS;
float pos_y = RADIUS;

int main(int argc, char *argv[])
{
    // Evitar warning de parámetros no usados
    (void)argc;
    (void)argv;
    
    // Corregido: SDL_Init retorna bool en SDL3, no int
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("[-] No se puede inicializar SDL: %s", SDL_GetError());
        return 1;
    }
    
    if (!SDL_CreateWindowAndRenderer("Google Maps Chafa", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer))
    {
        SDL_Log("[-] No se puede crear ventana/renderizador: %s", SDL_GetError());
        return 1;
    }
    
    Grafo grafo(TOTAL_NODES);
    bool running = true;
    SDL_Event event;
    
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
                
            // Corregido: En SDL3 la estructura de teclado cambió
            if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE)
                running = false;
                
            if (event.type == SDL_EVENT_MOUSE_MOTION)
            {
                if (event.motion.state == SDL_BUTTON_LMASK)
                {
                    float tmp_x = pos_x + VELOCIDAD * event.motion.yrel;
                    float tmp_y = pos_y + VELOCIDAD * event.motion.yrel;
                    
                    if (tmp_x >= -grafo.max_x + WINDOW_WIDTH - RADIUS - PADDING - GAP && tmp_x <= RADIUS + GAP)
                        pos_x = tmp_x;
                        
                    if (tmp_y >= -grafo.max_y + WINDOW_HEIGHT - RADIUS - PADDING - GAP && tmp_y <= RADIUS + GAP)
                        pos_y = tmp_y;
                }
            }
        }
        
        SDL_SetRenderDrawColor(renderer, COLOR_BROWN, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        grafo.render(renderer, pos_x, pos_y);
        SDL_RenderPresent(renderer);
    }
    
    SDL_Quit();
    return 0;
}