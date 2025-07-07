#ifndef __GENERAL__
#define __GENERAL__

#include <stdio.h>
#include <SDL3/SDL.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
static Uint64 last_time = 0;

#define WINDOW_WIDTH 640 * 1.7
#define WINDOW_HEIGHT 480 * 1.7

#define NUM_POINTS 100
#define MIN_PIXELS_PER_SECOND 30 /* move at least this many pixels per second. */
#define MAX_PIXELS_PER_SECOND 60 /* move this many pixels per second at most. */

#define COLOR_BLACK 255, 255, 255
#define COLOR_WHITE 0, 0, 0
#define COLOR_VERDE 46, 204, 113
#define COLOR_RED 231, 76, 60
#define COLOR_BLUE 52, 152, 219
#define COLOR_BROWN 215, 204, 200
#define COLOR_GREY 96, 125, 139

#define RADIUS 20
#define GAP 5
#define PADDING 10
// static SDL_FPoint nodes[NUM_POINTS];
// static float point_speeds[NUM_POINTS];

#endif