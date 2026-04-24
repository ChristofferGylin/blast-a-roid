#ifndef SHOOTING_H
#define SHOOTING_H

#include "raylib.h"
#include "ship.h"
#include <stdbool.h>

#define MAX_SHOTS 1024

typedef struct {    
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
void handleShooting(Ship* ship, ShotObjectPool* pool, Sound* shotSample);
void handleShotsMovement(ShotObjectPool* pool);
void initShotObjectPool(ShotObjectPool* pool);
void renderShots(ShotObjectPool* pool, Texture2D* shotSprite);

#endif