#include <SDL.h>
#include "game.h"
#include <stdio.h>
#include <time.h>
#include <SDL_ttf.h>

bool init(SDL_Window **window, SDL_Renderer **renderer)
{

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Erreur SDL_Init: %s", SDL_GetError());
        return false;
    }

    *window = SDL_CreateWindow("Space Invaders (SDL)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!*window)
    {
        SDL_Log("Erreur SDL_CreateWindow: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer)
    {
        SDL_Log("Erreur SDL_CreateRenderer: %s", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return false;
    }

    return true;
}

void handle_input(bool *running, const Uint8 *keys, Entity *player, Entity *bullet, bool *bullet_active)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            *running = false;
    }

    player->vx = 0.0f;
    if (keys[SDL_SCANCODE_LEFT])
        player->vx = -PLAYER_SPEED;
    if (keys[SDL_SCANCODE_RIGHT])
        player->vx = PLAYER_SPEED;

    if (keys[SDL_SCANCODE_SPACE] && !*bullet_active)
    {
        *bullet_active = true;
        bullet->x = player->x + player->w / 2 - BULLET_WIDTH / 2;
        bullet->y = player->y;
        bullet->w = BULLET_WIDTH;
        bullet->h = BULLET_HEIGHT;
        bullet->vy = -BULLET_SPEED;
    }
}



void update(Entity *player, Entity *bullet, Entity *grille, bool *bullet_active, float dt, float difficulty_factor)
{
    float vit_init = 5.0f; 

    for (int i = 0; i < 50; i++) {
    if (grille[i].alive) {
        float vit;

        if (grille[i].type == FAST) {
            vit = 10.0f; 
        } else {
            vit = vit_init; 
        }
        grille[i].y += (vit * difficulty_factor) * dt;
    }
}
    player->x += player->vx * dt;

    if (player->x < 0) player->x = 0;
    if (player->x + player->w > SCREEN_WIDTH) player->x = SCREEN_WIDTH - player->w;

    if (*bullet_active) {
        bullet->y += bullet->vy * dt;
        if (bullet->y + bullet->h < 0)
            *bullet_active = false;
    }
}

void init_level(Entity *grille, int level) {
    int rows;
    int cols = 10; 
    if (level == 1){ 
        rows = 2; 
    } 
    else if (level == 2) {
        rows = 4; 
    } 
    else { 
        rows = 5; 
    } 

    for (int i = 0; i < ENEMIES_NUMBER; i++) {
        grille[i].alive = false;
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int id = i * cols + j; 
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
}

void init_enemy(Entity *enemy, EnemyType type, float x, float y) {
    enemy->x = x;
    enemy->y = y;
    enemy->type = type;
    enemy->alive = true;
    enemy->w = 30;
    enemy->h = 20;

    switch(type) {
        case RESISTANT:
            enemy->lifep = 3; 
            enemy->vy = 5;  
            break;
        case FAST:
            enemy->lifep = 1;
            enemy->vy = 10; 
            break;
        case SNIPER:
            enemy->lifep = 1;
            enemy->vy = 5;  
            break;
        case NORMAL:
            enemy->lifep = 1;
            enemy->vy = 5; 
            break;
    }
}

void check_if_player_hit_enemy(Entity *bullet, Entity *grille, bool *bullet_active, int *score) {
    if (*bullet_active==false){
        return;
    } 

    for (int i = 0; i < 50; i++) {
        Entity *enemy = &grille[i];
        if (enemy->alive) {
            if (bullet->x < enemy->x + enemy->w && bullet->x + bullet->w > enemy->x &&
                bullet->y < enemy->y + enemy->h && bullet->y + bullet->h > enemy->y) {
                
                *bullet_active = false; 
                enemy->lifep-=1;                

                if (enemy->lifep <= 0) {
                    enemy->alive = false;
                    (*score) += 1;
                }
                break; 
            }
        }
    }
}

void Enemiesdraw(SDL_Renderer *renderer, Entity *grille) {
    for (int i = 0; i < 50; i++) {
        if (grille[i].alive==false){
            continue;
        } 

        switch(grille[i].type) {
            case RESISTANT:SDL_SetRenderDrawColor(renderer, 150, 0, 255, 255); break; 
            case FAST:SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);   break; 
            case SNIPER:SDL_SetRenderDrawColor(renderer, 255, 140, 0, 255); break; 
            case NORMAL:SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);   break; 
        }

        SDL_Rect rect = {(int)grille[i].x, (int)grille[i].y, grille[i].w, grille[i].h};
        SDL_RenderFillRect(renderer, &rect);
    }
}


void enemies_shoot(Entity *grille, float dt, Entity *enemy_bullets, bool enemy_bullets_active[]) {
    static float timer = 0.0f;
    timer += dt;
    if (timer >= 1.0f) { 
        timer = 0.0f;

        for (int i = 0; i < 50; i++) {
            if (grille[i].alive==true) {
                int chance_tir = 0;
                if (grille[i].type == SNIPER) chance_tir = 25; 
                else chance_tir = 5;                          

                if ((rand() % 100) < chance_tir) {
                    for (int j = 0; j < NB_ENEMY_BULLETS; j++) {
                        if (!enemy_bullets_active[j]) {
                            enemy_bullets_active[j] = true;
                            enemy_bullets[j].x = grille[i].x + (grille[i].w / 2) - 2;
                            enemy_bullets[j].y = grille[i].y + grille[i].h;
                            enemy_bullets[j].w = ENEMY_BULLET_WIDTH;
                            enemy_bullets[j].h = ENEMY_BULLET_HEIGHT;
                            enemy_bullets[j].vy = ENEMY_BULLET_SPEED;
                            break; 
                        }
                    }
                }
            }
        }
    }
}



