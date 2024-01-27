#include <stdio.h>
#include "main.h"

void init_status_lives (GameState *game) {
	SDL_Color white = {255, 255, 255, 255};

	// Player lives
	char str[128] = "";
	sprintf(str, "X  %d", (int)game->move.lives);

	// Show player lives
	SDL_Surface *tmp = TTF_RenderText_Blended(game->font, str, white);
	game->label = SDL_CreateTextureFromSurface(game->renderer, tmp);
	SDL_FreeSurface(tmp);
}

void draw_status_lives (GameState *game) {
	
	SDL_Renderer *renderer = game->renderer;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_RenderClear(renderer);

	// Player
	SDL_Rect playerRect = {295-24, 215, 24, 24};	
	SDL_RenderCopyEx(renderer, game->player, NULL, &playerRect, 0, NULL, 0);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	// Lives
	SDL_Rect textRect = {295+24, 215-18, 56, 66};
	SDL_RenderCopy(renderer, game->label, NULL, &textRect);
}

void draw_player_points (GameState *game) {
	SDL_Color white = {255, 255, 255, 255};

	// Player points
	char point[128] = "";
	sprintf(point, "points: %d", (int)game->point);

	// Show player points
	SDL_Surface *pmt = TTF_RenderText_Blended(game->font, point, white);
	game->points = SDL_CreateTextureFromSurface(game->renderer, pmt);
	SDL_FreeSurface(pmt);
	
	// Renderer
	SDL_Renderer *renderer = game->renderer;

	// Points
	SDL_Rect pointsRect = {15, 5, game->pointW, game->pointH};
	SDL_RenderCopy(renderer, game->points, NULL, &pointsRect);

	// Player lives
	char lives[128] = "";
	sprintf(lives, "lives: %d", (int)game->move.lives);

	// Show player lives
	SDL_Surface *mtp = TTF_RenderText_Blended(game->font, lives, white);
	game->label = SDL_CreateTextureFromSurface(game->renderer, mtp);
	SDL_FreeSurface(mtp);

	// lives
	SDL_Rect livesRect = {15, 35, 95, 55};
	SDL_RenderCopy(renderer, game->label, NULL, &livesRect);
}

void shutdown_status_lives (GameState *game) {
	SDL_DestroyTexture(game->label);
	game->label = NULL;
}

void init_game_over (GameState *game) {
	SDL_Color white = {255, 255, 255, 255};

	// Show player lives
	SDL_Surface *ptm = TTF_RenderText_Blended(game->font, "Game Over", white);
	game->labelOver = SDL_CreateTextureFromSurface(game->renderer, ptm);
	SDL_FreeSurface(ptm);
}

void draw_game_over (GameState *game) {
	SDL_Renderer *renderer = game->renderer;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	// Death
	SDL_Rect deathRect = {640 / 2 - 48, 480 / 2 - 48, 135, 75};
	SDL_RenderCopy(renderer, game->labelOver, NULL, &deathRect);
}

void shutdown_game_over (GameState *game) {
	SDL_DestroyTexture(game->labelOver);
	game->labelOver = NULL;
}
