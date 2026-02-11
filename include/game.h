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
#define ENEMY_SPEED 10.0f
#define ENEMIES_NUMBER 50
#define ENEMIES_LINE 10
#define ENEMIES_COLS 5

#define NB_ENEMY_BULLETS 2 
#define ENEMY_BULLET_SPEED 150.0f
#define ENEMY_BULLET_WIDTH 5
#define ENEMY_BULLET_HEIGHT 10



bool init(SDL_Window **window, SDL_Renderer **renderer);
void handle_input(bool *running, const Uint8 *keys, Entity *player, Entity *bullet, bool *bullet_active);
void update(Entity *player, Entity *bullet,Entity *grille, bool *bullet_active, float dt, float Vy);
void render(SDL_Renderer *renderer, Entity *player, Entity *grille, Entity *bullet, bool bullet_active, Entity *enemy_bullets, bool *enemy_bullets_active,int score, int lives,Entity *heart);
void cleanup(SDL_Window *window, SDL_Renderer *renderer);
void check_if_player_hit_enemy(SDL_Renderer *renderer, Entity *player, Entity *bullet, Entity *grille, bool *bullet_active, float dt,int *score);
void Enemiesdraw(SDL_Renderer *renderer, Entity *grille);
void win(SDL_Renderer *renderer, Entity *grille, SDL_Window **window);
void loose(bool *running, SDL_Renderer *renderer, SDL_Window **window, Entity *player, Entity *grille, Entity *bullet, bool bullet_active);
void enemies_shoot(Entity *grille, float dt,Entity *enemy_bullets,bool enemy_bullets_active[]);
void update_enemy_bullets(float dt,Entity *enemy_bullets,bool enemy_bullets_active[]);
void check_if_enemy_hit_player(Entity *player, int *life, bool *running,Entity *enemy_bullets,bool enemy_bullets_active[]);
void draw_lives(SDL_Renderer *renderer, int score, int lives);
void update_heart(Entity *heart, Entity *player, int *life, float dt);
void show_game_over(SDL_Renderer *renderer, int final_score);


#endif
