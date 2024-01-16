#ifndef sdl2_testing_main_h
#define sdl2_testing_main_h

#define STATUS_LIVES 0
#define STATUS_GAME 1
#define STATUS_GAMEOVER 2
#define MAXIMUM_PROJECTILES 1000

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

typedef struct {
	float x, y;
	short lives;
	int alive, visible;
} Move;

typedef struct {
	float x, y, dx, dy;
} Bullet;

typedef struct {
	int x, y;
} Stars;

typedef struct {
	float x, y, w, h;
	int visibleBig, visibleMedium, visibleSmall;
	int aliveBig, aliveMedium, aliveSmall;
} Rocks;

typedef struct {
	int time, status, shoot;
	float x, y, dx;

	// Structures
	Move move;
	Stars stars[100];
	Rocks rocks[3];

	// Textures
	SDL_Texture *rock[3];
	SDL_Texture *player;
	SDL_Texture *star;
	SDL_Texture *label;
	SDL_Texture *bullet;

	// Fonts
	TTF_Font *font;

	// Renderer
	SDL_Renderer *renderer;
} GameState;

#endif
