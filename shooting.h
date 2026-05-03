#ifndef SHOOTING_H
#define SHOOTING_H

#include "raylib.h"
#include "ship.h"
#include <stdbool.h>


#define MAX_SHOTS 1024

typedef struct GameContext GameContext;

typedef enum ShotType {
    PLAYER_SHOT,
    ENEMY_SHOT_1
}ShotType;

typedef struct {
    ShotType type;
    Texture2D* sprite;
    int size;
    Vector2 position;
    Vector2 direction;
    double destroyTime;
    bool destroyed;
}Shot;

typedef struct {
    bool active;
    Shot shot;
}ShotPoolObject;

typedef struct {
    ShotPoolObject shots[MAX_SHOTS];
    int activeCount;
}ShotObjectPool;

void addNewShot(ShotObjectPool* pool, Shot shot);
void clearShots(ShotObjectPool* pool);
void compactShotPool(ShotObjectPool* pool);
void destroyShot(ShotPoolObject* shot);
void handleShooting(GameContext* ctx);
void handleShotsMovement(ShotObjectPool* pool);
void initShotObjectPool(ShotObjectPool* pool);
void renderShots(GameContext* ctx);

#endif