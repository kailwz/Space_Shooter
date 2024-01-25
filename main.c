#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "main.h"
#include "status.h"

Bullet *bullets[MAXIMUM_PROJECTILES] = { NULL };
bigRocks *bigrocks[MAXIMUM_ASTEROIDS] = {	NULL };
mediumRocks *mediumrocks[MAXIMUM_ASTEROIDS] = {	NULL };
smallRocks *smallrocks[MAXIMUM_ASTEROIDS] = {	NULL };

void loadGame (GameState *game) {
	
	game->time = 0;

	// Player
	game->move.x = 295;
	game->move.y = 215;
	game->move.alive = 1;
	game->move.visible = 1;

	// Game
	game->label = NULL;
	game->status = STATUS_LIVES;
	game->move.lives = 3;
	
	SDL_Surface *surface = NULL;

	// Player image	
	surface = IMG_Load("images/spaceship.png");

	if (surface == NULL) {
		printf("Could not find spaceship.png\n\n");
		SDL_Quit();
		exit(1);
	}
	
	game->player = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	// Star image
	surface = IMG_Load("images/star.png");

	if (surface == NULL) {
		printf("Could not find star.png\n\n");
		SDL_Quit();
		exit(1);
	}
	
	game->star = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	// Load Stars on the screen
	for (int i = 0; i < 100; i++) {
		game->stars[i].x = random()%640;
		game->stars[i].y = random()%480;
	}
	
	// Big Asteroid image
	surface = IMG_Load("images/big_rock.png");

	if (surface == NULL) {
		printf("Could not find big_rock.png\n\n");
		SDL_Quit();
		exit(1);
	}
	
	game->rock[0] = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	// Medium Asteroid image
	surface = IMG_Load("images/medium_rock.png");

	if (surface == NULL) {
		printf("Could not find medium_rock.png\n\n");
		SDL_Quit();
		exit(1);
	}
	
	game->rock[1] = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	// Small Asteroid image
	surface = IMG_Load("images/small_rock.png");

	if (surface == NULL) {
		printf("Could not find small_rock.png\n\n");
		SDL_Quit();
		exit(1);
	}
	
	game->rock[2] = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

	// Screen text
	game->font = TTF_OpenFont("fonts/crazy-pixel.ttf", 56);
	if (!game->font) {
		printf("Could not find crazy-pixel.ttf\n\n");
		SDL_Quit();
		exit(1);
	}

	// Give the priopreties to the screen text
	init_status_lives(game);

	//Bullet image
	surface = IMG_Load("images/star.png");

	if (surface == NULL) {
		printf("Could not find bullet.png\n\n");
		SDL_Quit();
		exit(1);
	}
	
	game->bullet = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_FreeSurface(surface);

}

void spawnBullet (float x, float y, float dx, float dy) {
	int found = -1;
	for (int i = 0; i < MAXIMUM_PROJECTILES; i++) {
		if (bullets[i] == NULL) {
			found = i;
			break;
		}
	}

	if (found >= 0) {
		int i = found;
		bullets[i] = malloc(sizeof(Bullet));
		bullets[i]->x = x;
		bullets[i]->y = y;
		bullets[i]->dx = dx;
		bullets[i]->dy = dy;
	}
}

void deleteBullet (int i) {
	if (bullets[i]) {
		free(bullets[i]);
		bullets[i] = NULL;
	}
}

void spawnBigAsteroids (float x, float y, float dx, float dy) {
	int found = -1;
	for (int i = 0; i < MAXIMUM_ASTEROIDS; i++) {
		if (bigrocks[i] == NULL) {
			found = i;
			break;
		}
	}

	if (found >= 0) {
		int i = found;
		bigrocks[i] = malloc(sizeof(bigRocks));
		bigrocks[i]->x = x;
		bigrocks[i]->y = y;
		bigrocks[i]->dx = dx;
		bigrocks[i]->dy = dy;
	}
}

void deleteBigAsteroids (int i) {
	if (bigrocks[i]) {
		free(bigrocks[i]);
		bigrocks[i] = NULL;
	}
}