void update_enemy_bullets(float dt,Entity *enemy_bullets,bool enemy_bullets_active[]) {
    for (int i = 0; i < NB_ENEMY_BULLETS; i++) {
        if (enemy_bullets_active[i]==true) {
            enemy_bullets[i].y += enemy_bullets[i].vy * dt;
            if (enemy_bullets[i].y > SCREEN_HEIGHT || enemy_bullets[i].y < -ENEMY_BULLET_HEIGHT) {
                enemy_bullets_active[i] = false;
            }
        }
    }
}


void check_if_enemy_hit_player(Entity *player, int *life,Entity *enemy_bullets,bool enemy_bullets_active[] ) {
    for (int i = 0; i < NB_ENEMY_BULLETS; i++) {
        if (enemy_bullets_active[i]) {
            if (enemy_bullets[i].x < player->x + player->w &&
                enemy_bullets[i].x + enemy_bullets[i].w > player->x &&
                enemy_bullets[i].y < player->y + player->h &&
                enemy_bullets[i].y + enemy_bullets[i].h > player->y) {
                
                enemy_bullets_active[i]=false;
                (*life)-=1;
                if (*life <=0){
                    player->alive=false;
                }
            }
        }
    }
}



void update_heart(Entity *heart, Entity *player, int *life, float dt) {
    if (heart->alive==true) {
        heart->y += heart->vy * dt;
        if (heart->x < player->x + player->w &&
            heart->x + heart->w > player->x &&
            heart->y < player->y + player->h &&
            heart->y + heart->h > player->y) {
            (*life)++; 
            heart->alive = false;
        }
        if (heart->y > SCREEN_HEIGHT) {
            heart->alive = false;
    }
}
}



void render(SDL_Renderer *renderer, Entity *player, Entity *grille, Entity *bullet, bool bullet_active, Entity *enemy_bullets, bool *enemy_bullets_active, Entity *heart)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
   
    SDL_Rect player_rect = {
        (int)player->x, (int)player->y,
        player->w, player->h};
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &player_rect);

    if (bullet_active) {
        SDL_Rect bullet_rect = {
            (int)bullet->x, (int)bullet->y,
            bullet->w, bullet->h};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &bullet_rect);
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    Enemiesdraw(renderer, grille);

    for (int i = 0; i < NB_ENEMY_BULLETS; i++) {
        if (enemy_bullets_active[i]) {
            SDL_Rect b_rect = {
                (int)enemy_bullets[i].x, (int)enemy_bullets[i].y,
                enemy_bullets[i].w, enemy_bullets[i].h};
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); 
            SDL_RenderFillRect(renderer, &b_rect);
        }
    }

    if (heart->alive==true) {
        SDL_Rect heart_rect = {(int)heart->x, (int)heart->y, heart->w, heart->h};
        SDL_SetRenderDrawColor(renderer, 255, 105, 180, 255); 
        SDL_RenderFillRect(renderer, &heart_rect);
    }

}




void cleanup(SDL_Window *window, SDL_Renderer *renderer)
{
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);
    SDL_Quit();
}



void win(SDL_Renderer *renderer, Entity *grille, TTF_Font *font, bool *running) {
    int k = 0;
    for (int i = 0; i < 50; i++) {
        if (!grille[i].alive) {
            k+=1;
        }
    }
    if (k == 50) { 
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_Surface *s = TTF_RenderText_Solid(font, "BRAVO CHAMPION.NE !", (SDL_Color){0, 255, 0, 255});
        if (s!=NULL) {
            SDL_Texture *t = SDL_CreateTextureFromSurface(renderer, s);
            SDL_Rect r = { (SCREEN_WIDTH - s->w) / 2, (SCREEN_HEIGHT - s->h) / 2, s->w, s->h };
            SDL_RenderCopy(renderer, t, NULL, &r);
            SDL_RenderPresent(renderer);

            SDL_Delay(2000); 
            
            SDL_FreeSurface(s);
            SDL_DestroyTexture(t);
        }
        *running = false; 
    }
}

void loose(SDL_Renderer *renderer, int life, Entity *grille, TTF_Font *font, bool *running) {
    bool out = false;

    for (int i = 0; i < ENEMIES_NUMBER; i++) {
        if (grille[i].alive && (grille[i].y + grille[i].h) >= (SCREEN_HEIGHT - 70)) {
            out = true;
            break;
        }
    }

    if (life <= 0 || out==true) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        SDL_Surface* s = TTF_RenderText_Solid(font, "T'AS PERDU GROS.SE LOOSEU.R.SE !", (SDL_Color){255, 0, 0, 255});
        if (s != NULL) {
            SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
            SDL_Rect r = {(SCREEN_WIDTH - s->w) / 2, (SCREEN_HEIGHT - s->h) / 2, s->w, s->h};
            SDL_RenderCopy(renderer, t, NULL, &r);
            SDL_RenderPresent(renderer);

            SDL_Delay(2000); 

            SDL_FreeSurface(s);
            SDL_DestroyTexture(t);
        }
        *running = false; 
    }
}

void draw_hud(SDL_Renderer *renderer, TTF_Font *font, int score, int lives) {

    SDL_Color white = {255, 255, 255, 255};
    char buffer[50];
    sprintf(buffer, "SCORE: %d | VIES: %d", score, lives);

    SDL_Surface *surf = TTF_RenderText_Solid(font, buffer, white);
    if (surf) {
        SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_Rect rect = { 20, 20, surf->w, surf->h }; 
        
        SDL_RenderCopy(renderer, tex, NULL, &rect);

        SDL_FreeSurface(surf);
        SDL_DestroyTexture(tex);
    }
}