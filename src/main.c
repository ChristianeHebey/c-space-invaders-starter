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

    bool running = true;
    Uint32 last_ticks = SDL_GetTicks();

    int vies[5]={1};

    Entity grille[ENEMIES_NUMBER];

    int a=(SCREEN_WIDTH-10*ENEMY_WIDTH)/10;
    for (int i=0; i<5; i++){
        for (int j=0; j<10; j++){
            Entity enemy = {
                .x = SCREEN_WIDTH/10*(j+0.5)-ENEMY_WIDTH/2,
                .y = SCREEN_HEIGHT/10*i,
                .w = ENEMY_WIDTH,
                .h = ENEMY_HEIGHT,
                .vx = 0,
                .alive=true,
                .vy = 7};
            grille[i*10+j]=enemy;
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
        
        render(renderer, &player, grille, &bullet, bullet_active, enemy_bullets, enemy_bullets_active,score,life, &heart);

        win(renderer, grille, &window);
        loose(&running, renderer, &window, &player, grille, &bullet, &bullet_active);
    }
    cleanup(window, renderer);
    return 0;
}