#ifndef SHOOTING_H
#define SHOOTING_H

#include "raylib.h"
#include "ship.h"
#include <stdbool.h>


#define MAX_SHOTS 1024

typedef struct GameContext GameContext;

typedef enum ShotOwner {
    PLAYER_SHOT,
    ENEMY_SHOT
}ShotOwner;

typedef enum ShotType {
    GREEN_SHOT_1
}ShotType;

typedef struct ShotProperties {
    Texture2D* sprite;
    int level;
    int lifetime;
    int size;
    int velocity;
}ShotProperties;

typedef struct ShootingProperties {
    int perfectHitChance;
    int spreadRadian;
    double lastShot;
    int shotCount;
    int salvoSize;
    float fireRate;
    float salvoRate;
    ShotProperties shot;
}ShootingProperties;

typedef struct {
    ShotOwner owner;
    int level;
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
ShotProperties getShotProps(GameContext* ctx, ShotType type);
void handleShooting(GameContext* ctx);
void handleShotsMovement(ShotObjectPool* pool);
void initShotObjectPool(ShotObjectPool* pool);
void renderShots(GameContext* ctx);

#endif