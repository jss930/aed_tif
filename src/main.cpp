#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include "general.h"
#include "grafo.h"
#include <SDL3/SDL_main.h>
#include <iostream>
/* extern "C"
{
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
} */

/* static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static Uint64 last_time = 0;

#define WINDOW_WIDTH 640 * 1.7
#define WINDOW_HEIGHT 480 * 1.7

#define NUM_POINTS 100
#define MIN_PIXELS_PER_SECOND 30
#define MAX_PIXELS_PER_SECOND 60

static SDL_FPoint nodes[NUM_POINTS];
static float point_speeds[NUM_POINTS]; */

/* This function runs once at startup. */
Grafo *grafo;
float pos_x = RADIUS;
float pos_y = RADIUS;
#define VELOCIDAD 1
#define TOTAL_NODES 15

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
  int i;

  // SDL_SetAppMetadata("Example Renderer Points", "1.0", "com.example.renderer-points");

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
  /* for (i = 0; i < SDL_arraysize(nodes); i++)
  {
    nodes[i].x = SDL_randf() * ((float)WINDOW_WIDTH);
    nodes[i].y = SDL_randf() * ((float)WINDOW_HEIGHT);
    point_speeds[i] = MIN_PIXELS_PER_SECOND + (SDL_randf() * (MAX_PIXELS_PER_SECOND - MIN_PIXELS_PER_SECOND));
  }

  last_time = SDL_GetTicks();
 */
  return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
  //  SDL_PeepEvents(event, 2, SDL_EVENT_MOUSE_MOTION, );
  if (event->type == SDL_EVENT_QUIT)
  {
    return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */
  }
  if (event->type == SDL_EVENT_KEY_DOWN)
    if (event->key.key == 27)
      return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */

  //! movimiento
  if (event->type == SDL_EVENT_MOUSE_MOTION)
  {
    //  if(event->wheel)
    //? click derecho
    if (event->motion.state == 4)
    {
      // printf("RIGHT clic\n");
      //  printf("movimiento en %2.f - %2.f\n", event->motion.x, event->motion.y);
    }
    else if (event->motion.state == SDL_BUTTON_LEFT)
    {
      /* printf("x : %2.f\n", event->motion.x);
      printf("xrel : %2.f\n", event->motion.xrel);
      printf("y : %2.f\n", event->motion.y);
      printf("yrel : %2.f\n", event->motion.yrel);
      */
      // printf("movimiento en %2.f - %2.f\n", event->motion.x, event->motion.y);
      // printf("antes: %2.f - %2.f\n", pos_x, pos_y);
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
      printf("actualizar: %2.f - %2.f\n", pos_x, pos_y);
      //  tmp_pos_x = event->motion.x;
      //  tmp_pos_y = event->motion.y;
      // printf("LEFT clic\n");
    }
  }
  //! levanta click
  /*   if (event->type == SDL_EVENT_MOUSE_BUTTON_UP)
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
   */
  return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
  // const Uint64 now = SDL_GetTicks();

  /* SDL_MouseButtonEvent * mouse= new SDL-mos ;
  mouse.button = 2;
  mouse.which = 0; */

  SDL_SetRenderDrawColor(renderer, COLOR_BROWN, SDL_ALPHA_OPAQUE); /* black, full alpha */
  SDL_RenderClear(renderer);                                       /* start with a blank canvas. */
  grafo->render(renderer, pos_x, pos_y);
  SDL_RenderPresent(renderer); /* put it all on the screen! */
  return SDL_APP_CONTINUE;     /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
  if (grafo)
    delete grafo;
  printf("Eso fue todo pa D:\n");
  /* SDL will clean up the window/renderer for us. */
}
