#include <SDL.h>
#include <stdbool.h>
#include "entity.h"
#include "game.h"
#include <time.h>
#include <SDL_ttf.h>


int main(void)
{   
    int score = 0;
    int life = 3;

    srand(time(NULL));
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (!init(&window, &renderer))
    {
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("Erreur TTF: %s\n", TTF_GetError());
        return 1;
    }
    TTF_Font *font = TTF_OpenFont("Arial.ttf", 15);; 
    

    bool running = true;
    Uint32 last_ticks = SDL_GetTicks();

    Entity grille[ENEMIES_NUMBER];

    for (int i = 0; i < 5; i++) { 
        for (int j = 0; j < 10; j++) { 
            Entity enemy = {
                .x = 180 + (j * 45),
                .y = 80 + (i * 35),
                .w = 30,  
                .h = 20,  
                .alive = true,
                .vy = 1 
            };
            grille[i * 10 + j] = enemy;
        }
    }


    Entity player = {
        .x = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2,
        .y = SCREEN_HEIGHT -60,
        .w = PLAYER_WIDTH,
        .h = PLAYER_HEIGHT,
        .vx = 0,
        .alive=true,
        .vy = BULLET_SPEED};

    Entity bullet_enemy;       
    bool bullet_enemy_active = false; 
    float enemy_shoot_timer = 0.0f;

    Entity bullet = {0};
    bool bullet_active = false;

    Entity enemy_bullets[NB_ENEMY_BULLETS];
    bool enemy_bullets_active[NB_ENEMY_BULLETS] = {false};

    Entity heart = { 
        .x = 0, 
        .y = 0, 
        .w = 20, 
        .h = 20, 
        .alive = false, 
        .vy = 120.0f };

    float heart_timer = 0.0f;

    float enemy_speed=10.0f;

    while (running)
    {   
        Uint32 ticks = SDL_GetTicks();
        float dt = (ticks - last_ticks) / 1000.0f;
        if (dt > 0.05f) dt = 0.05f;
        last_ticks = ticks;

        enemy_speed=10.0f + (score * 2.0f);

        heart_timer += dt;
        if (heart_timer >= 5.0f && !heart.alive) {
            heart.x = rand() % (SCREEN_WIDTH - heart.w);
            heart.y = -20;
            heart.alive = true;
            heart_timer = 0.0f;
        }

        SDL_PumpEvents();
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        handle_input(&running, keys, &player, &bullet, &bullet_active);
        
        update_heart(&heart, &player, &life, dt);

        update(&player, &bullet, grille, &bullet_active, dt,enemy_speed);
        enemies_shoot(grille, dt, enemy_bullets, enemy_bullets_active);
        update_enemy_bullets(dt, enemy_bullets, enemy_bullets_active);
        
        check_if_player_hit_enemy(renderer, &player, &bullet, grille, &bullet_active, dt, &score);
        check_if_enemy_hit_player(&player, &life, &running, enemy_bullets, enemy_bullets_active);
        
        win(renderer, grille, &window,font, &running);
        loose2(&running, renderer, &window, &player, grille, &bullet, &bullet_active,font);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        render(renderer, &player, grille, &bullet, bullet_active, enemy_bullets, enemy_bullets_active, score, life, &heart);

        draw_hud(renderer, font, score, life);

        if (player.alive==false) {
            player.alive = false; 
            loose2(&running, renderer, &window, &player, grille, &bullet, &bullet_active, font);
            SDL_RenderPresent(renderer);
        }

    SDL_RenderPresent(renderer);
    }
    TTF_CloseFont(font);
    TTF_Quit();
    cleanup(window, renderer);
    return 0;
}