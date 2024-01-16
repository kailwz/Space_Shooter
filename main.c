#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "main.h"
#include "status.h"

Bullet *bullets[MAXIMUM_PROJECTILES] = { NULL };

void loadGame (GameState *game) {
	
	game->time = 0;

	// Player
	game->move.x = 295;
	game->move.y = 215;
	game->move.alive = 1;
	game->move.visible = 1;

	// Asteroids
	game->rocks[0].aliveBig = 1;
	game->rocks[1].aliveMedium = 1;
	game->rocks[2].aliveSmall = 1;

	game->rocks[0].visibleBig = 1;
	game->rocks[1].visibleMedium = 1;
	game->rocks[2].visibleSmall = 1;

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

	// big Asteroid info
	game->rocks[0].w = 45;
	game->rocks[0].h = 45;
	game->rocks[0].x = 100;
	game->rocks[0].y = 100;

	// Medium Asteroid info
	game->rocks[1].w = 35;
	game->rocks[1].h = 35;
	game->rocks[1].x = 100;
	game->rocks[1].y = 200;

	// Small Asteroid info
	game->rocks[2].w = 25;
	game->rocks[2].h = 25;
	game->rocks[2].x = 100;
	game->rocks[2].y = 280;

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

void process (GameState *game) {
	game->time++;
	
	// Time that the text will apear in the screen
	if (game->time > 250) {
		shutdown_status_lives(game);
		game->status = STATUS_GAME;
	}

	if (game->status == STATUS_GAME) {	
		// Player Movements
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
			if (game->time % 12 == 0) {
				spawnBullet(game->move.x+10, game->move.y, 0, -5);
			}
		}
	}

	// Maximum bullets on the screen
	for (int i = 0; i < MAXIMUM_PROJECTILES; i++) if (bullets[i]) {
		bullets[i]->x += bullets[i]->dx;
		bullets[i]->y += bullets[i]->dy;
		
		// Big bullet colision
		if (bullets[i]->x > game->rocks[0].x && bullets[i]->x < game->rocks[0].x+45 && 
		bullets[i]->y > game->rocks[0].y && bullets[i]->y < game->rocks[0].y+45) {
			game->rocks[0].aliveBig = 0;
		}
		
		// Medium bullet colision
		if (bullets[i]->x > game->rocks[1].x && bullets[i]->x < game->rocks[1].x+35 && 
		bullets[i]->y > game->rocks[1].y && bullets[i]->y < game->rocks[1].y+35) {
			game->rocks[1].aliveMedium = 0;
		}

		// Small bullet colision
		if (bullets[i]->x > game->rocks[2].x && bullets[i]->x < game->rocks[2].x+25 && 
		bullets[i]->y > game->rocks[2].y && bullets[i]->y < game->rocks[2].y+25) {
			game->rocks[2].aliveSmall = 0;
		}

		// Delete bullets
		if (bullets[i]->y < -1000 || bullets[i]->y > 1000) {
			deleteBullet(i);
		}

		// Destroyed big asteroids treatment
		if (game->rocks[0].aliveBig == 0) {
			game->rocks[0].visibleBig = 0;
			game->rocks[0].aliveBig = 0;
		}
		
		// Destroyed medium asteroids treatment
		if (game->rocks[1].aliveMedium == 0) {
			game->rocks[1].visibleMedium = 0;
			game->rocks[1].aliveMedium = 0;
		}
		
		// Destroyed small asteroids treatment
		if (game->rocks[2].aliveSmall == 0) {
			game->rocks[2].visibleSmall = 0;
			game->rocks[2].aliveSmall = 0;
		}
	}

}

void detectColision (GameState *game) {
	float space = 0, ww = 640 - 24, wh = 480 - 24;
	float pw = 24, ph = 24, px = game->move.x, py = game->move.y;
	float rx = game->rocks[0].x, ry = game->rocks[0].y, rw = game->rocks[0].w, rh = game->rocks[0].h;

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

	// Big Asteroid collision 
	if (py+ph > ry && py < ry+rh) {
		if (px < rx+rw && px+pw > rx+rw) {
			game->move.x = rx+rh;
			px = rx+rw;
			game->move.visible = 0;
			game->move.alive = 0;
			SDL_DestroyTexture(game->player);
		}
		else if (px+pw > rx && px < rx) {
			game->move.x = rx-pw;
			px = rx-pw;
			game->move.visible = 0;
			game->move.alive = 0;
			SDL_DestroyTexture(game->player);
		}
	}
	if (px+pw > rx && px < rx+rw) {
		if (py < ry+rh && py > ry) {
			game->move.y = ry+rw;
			py = ry+rh;
			game->move.visible = 0;
			game->move.alive = 0;
			SDL_DestroyTexture(game->player);
		}
		else if (py+ph > ry && py < ry) {
			game->move.y = ry-ph;
			py = ry-ph;
			game->move.visible = 0;
			game->move.alive = 0;
			SDL_DestroyTexture(game->player);
		}
	}

	/*/ Medium Asteroid collision 
	if (py+ph > ry && py < ry+rh) {
		if (px < rx+rw && px+pw > rx+rw) {
			game->move.x = rx+rh;
			px = rx+rw;
			game->move.visible = 0;
			game->move.alive = 0;
			SDL_DestroyTexture(game->player);
		}
		else if (px+pw > rx && px < rx) {
			game->move.x = rx-pw;
			px = rx-pw;
			game->move.visible = 0;
			game->move.alive = 0;
			SDL_DestroyTexture(game->player);
		}
	}
	if (px+pw > rx && px < rx+rw) {
		if (py < ry+rh && py > ry) {
			game->move.y = ry+rw;
			py = ry+rh;
			game->move.visible = 0;
			game->move.alive = 0;
			SDL_DestroyTexture(game->player);
		}
		else if (py+ph > ry && py < ry) {
			game->move.y = ry-ph;
			py = ry-ph;
			game->move.visible = 0;
			game->move.alive = 0;
			SDL_DestroyTexture(game->player);
		}
	}*/
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

		if (game->move.visible) {
			// Player
			SDL_Rect playerRect = {game->move.x, game->move.y, 24, 24};	
			SDL_RenderCopyEx(renderer, game->player, NULL, &playerRect, 0, NULL, 0);
		}

			// Stars
		for (int i = 0; i < 100; i++) {
			SDL_Rect starRect = {game->stars[i].x, game->stars[i].y, 1, 1};
			SDL_RenderCopy(renderer, game->star, NULL, &starRect);
		}
		
		if (game->rocks[0].visibleBig) {
			// Big Asteroid
			SDL_Rect bigrockRect = {game->rocks[0].x, game->rocks[0].y, 45, 45};
			SDL_RenderCopyEx(renderer, game->rock[0], NULL, &bigrockRect, 0, NULL, 0);
		}

		if (game->rocks[1].visibleMedium) {
			// Medium Asteroid
			SDL_Rect mediumrockRect = {game->rocks[1].x, game->rocks[1].y, 35, 35};
			SDL_RenderCopyEx(renderer, game->rock[1], NULL, &mediumrockRect, 0, NULL, 0);
		}

		if (game->rocks[2].visibleSmall) {
			// Small Asteroid
			SDL_Rect smallrockRect = {game->rocks[2].x ,game->rocks[2].y, 25, 25};
			SDL_RenderCopyEx(renderer, game->rock[2], NULL, &smallrockRect, 0, NULL, 0);
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
// to compile gcc -Wall -Werror status.c main.c `sdl2-config --cflags --libs` -o