void spawnMediumAsteroids (float x, float y, float dx, float dy) {
	int found = -1;
	for (int i = 0; i < MAXIMUM_ASTEROIDS; i++) {
		if (mediumrocks[i] == NULL) {
			found = i;
			break;
		}
	}

	if (found >= 0) {
		int i = found;
		mediumrocks[i] = malloc(sizeof(bigRocks));
		mediumrocks[i]->x = x;
		mediumrocks[i]->y = y;
		mediumrocks[i]->dx = dx;
		mediumrocks[i]->dy = dy;
	}
}

void deleteMediumAsteroids (int i) {
	if (mediumrocks[i]) {
		free(mediumrocks[i]);
		mediumrocks[i] = NULL;
	}
}

void spawnSmallAsteroids (float x, float y, float dx, float dy) {
	int found = -1;
	for (int i = 0; i < MAXIMUM_ASTEROIDS; i++) {
		if (smallrocks[i] == NULL) {
			found = i;
			break;
		}
	}

	if (found >= 0) {
		int i = found;
		smallrocks[i] = malloc(sizeof(smallRocks));
		smallrocks[i]->x = x;
		smallrocks[i]->y = y;
		smallrocks[i]->dx = dx;
		smallrocks[i]->dy = dy;
	}
}

void deleteSmallAsteroids (int i) {
	if (smallrocks[i]) {
		free(smallrocks[i]);
		smallrocks[i] = NULL;
	}
}

void process (GameState *game) {
	game->time++;
		
	// Time that the text will apear in the screen
	if (game->time > 250) {
		shutdown_status_lives(game);
		game->status = STATUS_GAME;
	}

	// Player Movements
	if (game->status == STATUS_GAME && game->move.alive) {
		const Uint8 *state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_A]) {
			game->move.x -= 5;
		}
		if (state[SDL_SCANCODE_D]) {
			game->move.x += 5;
		}
		if (state[SDL_SCANCODE_W]) {
			game->move.y -= 5;
		}
		if (state[SDL_SCANCODE_S]) {
			game->move.y += 5;
		}
		
		// Shoot bullets
		if (state[SDL_SCANCODE_SPACE]) {
			if (game->time % 6 == 0) {
				spawnBullet(game->move.x+10, game->move.y, 0, -5);
			}
		}
	}

	// Maximum bullets on the screen
	for (int i = 0; i < MAXIMUM_PROJECTILES; i++) if (bullets[i]) {
		bullets[i]->x += bullets[i]->dx;
		bullets[i]->y += bullets[i]->dy;

		// Delete bullets
		if (bullets[i]->y < 0 || bullets[i]->y > 480) {
			deleteBullet(i);
		}
	}

	// Move the big asteroids
	if (game->time % 40 == 0) {
		spawnBigAsteroids(random()%640-45, -50, 0, 2);
	}
	
	for (int i = 0; i < MAXIMUM_ASTEROIDS; i++) if (bigrocks[i]) {
		bigrocks[i]->x += bigrocks[i]->dx;
		bigrocks[i]->y += bigrocks[i]->dy;

		// Delete big asteroids
		if (bigrocks[i]->y < -50 || bigrocks[i]->y > 480) {
			deleteBigAsteroids(i);
		}
	}

	// Move the medium asteroids
	if (game->time % 40 == 0) {
		spawnMediumAsteroids(random()%640-35, -50, 0, 3);
	}
	
	for (int i = 0; i < MAXIMUM_ASTEROIDS; i++) if (mediumrocks[i]) {
		mediumrocks[i]->x += mediumrocks[i]->dx;
		mediumrocks[i]->y += mediumrocks[i]->dy;

		// Delete medium asteroids
		if (mediumrocks[i]->y < -50 || mediumrocks[i]->y > 480) {
			deleteMediumAsteroids(i);
		}
	}

	// Move the small asteroids
	if (game->time % 40 == 0) {
		spawnSmallAsteroids(random()%640-25, -50, 0, 4);
	}
	
	for (int i = 0; i < MAXIMUM_ASTEROIDS; i++) if (smallrocks[i]) {
		smallrocks[i]->x += smallrocks[i]->dx;
		smallrocks[i]->y += smallrocks[i]->dy;

		// Delete small asteroids
		if (smallrocks[i]->y < -50 || smallrocks[i]->y > 480) {
			deleteSmallAsteroids(i);
		}
	}

}

