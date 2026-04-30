#ifndef ENEMIES_H
#define ENEMIES_H

#include "raylib.h"
typedef enum EnemyType {
    UFO_1
}EnemyType;

typedef struct Enemy {
    Vector2 position;
    Vector2 destinaton;
    float velocity;
    float health;
    double shotTime;
    int shotCount;
    EnemyType type;
}Enemy;

#endif