#ifndef sdl2_testing_main_h
#define sdl2_testing_main_h

#define STATUS_LIVES 0
#define STATUS_GAME 1
#define STATUS_GAMEOVER 2
#define MAXIMUM_PROJECTILES 3
#define MAXIMUM_ASTEROIDS 3

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

typedef struct {
	float x, y;
	short lives;
	int alive, visible, dead;
} Move;

typedef struct {
	float x, y, dx, dy;
} Bullet;

typedef struct {
	int x, y;
} Stars;

typedef struct {
	float x, y, w, h, dx, dy;
	int visibleBig;
	int aliveBig;
} bigRocks;

typedef struct {
	float x, y, w, h, dx, dy;
} mediumRocks;

typedef struct {
	float x, y, w, h, dx, dy;
} smallRocks;

typedef struct {
	int time, status, shoot, point, countdown;
	float x, y, dx, pointW, pointH;

	//Sounds
	Mix_Music *bgMusic;
	Mix_Chunk *shipShoot, *roidExplode, *shipExplode;

	// Structures
	Move move;
	Stars stars[100];

	// Textures
	SDL_Texture *rock[3];
	SDL_Texture *player;
	SDL_Texture *star;
	SDL_Texture *label;
	SDL_Texture *labelOver;
	SDL_Texture *points;
	SDL_Texture *bullet;

	// Fonts
	TTF_Font *font;

	// Renderer
	SDL_Renderer *renderer;
} GameState;

#endif