void detectColision (GameState *game) {
	float space = 0, ww = 640 - 24, wh = 480 - 24;
	float /*pw = 24, ph = 24,*/ px = game->move.x, py = game->move.y;
	/*float rx = rocks[0]->x, ry = rocks[0]->y, rw = rocks[0]->w, rh = rocks[0]->h;*/

	// Window collision
	if (px <= space) {
		game->move.x = space;
	}
	else if (py <= space){
		game->move.y = space;
	}
	if (px >= ww) {
		game->move.x = ww;
	}
	else if (py >= wh){
		game->move.y = wh;
	}
	if (px <= space && py <= space) {
		game->move.x = space;
		game->move.y = space;
	}
	else if (px >= ww &&py >= wh){
		game->move.x = ww;
		game->move.y = wh;
	}

	for (int i = 0; i < MAXIMUM_ASTEROIDS; i++) if (bigrocks[i]) if(bullets[i]) {
		if (bigrocks[i]->y + 45 > bullets[i]->y && bullets[i]->y + 8 > bigrocks[i]->y) {
			if (bigrocks[i]->x + 45 > bullets[i]->x && bullets[i]->x + 6 > bigrocks[i]->x) {
				deleteBigAsteroids(i);
				deleteBullet(i);
			}
		}
	}

	for (int i = 0; i < MAXIMUM_ASTEROIDS; i++) if (mediumrocks[i]) if(bullets[i]) {
		if (mediumrocks[i]->y + 45 > bullets[i]->y && bullets[i]->y + 8 > mediumrocks[i]->y) {
			if (mediumrocks[i]->x + 45 > bullets[i]->x && bullets[i]->x + 6 > mediumrocks[i]->x) {
				deleteMediumAsteroids(i);
				deleteBullet(i);
			}
		}
	}

	for (int i = 0; i < MAXIMUM_ASTEROIDS; i++) if (smallrocks[i]) if(bullets[i]) {
		if (smallrocks[i]->y + 45 > bullets[i]->y && bullets[i]->y + 8 > smallrocks[i]->y) {
			if (smallrocks[i]->x + 45 > bullets[i]->x && bullets[i]->x + 6 > smallrocks[i]->x) {
				deleteSmallAsteroids(i);
				deleteBullet(i);
			}
		}
	}
}

int eventProcessing (SDL_Window *window, GameState *game) {
	SDL_Event event;
	int run = 1;

	// Main Window event
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_WINDOWEVENT_CLOSE: 
			{
				if (window) {
					SDL_DestroyWindow(window);
					window = NULL;
					run = 0;
				}
			}
			break;

			case SDL_QUIT:
			{
				run = 0;
			}
			break;
		}
	}

	return run;
}

void loadRender (SDL_Renderer *renderer, GameState *game) {
		
	// Draw the lives on the screen
	if (game->status == STATUS_LIVES) {
		draw_status_lives(game);
	}

	// Start the game after the lives screen
	else if (game->status == STATUS_GAME) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		SDL_RenderClear(renderer);
		
		// Player
		if (game->move.visible) {
			SDL_Rect playerRect = {game->move.x, game->move.y, 24, 24};	
			SDL_RenderCopyEx(renderer, game->player, NULL, &playerRect, 0, NULL, 0);
		}

		// Stars
		for (int i = 0; i < 100; i++) {
			SDL_Rect starRect = {game->stars[i].x, game->stars[i].y, 1, 1};
			SDL_RenderCopy(renderer, game->star, NULL, &starRect);
		}
		
		// Big Asteroid
		for (int i = 0; i < MAXIMUM_ASTEROIDS; i++) if (bigrocks[i]) {
			SDL_Rect bigRockRect = {bigrocks[i]->x, bigrocks[i]->y, 45, 45};
			SDL_RenderCopy(renderer, game->rock[0], NULL, &bigRockRect);
		}

		// Medium Asteroid
		for (int i = 0; i < MAXIMUM_ASTEROIDS; i++) if (mediumrocks[i]) {
			SDL_Rect mediumRockRect = {mediumrocks[i]->x, mediumrocks[i]->y, 45, 45};
			SDL_RenderCopy(renderer, game->rock[1], NULL, &mediumRockRect);
		}

		// Small Asteroid
		for (int i = 0; i < MAXIMUM_ASTEROIDS; i++) if (smallrocks[i]) {
			SDL_Rect smallRockRect = {smallrocks[i]->x, smallrocks[i]->y, 45, 45};
			SDL_RenderCopy(renderer, game->rock[2], NULL, &smallRockRect);
		}

		// Bullets
		for (int i = 0; i < MAXIMUM_PROJECTILES; i++) if (bullets[i]) {
			SDL_Rect bulletRect = {bullets[i]->x, bullets[i]->y, 6, 8};
			SDL_RenderCopy(renderer, game->bullet, NULL, &bulletRect);
		}

	}

	SDL_RenderPresent(renderer);
}

