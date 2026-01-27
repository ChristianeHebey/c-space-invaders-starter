#include <SDL.h>
#include <stdbool.h>
#include "entity.h"
#include "game.h"




int main(void)
{

    bool dead=false;
    int i=0;
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
        .vy = 0};
  

    Entity bullet = {0};
    bool bullet_active = false;

    while (running)
    {
        Uint32 ticks = SDL_GetTicks();
        float dt = (ticks - last_ticks) / 1000.0f;
        if (dt > 0.05f)
            dt = 0.05f;
        last_ticks = ticks;

        SDL_PumpEvents();
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        handle_input(&running, keys, &player, &bullet, &bullet_active);
        update(&player, &bullet,&grille, &bullet_active, dt);
        death(renderer, &player, &bullet, grille, &bullet_active, dt);
        render(renderer, &player, &grille, &bullet, bullet_active);
        //tirs_enemis(i, dt,renderer, &player, &grille, &bullet);
        win(renderer, &grille, &window);
        loosenv1(dead, renderer, &window, &player, &grille, &bullet, &bullet_active);
    }

    cleanup(window, renderer);
    return 0;
}
