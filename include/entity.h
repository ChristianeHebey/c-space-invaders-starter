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
    int hp;
} Entity;





#endif
