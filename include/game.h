#include <SDL_ttf.h>

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
void render(SDL_Renderer *renderer, Entity *player, Entity *grille, Entity *bullet, bool bullet_active, Entity *enemy_bullets, bool *enemy_bullets_active,Entity *heart);
void cleanup(SDL_Window *window, SDL_Renderer *renderer);
void win(SDL_Renderer *renderer, Entity *grille, TTF_Font *font, bool *running);
void update_enemy_bullets(float dt,Entity *enemy_bullets,bool enemy_bullets_active[]);
void check_if_enemy_hit_player(Entity *player, int *life,Entity *enemy_bullets,bool enemy_bullets_active[]);
void update_heart(Entity *heart, Entity *player, int *life, float dt);
void loose(SDL_Renderer *renderer, int life, Entity *grille, TTF_Font *font, bool *running);
void draw_hud(SDL_Renderer *renderer, TTF_Font *font, int score, int lives);
void check_if_player_hit_enemy(Entity *bullet, Entity *grille, bool *bullet_active, int *score);
void Enemiesdraw(SDL_Renderer *renderer, Entity *grille);
void enemies_shoot(Entity *grille, float dt, Entity *enemy_bullets, bool enemy_bullets_active[]);
void check_if_player_hit_enemy(Entity *bullet, Entity *grille, bool *bullet_active, int *score);
void update(Entity *player, Entity *bullet, Entity *grille, bool *bullet_active, float dt, float difficulty_factor);
void init_enemy(Entity *enemy, EnemyType type, float x, float y);
void init_level(Entity *grille, int level);

#endif
