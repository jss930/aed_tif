#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include "general.h"
#include "grafo.h"
#include <SDL3/SDL_main.h>
#include <iostream>

Grafo *grafo;
float pos_x = RADIUS;
float pos_y = RADIUS;
#define VELOCIDAD 1
#define TOTAL_NODES 1000

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{

  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    SDL_Log("[-] No se puede inicializar video: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_CreateWindowAndRenderer("Google Maps Chafa", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer))
  {
    SDL_Log("[-] No se puede crear window/renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  grafo = new Grafo(TOTAL_NODES);

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
  if (event->type == SDL_EVENT_QUIT)
  {
    return SDL_APP_SUCCESS;
  }
  if (event->type == SDL_EVENT_KEY_DOWN)
    if (event->key.key == 27)
      return SDL_APP_SUCCESS;

  //! movimiento
  if (event->type == SDL_EVENT_MOUSE_MOTION)
  {
    //? click derecho
    if (event->motion.state == 4)
    {
      printf("RIGHT clic\n");
      //  printf("movimiento en %2.f - %2.f\n", event->motion.x, event->motion.y);
    }
    else if (event->motion.state == SDL_BUTTON_LEFT)
    {

      float tmp = pos_x + VELOCIDAD * event->motion.xrel;
      if (tmp >= -grafo->max_x + WINDOW_WIDTH - RADIUS - PADDING - GAP && tmp <= RADIUS + GAP)
      {
        pos_x = tmp;
      }
      tmp = pos_y + VELOCIDAD * event->motion.yrel;
      if (tmp >= -grafo->max_y + WINDOW_HEIGHT - RADIUS - PADDING - GAP && tmp <= RADIUS + GAP)
      {
        pos_y = tmp;
      }
      // printf("actualizar: %2.f - %2.f\n", pos_x, pos_y);
    }
  }
  //! levanta click
  if (event->type == SDL_EVENT_MOUSE_BUTTON_UP)
  {
    printf("levantado ");
    if (event->button.button == SDL_BUTTON_LEFT)
    {
      printf("LEFT clic\n");
    }
    else if (event->button.button == SDL_BUTTON_RIGHT)
    {
      printf("RIGHT clic\n");
    }
  }
  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
  SDL_SetRenderDrawColor(renderer, COLOR_BROWN, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
  grafo->render(renderer, pos_x, pos_y);
  SDL_RenderPresent(renderer);
  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
  if (grafo)
    delete grafo;
  printf("Eso fue todo pa D:\n");
}
