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
ufoShip *ufoship[MAXIMUM_UFOS] = { NULL };

void loadGame (GameState *game) {
	// time
	game->time = 0;

	// Player
	game->move.x = 295;
	game->move.y = 215;
	game->move.dead = 0;
	game->move.alive = 1;
	game->move.visible = 1;

	// Game
	game->point = 0;
	game->label = NULL;
	game->labelOver = NULL;
	game->status = STATUS_LIVES;
	game->move.lives = 3;
	game->pointW = 105;
	game->pointH = 55;
	game->countdown = -1;
	game->cooldown = 0;
	game->soundcooldown = 0;
	
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

	// Ufo image
	surface = IMG_Load("images/ufo.png");

	if (surface == NULL) {
		printf("Could not find ufo.png\n\n");
		SDL_Quit();
		exit(1);
	}
	
	game->ufo = SDL_CreateTextureFromSurface(game->renderer, surface);
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
	surface = IMG_Load("images/bullet.png");

	if (surface == NULL) {
		printf("Could not find bullet.png\n\n");
		SDL_Quit();
		exit(1);
	}

	// Load bg sound
	game->bgMusic = Mix_LoadMUS("sounds/music.mp3");
	if (game->bgMusic != NULL) {
		Mix_VolumeMusic(40);
	}

	// Load shoot sound
	game->shipShoot = Mix_LoadWAV("sounds/shoot.wav");
	if (game->shipShoot != NULL) {
		Mix_VolumeChunk(game->shipShoot, 20);
	}

	// Load explode sound
	game->roidExplode = Mix_LoadWAV("sounds/asteroid_explosion.wav");
	if (game->roidExplode != NULL) {
		Mix_VolumeChunk(game->roidExplode, 20);
	}

	// Load ship explode sound
	game->shipExplode = Mix_LoadWAV("sounds/spaceship_explosion.wav");
	if (game->shipExplode != NULL) {
		Mix_VolumeChunk(game->shipExplode, 20);
	}

	// Load bullet
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

void spawnUfos (float x, float y, float dx, float dy) {
	int found = -1;
	for (int i = 0; i < MAXIMUM_UFOS; i++) {
		if (ufoship[i] == NULL) {
			found = i;
			break;
		}
	}

	if (found >= 0) {
		int i = found;
		ufoship[i] = malloc(sizeof(ufoShip));
		ufoship[i]->x = x;
		ufoship[i]->y = y;
		ufoship[i]->dx = dx;
		ufoship[i]->dy = dy;
	}
}

void deleteUfos (int i) {
	if (ufoship[i]) {
		free(ufoship[i]);
		ufoship[i] = NULL;
	}
}

void process (GameState *game) {
	game->time++;
	game->cooldown++;
	game->soundcooldown++;

	// Time that the text will apear in the screen
	if (game->status == STATUS_LIVES) {
		if (game->time > 250) {
			shutdown_status_lives(game);
			game->status = STATUS_GAME;
		}
	}

	else if (game->status == STATUS_GAMEOVER) {
		if (game->time > 250) {
			shutdown_game_over(game);
			SDL_Quit();
			exit(0);
		}
	}

	// Player Movements
	else if (game->status == STATUS_GAME) {
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
			if (game->soundcooldown >= 20) {
				Mix_PlayChannel(-1, game->shipShoot, 0);
      	game->soundcooldown = 0;
      }

			if (game->cooldown >= 10) {
				spawnBullet(game->move.x+10, game->move.y, 0, -5);

				if (game->point >= 10) {
					game->pointW = 115;
				}
				else if (game->point >= 100) {
					game->pointW = 125;
				}
				else if (game->point >= 1000) {
					game->pointW = 135;
				}
				else if (game->point >= 10000) {
					game->pointW = 145;
				}
				else if (game->point >= 100000) {
					game->pointW = 155;
				}
				game->cooldown = 0;
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
	if (game->status == STATUS_GAME && game->time % 12 == 0) {
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
	if (game->status == STATUS_GAME && game->time % 12 == 0) {
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
	if (game->status == STATUS_GAME && game->time % 12 == 0) {
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

	// Move Ufos
	if (game->status == STATUS_GAME && game->time % 12 == 0) {
		spawnUfos(690, random()%480-32, -3, 0);
	}
	
	for (int i = 0; i < MAXIMUM_UFOS; i++) if (ufoship[i]) {
		ufoship[i]->x += ufoship[i]->dx;
		ufoship[i]->y += ufoship[i]->dy;

		// Delete small asteroids
		if (ufoship[i]->x < -50 || ufoship[i]->x > 690) {
			deleteUfos(i);
		}
	}

	if (game->status == STATUS_GAME) {
		if (game->move.dead && game->countdown < 0) {
			game->countdown = 1;
		}

		if (game->countdown >= 0) {
			game->countdown--;

			if (game->countdown < 0) {
				game->move.lives--;

				if (game->move.lives >= 0) {
					init_status_lives(game);
					game->status = STATUS_LIVES;
					game->time = 0;

					game->move.dead = 0;
					game->move.x = 295;
					game->move.y = 215;
				}
				else {
					init_game_over(game);
					game->status = STATUS_GAMEOVER;
					game->time = 0;
				}
			}
		}
	}

}

void detectColision (GameState *game) {
	float space = 0, ww = 640 - 24, wh = 480 - 24;
	float px = game->move.x, py = game->move.y;
	
	// Player asteroids colision
	if (game->status == STATUS_GAME) {
		for (int i = 0; i < MAXIMUM_ASTEROIDS; i++) if (bigrocks[i]) if (mediumrocks[i]) if (smallrocks[i]) {
			if (bigrocks[i]->y + 45 > game->move.y && game->move.y + 24 > bigrocks[i]->y) {
				if (bigrocks[i]->x + 45 > game->move.x && game->move.x + 24 > bigrocks[i]->x) {
					Mix_PlayChannel(-1, game->shipExplode, 0);
					game->point = 0;
					game->move.dead = 1;
				}
			}

			else if (mediumrocks[i]->y + 35 > game->move.y && game->move.y + 24 > mediumrocks[i]->y) {
				if (mediumrocks[i]->x + 35 > game->move.x && game->move.x + 24 > mediumrocks[i]->x) {
					Mix_PlayChannel(-1, game->shipExplode, 0);
					game->point = 0;
					game->move.dead = 1;
				}
			}

			else if (smallrocks[i]->y + 25 > game->move.y && game->move.y + 24 > smallrocks[i]->y) {
				if (smallrocks[i]->x + 25 > game->move.x && game->move.x + 24 > smallrocks[i]->x) {
					Mix_PlayChannel(-1, game->shipExplode, 0);
					game->point = 0;
					game->move.dead = 1;
				}
			}
		}

		for (int i = 0; i < MAXIMUM_UFOS; i++) if (ufoship[i]) {
			if (ufoship[i]->y + 25 > game->move.y && game->move.y + 24 > ufoship[i]->y) {
				if (ufoship[i]->x + 25 > game->move.x && game->move.x + 24 > ufoship[i]->x) {
					Mix_PlayChannel(-1, game->shipExplode, 0);
					game->point = 0;
					game->move.dead = 1;
				}
			}
		}
	}

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

	// Asteroids colision
	for (int i = 0; i < MAXIMUM_ASTEROIDS; i++) if (bigrocks[i]) if(bullets[i]) {
		if (bigrocks[i]->y + 45 > bullets[i]->y && bullets[i]->y + 8 > bigrocks[i]->y) {
			if (bigrocks[i]->x + 45 > bullets[i]->x && bullets[i]->x + 8 > bigrocks[i]->x) {
				Mix_PlayChannel(-1, game->roidExplode, 0);
				deleteBigAsteroids(i);
				deleteBullet(i);
				game->point++;
			}
		}
	}

	for (int i = 0; i < MAXIMUM_ASTEROIDS; i++) if (mediumrocks[i]) if(bullets[i]) {
		if (mediumrocks[i]->y + 45 > bullets[i]->y && bullets[i]->y + 8 > mediumrocks[i]->y) {
			if (mediumrocks[i]->x + 45 > bullets[i]->x && bullets[i]->x + 8 > mediumrocks[i]->x) {
				Mix_PlayChannel(-1, game->roidExplode, 0);
				deleteMediumAsteroids(i);
				deleteBullet(i);
				game->point+=2;
			}
		}
	}

	for (int i = 0; i < MAXIMUM_ASTEROIDS; i++) if (smallrocks[i]) if(bullets[i]) {
		if (smallrocks[i]->y + 45 > bullets[i]->y && bullets[i]->y + 8 > smallrocks[i]->y) {
			if (smallrocks[i]->x + 45 > bullets[i]->x && bullets[i]->x + 8 > smallrocks[i]->x) {
				Mix_PlayChannel(-1, game->roidExplode, 0);
				deleteSmallAsteroids(i);
				deleteBullet(i);
				game->point+=3;
			}
		}
	}

	for (int i = 0; i < MAXIMUM_UFOS; i++) if (ufoship[i]) if(bullets[i]) {
		if (ufoship[i]->y + 45 > bullets[i]->y && bullets[i]->y + 8 > ufoship[i]->y) {
			if (ufoship[i]->x + 45 > bullets[i]->x && bullets[i]->x + 8 > ufoship[i]->x) {
				Mix_PlayChannel(-1, game->roidExplode, 0);
				deleteUfos(i);
				deleteBullet(i);
				game->point+=5;
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

	if (game->status == STATUS_GAMEOVER) {
		draw_game_over(game);
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

		// Ufo
		for (int i = 0; i < MAXIMUM_UFOS; i++) if (ufoship[i]) {
			SDL_Rect ufoRect = {ufoship[i]->x, ufoship[i]->y, 32, 32};
			SDL_RenderCopy(renderer, game->ufo, NULL, &ufoRect);
		}

		// Bullets
		for (int i = 0; i < MAXIMUM_PROJECTILES; i++) if (bullets[i]) {
			SDL_Rect bulletRect = {bullets[i]->x, bullets[i]->y, 8, 8};
			SDL_RenderCopy(renderer, game->bullet, NULL, &bulletRect);
		}

		draw_player_points(game);
	}

	SDL_RenderPresent(renderer);
}

int main (int argc, char *argv[]) {

	GameState gameState;

	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

	TTF_Init();
	
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

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
	
	Mix_PlayMusic(gameState.bgMusic, -1);

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
	SDL_DestroyTexture(gameState.ufo);
	SDL_DestroyTexture(gameState.rock[0]);
	SDL_DestroyTexture(gameState.rock[1]);
	SDL_DestroyTexture(gameState.rock[2]);
	SDL_DestroyTexture(gameState.star);
	SDL_DestroyTexture(gameState.bullet);
	SDL_DestroyTexture(gameState.points);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	TTF_CloseFont(gameState.font);
	
	if (gameState.label != NULL) {
		SDL_DestroyTexture(gameState.label);
	}

	for (int i = 0; i < MAXIMUM_PROJECTILES; i++) {
		deleteBullet(i);
	}

	Mix_FreeMusic(gameState.bgMusic);
	Mix_FreeChunk(gameState.shipShoot);
	Mix_FreeChunk(gameState.roidExplode);
	Mix_FreeChunk(gameState.shipExplode);

	Mix_CloseAudio();

	TTF_Quit();

	SDL_Quit();

	return 0;
}