int main (int argc, char *argv[]) {

	GameState gameState;

	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;

	SDL_Init(SDL_INIT_VIDEO);

	TTF_Init();
	
	srandom((int)time(NULL));

	window = SDL_CreateWindow(
			"Space Shooter",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			640,
			480,
			0
	);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	gameState.renderer = renderer;
	loadGame(&gameState);
	
	int run = 1;
	
	// Run the game
	while (run) {
		if (eventProcessing(window, &gameState) == 0)
			run = 0;

		process(&gameState);
		
		detectColision(&gameState);

		loadRender(renderer, &gameState);

		// SDL_Delay(10);
	}

	// Clean Textures
	SDL_DestroyTexture(gameState.player);
	SDL_DestroyTexture(gameState.rock[0]);
	SDL_DestroyTexture(gameState.rock[1]);
	SDL_DestroyTexture(gameState.rock[2]);
	SDL_DestroyTexture(gameState.star);
	SDL_DestroyTexture(gameState.bullet);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	TTF_CloseFont(gameState.font);
	
	if (gameState.label != NULL) {
		SDL_DestroyTexture(gameState.label);
	}

	for (int i = 0; i < MAXIMUM_PROJECTILES; i++) {
		deleteBullet(i);
	}

	TTF_Quit();

	SDL_Quit();

	return 0;
}

// How to Program a Game in C Tutorial
// https://www.youtube.com/watch?v=FCRmIoX6PTA&list=PLT6WFYYZE6uLMcPGS3qfpYm7T_gViYMMt&index=21
// https://www.youtube.com/shorts/MZaShGcVBLw
// https://www.youtube.com/watch?v=OJrX3aNPsHM
// https://stackoverflow.com/questions/69366699/why-does-the-compiler-is-giving-me-this-undefined-reference-to-function
// https://www.youtube.com/watch?v=5ccarSklfVM

// to compile gcc -Wall -Werror status.c main.c `sdl2-config --cflags --libs` -o

/*
// Usefull Player vs enemy collision

if (game->status == STATUS_GAME && game->rocks[0].aliveBig) {

	for (int i = 0; i < MAXIMUM_ASTEROIDS; i++) {
		if (game->move.y + 24 > bigrocks[i]->y && game->move.y < bigrocks[i]->y + bigrocks[i]->h) {
			if (game->move.y > bigrocks[i]->y + bigrocks[i]->h && game->move.y > bigrocks[i]->y) {
				deleteBigAsteroids(i);
				deleteBullet(i);
				printf("boommm!\n\n");
			}
			
			else if (game->move.x + 24 > bigrocks[i]->x && game->move.x < bigrocks[i]->x) {
				deleteBigAsteroids(i);
				deleteBullet(i);
				printf("boommm!\n\n");
			}
		}

		if (game->move.x + 24 > bigrocks[i]->x && game->move.x < bigrocks[i]->x + bigrocks[i]->w) {
			if (game->move.y > bigrocks[i]->y + bigrocks[i]->h && game->move.y > bigrocks[i]->y) {
				deleteBigAsteroids(i);
				deleteBullet(i);
				printf("boommm!\n\n");
			}

			else if (game->move.y + 24 > bigrocks[i]->y && game->move.y < bigrocks[i]->y) {
				deleteBigAsteroids(i);
				deleteBullet(i);
				printf("boommm!\n\n");
			}
		}
	}
}


// Collision test
	for (int i = 0; i < MAXIMUM_ASTEROIDS; i++) if (bigrocks[i]) if (bullets[i]) {
			if (bigrocks[i]->y + bigrocks[i]->h >= bullets[i]->y) {
				deleteBigAsteroids(i);
				deleteBullet(i);
			}
	}

*/
