#include <SDL.h>
#include "game.h"
#include <stdio.h>
#include <time.h>
#include <SDL_ttf.h>


//BIEN REVERIFIER LE TRUC DE QUAND UNE BALLE TOUCHE CAR MES ENCADRÉS SONT PAS EXACTS
//FAIRE UNE FONCTION GAME OVER
//METTRE LES FONCTIONS DANS UN ORDRE QUI FAIT SENS PAR PIT
//IL FAUT QUE JE DEMANDE COMMEND AFFICHEER DU TEXTE EN DSL PARCE QUE HUD VA PAS

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


void update(Entity *player, Entity *bullet, Entity *grille, bool *bullet_active, float dt, float Vy)
{

    for (int i=0; i<5; i++){
        for (int j=0; j<10; j++){
            Entity enemy=grille[i*10+j];
            enemy.vy=Vy;
            enemy.y+=enemy.vy*dt;
            grille[i*10+j]=enemy;
        }
    }
    

    player->x += player->vx * dt;

    if (player->x < 0)
        player->x = 0;
    if (player->x + player->w > SCREEN_WIDTH)
        player->x = SCREEN_WIDTH - player->w;

    if (*bullet_active)
    {
        bullet->y += bullet->vy * dt;
        if (bullet->y + bullet->h < 0)
            *bullet_active = false;
    }
}




void check_if_player_hit_enemy(SDL_Renderer *renderer, Entity *player, Entity *bullet, Entity *grille, bool *bullet_active, float dt, int *score){
    for (int i=0; i<5; i++){
        for (int j=0; j<10; j++){
            Entity *enemy=&grille[i*10+j];
            if (*bullet_active){
                if (bullet->x >= enemy->x && bullet->x <= enemy->x + enemy->w && bullet->y >= enemy->y && bullet->y <= enemy->y + enemy->h){
                    grille[i*10 + j].x = 0;
                    grille[i*10 + j].y = 0;
                    grille[i*10 + j].w = 0;
                    grille[i*10 + j].h = 0;
                    grille[i*10 + j].vx = 0;
                    grille[i*10 + j].vy = 0;
                    grille[i*10 + j].alive = false;
                    *bullet_active = false;
                    (*score)+=1;

                }
            }
        }
    }
}


void Enemiesdraw(SDL_Renderer *renderer, Entity *grille) {
    for (int i = 0; i < 50; i++) {

        if (!grille[i].alive)
            continue;

        SDL_Rect enemies_rect = {
        (int)grille[i].x, (int)grille[i].y,
        grille[i].w, grille[i].h};

        SDL_RenderFillRect(renderer, &enemies_rect);
    }
}

void enemies_shoot(Entity *grille, float dt,Entity *enemy_bullets,bool enemy_bullets_active[]) {
    static float timer = 0.0f; 
    timer += dt;
    if (timer >= 1.5f) {
        timer = 0.0f;
        int essais = 0;
        int indi = -1;
        while(essais < 10) {
            int r = rand() % 50;
            if (grille[r].alive) {
                indi = r;
                break;
            }
            essais+=1;
        }
        if (indi != -1) {
            for (int i = 0; i < NB_ENEMY_BULLETS; i++) {
                if (enemy_bullets_active[i]==false) {
                    enemy_bullets_active[i] = true;
                    enemy_bullets[i].x = grille[indi].x + (grille[indi].w / 2) - 2;
                    enemy_bullets[i].y = grille[indi].y + grille[indi].h;
                    enemy_bullets[i].w = ENEMY_BULLET_WIDTH;
                    enemy_bullets[i].h = ENEMY_BULLET_HEIGHT;
                    enemy_bullets[i].vy = ENEMY_BULLET_SPEED; 
                    break; 
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


void check_if_enemy_hit_player(Entity *player, int *life, bool *running,Entity *enemy_bullets,bool enemy_bullets_active[] ) {
    for (int i = 0; i < NB_ENEMY_BULLETS; i++) {
        if (enemy_bullets_active[i]) {
            if (enemy_bullets[i].x < player->x + player->w &&
                enemy_bullets[i].x + enemy_bullets[i].w > player->x &&
                enemy_bullets[i].y < player->y + player->h &&
                enemy_bullets[i].y + enemy_bullets[i].h > player->y) {
                
                enemy_bullets_active[i]=false;
                (*life)-=1;
                if (*life <=0){
                    printf("PARTIE TERMINÉE.");
                    *running = false; 
                }
            }
        }
    }
}

void draw_lives(SDL_Renderer *renderer, int score, int lives) {

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
    for (int i = 0; i < score; i++) {
        SDL_Rect scoreRect = { 10 + (i * 12), 10, 10, 10 };
        SDL_RenderFillRect(renderer, &scoreRect);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); 
    for (int i = 0; i < lives; i++) {
        SDL_Rect lifeRect = { (SCREEN_WIDTH - 30) - (i * 25), 10, 20, 10 };
        SDL_RenderFillRect(renderer, &lifeRect);
    }
}


void update_heart(Entity *heart, Entity *player, int *life, float dt) {
    if (heart->alive==true) {
        heart->y += heart->vy * dt;
        if (heart->x < player->x + player->w &&
            heart->x + heart->w > player->x &&
            heart->y < player->y + player->h &&
            heart->y + heart->h > player->y) {
            (*life)++; //mettre un nombre de vie max?
            heart->alive = false;
        }
        if (heart->y > SCREEN_HEIGHT) {
            heart->alive = false;
    }
}
}



void render(SDL_Renderer *renderer, Entity *player, Entity *grille, Entity *bullet, bool bullet_active, Entity *enemy_bullets, bool *enemy_bullets_active, int score, int lives, Entity *heart)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
   
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

    draw_lives(renderer, score, lives);

    SDL_RenderPresent(renderer);
}




void cleanup(SDL_Window *window, SDL_Renderer *renderer)
{
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);
    SDL_Quit();
}


void win(SDL_Renderer *renderer, Entity *grille, SDL_Window **window){
    int k=0;
    for (int i=0; i<50; i++) {
        if (grille[i].alive==false){
            k+=1;
        }
    }
    if (k==50){
        printf("BRAVO TU AS GAGNÉ");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(0);

    }
    return ;
}

void loose(bool *running, SDL_Renderer *renderer, SDL_Window **window, Entity *player, Entity *grille, Entity *bullet, bool bullet_active){
    int a=0;
    Entity EnemY;
    for (int i=49; i>=0; i--){
        Entity enemy= grille[i];
        if (enemy.alive==true){
            a=i;
            EnemY=enemy;
            break;
        }

    }
    if (-PLAYER_HEIGHT<=EnemY.y-player->y && EnemY.y-player->y<=PLAYER_HEIGHT){
        printf("YOU LOOSE BIG LOOSER !");
        *running=false;
    }

}
