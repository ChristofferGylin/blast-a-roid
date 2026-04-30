#ifndef ENEMIES_H
#define ENEMIES_H

#include "raylib.h"

typedef struct Animation Animation;
typedef struct GameContext GameContext;

typedef enum EnemyType {
    UFO_1
}EnemyType;

typedef enum VisualType {
    VISUAL_SPRITE,
    VISUAL_ANIMATION
}VisualType;

typedef struct Enemy {
    float acceleration;
    Vector2 position;
    Vector2 destinaton;
    float rotation;
    Vector2 velocity;
    float maxVelocity;
    float health;
    double shotTime;
    int shotCount;
    EnemyType type;
    VisualType visualType;
    union {
        Texture2D* sprite;
        Animation* animation;
    };

}Enemy;

void initEnemy(GameContext* ctx, Enemy* enemy, EnemyType type);

#endif