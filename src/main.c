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
    float difficulty = 1.0f;
    bool running = true;

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
    
    Uint32 last_ticks = SDL_GetTicks();

    Entity grille[ENEMIES_NUMBER];
    for (int i = 0; i < 5; i++) { 
        for (int j = 0; j < 10; j++) { 
            int id = i * 10 + j;
            grille[id].x = 180 + (j * 45);
            grille[id].y = 80 + (i * 35);
            grille[id].w = 30; 
            grille[id].h = 20;
            grille[id].alive = true;
            if (j == 0 || j == 9) { 
                grille[id].type = RESISTANT;
                grille[id].lifep = 3;
                grille[id].vy = 5;
            } 
            else if (j == 4 || j == 5) { 
                grille[id].type = SNIPER;
                grille[id].lifep = 1;
                grille[id].vy = 5;
            } 
            else if (j == 2 || j == 7) {
                grille[id].type = FAST;
                grille[id].lifep = 1;
                grille[id].vy = 10;
            }
            else { 
                grille[id].type = NORMAL;
                grille[id].lifep = 1;
                grille[id].vy = 5;
            }
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

    SDL_Rect pauseBtn = { SCREEN_WIDTH - 60, 20, 40, 40 };
    SDL_Rect eazBtn = { 0, 200, SCREEN_WIDTH, 60 };
    SDL_Rect midBtn = { 0, 300, SCREEN_WIDTH, 60 };
    SDL_Rect hardBtn = { 0, 400, SCREEN_WIDTH, 60 };
    GameState state = STATE_MENU;

    while (running)
    {   
        Uint32 ticks = SDL_GetTicks();
        float dt = (ticks - last_ticks) / 1000.0f;
        if (dt > 0.05f) dt = 0.05f; 
        last_ticks = ticks;

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) running = false;

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mx = event.button.x;
                int my = event.button.y;

                if (state == STATE_MENU) {
                    if (mx >= eazBtn.x && mx <= eazBtn.x + eazBtn.w && my >= eazBtn.y && my <= eazBtn.y + eazBtn.h) {
                        difficulty = 1.0f; 
                        state = STATE_PLAYING;
                    }
                    else if (mx >= midBtn.x && mx <= midBtn.x + midBtn.w && my >= midBtn.y && my <= midBtn.y + midBtn.h) {
                        difficulty = 2.0f; 
                        state = STATE_PLAYING;
                    }
                    else if (mx >= hardBtn.x && mx <= hardBtn.x + hardBtn.w && my >= hardBtn.y && my <= hardBtn.y + hardBtn.h) {
                        difficulty = 3.0f; 
                        state = STATE_PLAYING;
                    }
                }
                else if (state == STATE_PLAYING || state == STATE_PAUSE) {
                    if (mx >= pauseBtn.x && mx <= pauseBtn.x + pauseBtn.w && my >= pauseBtn.y && my <= pauseBtn.y + pauseBtn.h) {
                        if (state == STATE_PLAYING) {
                            state = STATE_PAUSE;
                        }
                        else {
                            state = STATE_PLAYING;
                        }
                    }
                }
            }
        }

        if (state == STATE_PLAYING) 
        {
            const Uint8 *keys = SDL_GetKeyboardState(NULL);
            handle_input(&running, keys, &player, &bullet, &bullet_active);

            difficulty += 0.05f * dt;
            heart_timer += dt;

            if (heart_timer >= 5.0f && !heart.alive) {
                heart.x = rand() % (SCREEN_WIDTH - heart.w);
                heart.y = -20;
                heart.alive = true;
                heart_timer = 0.0f;
            }

            update_heart(&heart, &player, &life, dt);
            update(&player, &bullet, grille, &bullet_active, dt, difficulty);
            enemies_shoot(grille, dt, enemy_bullets, enemy_bullets_active);
            update_enemy_bullets(dt, enemy_bullets, enemy_bullets_active);
            check_if_player_hit_enemy(&bullet, grille, &bullet_active, &score);
            check_if_enemy_hit_player(&player, &life, enemy_bullets, enemy_bullets_active);
            win(renderer, grille, font, &running); 
            loose(renderer, life, grille, font, &running);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        switch (state) 
        {
            case STATE_MENU:
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); 
                SDL_RenderFillRect(renderer, &eazBtn);
                SDL_Surface* sEaz = TTF_RenderText_Solid(font, "NIVEAU FACILE (FLEMMARD.E)", (SDL_Color){0, 0, 0, 255}); 
                if(sEaz) {
                    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, sEaz);
                    SDL_Rect r = { (SCREEN_WIDTH - sEaz->w)/2, eazBtn.y + (eazBtn.h - sEaz->h)/2, sEaz->w, sEaz->h };
                    SDL_RenderCopy(renderer, tex, NULL, &r);
                    SDL_FreeSurface(sEaz); 
                    SDL_DestroyTexture(tex);
                }

                SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); 
                SDL_RenderFillRect(renderer, &midBtn);
                SDL_Surface* sMid = TTF_RenderText_Solid(font, "NIVEAU INTERMEDIAIRE (JE CONSEILLE)", (SDL_Color){0, 0, 0, 255}); 
                if(sMid) {
                    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, sMid);
                    SDL_Rect r = { (SCREEN_WIDTH - sMid->w)/2, midBtn.y + (midBtn.h - sMid->h)/2, sMid->w, sMid->h };
                    SDL_RenderCopy(renderer, tex, NULL, &r);
                    SDL_FreeSurface(sMid); 
                    SDL_DestroyTexture(tex);
                }

                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
                SDL_RenderFillRect(renderer, &hardBtn);
                SDL_Surface* sHard = TTF_RenderText_Solid(font, "NIVEAU DIFFICILE (BOSS)", (SDL_Color){0, 0, 0, 255}); 
                if(sHard) {
                    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, sHard);
                    SDL_Rect r = { (SCREEN_WIDTH - sHard->w)/2, hardBtn.y + (hardBtn.h - sHard->h)/2, sHard->w, sHard->h };
                    SDL_RenderCopy(renderer, tex, NULL, &r);
                    SDL_FreeSurface(sHard); 
                    SDL_DestroyTexture(tex);
                }
                break;

            case STATE_PLAYING:
            case STATE_PAUSE:
                render(renderer, &player, grille, &bullet, bullet_active, enemy_bullets, enemy_bullets_active, &heart);
                draw_hud(renderer, font, score, life);
                SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255); 
                SDL_RenderFillRect(renderer, &pauseBtn);
                if (state == STATE_PAUSE) {
                    SDL_Surface* s = TTF_RenderText_Solid(font, "T'ES EN PAUSE BG ;)", (SDL_Color){255, 255, 0, 255});
                    if(s) {
                        SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
                        SDL_Rect r = {(SCREEN_WIDTH - s->w)/2, (SCREEN_HEIGHT - s->h)/2, s->w, s->h};
                        SDL_RenderCopy(renderer, t, NULL, &r);
                        SDL_FreeSurface(s); 
                        SDL_DestroyTexture(t);
                    }
                }
                else {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    SDL_Rect b1 = { pauseBtn.x + 10, pauseBtn.y + 10, 6, 20 };
                    SDL_Rect b2 = { pauseBtn.x + 24, pauseBtn.y + 10, 6, 20 };
                    SDL_RenderFillRect(renderer, &b1);
                    SDL_RenderFillRect(renderer, &b2);
                }
                break;
                
            default: break;
        }

        SDL_RenderPresent(renderer);
    }
    TTF_CloseFont(font);
    TTF_Quit();
    cleanup(window, renderer);
    return 0;
}