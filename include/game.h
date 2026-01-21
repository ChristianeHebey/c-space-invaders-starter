#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <stdbool.h>
#include "entity.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define PLAYER_WIDTH 50
#define PLAYER_HEIGHT 20
#define PLAYER_SPEED 400.0f

#define BULLET_WIDTH 10
#define BULLET_HEIGHT 20
#define BULLET_SPEED 600.0f

#define ENEMY_WIDTH 20
#define ENEMY_HEIGHT 20
#define ENEMY_SPEED 400.0f
#define ENEMIES_NUMBER 50
#define ENEMIES_LINE 10
#define ENEMIES_COLS 5



bool init(SDL_Window **window, SDL_Renderer **renderer);
void handle_input(bool *running, const Uint8 *keys, Entity *player, Entity *bullet, bool *bullet_active);
void update(Entity *player, Entity *bullet,Entity *grille, bool *bullet_active, float dt);
void render(SDL_Renderer *renderer, Entity *player, Entity *grille, Entity *bullet, bool bullet_active);
void cleanup(SDL_Window *window, SDL_Renderer *renderer);







#endif
