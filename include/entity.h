#ifndef ENTITY_H
#define ENTITY_H

typedef enum {
    NORMAL,
    RESISTANT,
    FAST,
    SNIPER
} EnemyType;


typedef struct
{
    float x, y;
    float vx, vy;
    int w, h;
    bool alive;
    EnemyType type; 
    int lifep;
} Entity;

typedef enum {
    STATE_MENU,
    STATE_PLAYING,
    STATE_PAUSE,
    STATE_WIN,
    STATE_GAMEOVER
} GameState;


#endif
