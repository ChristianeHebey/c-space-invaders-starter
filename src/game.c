#include <SDL.h>
#include "game.h"
#include <stdio.h>



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


void update(Entity *player, Entity *bullet, Entity *grille, bool *bullet_active, float dt)
{

    for (int i=0; i<5; i++){
        for (int j=0; j<10; j++){
            Entity enemy=grille[i*10+j];
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

//Fonction qui dit si le joueur est mort selon les critères du niveau 1
void death(SDL_Renderer *renderer, Entity *player, Entity *bullet, Entity *grille, bool *bullet_active, float dt){
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

//Fonction qui dit si le joueur est mort selon les critères du niveau 1
void loosenv1(bool dead, SDL_Renderer *renderer, SDL_Window **window, Entity *player, Entity *grille, Entity *bullet, bool bullet_active){
    int a=0;
    Entity EnemY;
    for (int i=49; i>=0; i--){
        Entity enemy= grille[i];
        if (enemy.alive=true){
            a=i;
            EnemY=enemy;
            break;
        }

    }
    if (-PLAYER_HEIGHT<=EnemY.y-player->y && EnemY.y-player->y<=PLAYER_HEIGHT){
        printf("YOU LOOSE BIG LOOSER !");
        dead=true;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(0);

    }

}

void render(SDL_Renderer *renderer, Entity *player, Entity *grille, Entity *bullet, bool bullet_active)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect player_rect = {
        (int)player->x, (int)player->y,
        player->w, player->h};
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &player_rect);

    if (bullet_active)
    {
        SDL_Rect bullet_rect = {
            (int)bullet->x, (int)bullet->y,
            bullet->w, bullet->h};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &bullet_rect);
    }
    
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    Enemiesdraw(renderer, grille);

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

int compteur_vivants(Entity *grille){
    int a=0;
    for (int i =0; i<50; i++){
        if (grille[i].alive=true){
            a+=1;
        }

    }
    return a;
}

Entity *tableau_des_vivants(Entity *grille) {
    int a = compteur_vivants(grille);
    int b = 0;

    Entity *vivants = malloc(a * sizeof(Entity));
    if (vivants == NULL) return NULL;

    for (int i = 0; i < 50; i++) {
        if (grille[i].alive) {
            vivants[b] = grille[i];
            b++;
        }
    }

    return vivants;
}

void tirs_enemis(int i, float dt, SDL_Renderer *renderer, Entity *player, Entity *grille, Entity *bullet){
    Entity *vivants= tableau_des_vivants(grille);
    Entity bullet_enemy = {
        .x = vivants[i].x,
        .y = vivants[i].y,
        .w = BULLET_WIDTH,
        .h = BULLET_HEIGHT,
        .vx = 0,
        .alive=true,
        .vy = -BULLET_SPEED};
    bullet->y += bullet->vy * dt;
    if (bullet_enemy.x >= player->x && bullet_enemy.x <= player->x + player->w && bullet_enemy.y >= player->y && bullet_enemy.y <= player->y + player->h){
        printf("MORT");
        i+=1%5;
    }

    SDL_Rect bullet_enemy_rect = {
            (int)bullet->x, (int)bullet->y,
            bullet->w, bullet->h};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &bullet_enemy_rect);
}