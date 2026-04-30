#ifndef ENEMIES_H
#define ENEMIES_H

#include "raylib.h"

typedef struct Animation Animation;

typedef enum EnemyType {
    UFO_1
}EnemyType;

typedef enum VisualType {
    VISUAL_SPRITE,
    VISUAL_ANIMATION
}VisualType;

typedef struct Enemy {
    Vector2 position;
    Vector2 destinaton;
    float rotation;
    float velocity;
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

#endif