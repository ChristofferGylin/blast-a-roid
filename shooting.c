#include "shooting.h"
#include "constants.h"
#include "ship.h"
#include <math.h>
#include <stdio.h>
#include "outOfBoundsCheck.h"
#include "gameContext.h"

double lastShot = 0;

void addNewShot(ShotObjectPool* pool, Shot shot) {
    if (pool->activeCount >= MAX_SHOTS) {
        printf("Error: Memory overflow in addNewShot\n");
        return;
    }

    if (pool->shots[pool->activeCount].active) {
        printf("Error: Could not add new shot, index allready in use in addNewShot\n");
        return;
    }
    
    pool->shots[pool->activeCount].shot = shot;
    pool->shots[pool->activeCount].active = true;
    pool->activeCount++;
}

void clearShots(ShotObjectPool* pool) {
    
    bool isChanged = false;
    
    for (int i = 0; i < pool->activeCount; i++) {
        if (!pool->shots[i].active) continue;

        if (pool->shots[i].shot.destroyed) {
            pool->shots[i].active = false;
            isChanged = true;
        }

        if (GetTime() * 1000.0 >= pool->shots[i].shot.destroyTime) {
            pool->shots[i].active = false;
            isChanged = true;
        }
    }

    if (isChanged) {
        compactShotPool(pool);
    }
}

void compactShotPool(ShotObjectPool* pool) {
    int write = 0;

    for (int i = 0; i < pool->activeCount; i++) {
        if (pool->shots[i].active) {
            if (write != i) {
                pool->shots[write] = pool->shots[i];
            }
            write++;
        }
    }

    for (int i = write; i < pool->activeCount; i++) {
        pool->shots[i].active = false;
    }

    pool->activeCount = write;
}

void destroyShot(ShotPoolObject* shot) {
    shot->shot.destroyed = true;
}

ShotProperties getShotProps(GameContext* ctx, ShotType type) {
    switch (type)
    {
    case GREEN_SHOT_1:
        return (ShotProperties){
            &ctx->assets.sprites.enemyShot1,
            0,
            3000,
            4,
            150
        };
        break;
    
    default:
        printf("Error: Invalid Shot Type in getShotProps\n");
        break;
    }
}

void handleShooting(GameContext* ctx) {

    const int SHOT_COOLDOWN_TIME = 100;
    const int SHOT_LIFE_TIME = 800;
    const int SHOT_SIZE = 6;
    const int SHOT_VELOCITY = 450;

    if (ctx->ship.destroyed) return;
    if (ctx->objectPools.shots.activeCount >= MAX_SHOTS) return;
    if (GetTime() * 1000.0 <= lastShot + SHOT_COOLDOWN_TIME) return;

    bool shoot = false;

    if (IsKeyPressed(KEY_RIGHT_CONTROL) || (ctx->player.powerups.fullAuto && IsKeyDown(KEY_RIGHT_CONTROL))) {

        float radians[] = {
            (ctx->ship.rotation - 90.0f) * (PI / 180.0f),
            (ctx->ship.rotation - 55.0f) * (PI / 180.0f),
            (ctx->ship.rotation - 125.0f) * (PI / 180.0f)
        };

        double nowMillis = GetTime() * 1000.0f;
        int lifetime = ctx->player.powerups.longShot ? nowMillis + (SHOT_LIFE_TIME * 2) : nowMillis + SHOT_LIFE_TIME;
        int numberOfShots = ctx->player.powerups.trippleShot ? 3 : 1;

        for (int i = 0; i < numberOfShots; i++) {
            Shot newShot = {
                PLAYER_SHOT,
                1,
                &ctx->assets.sprites.shot,
                SHOT_SIZE,
                ctx->ship.position,
                {cosf(radians[i]) * SHOT_VELOCITY, sinf(radians[i]) * SHOT_VELOCITY},
                lifetime,
                false
            };

            addNewShot(&ctx->objectPools.shots, newShot);
        }
        
        PlaySound(ctx->assets.samples.shot);
        lastShot = nowMillis;
    }
}

void handleShotsMovement(ShotObjectPool* pool) {
    for (int i = 0; i < pool->activeCount; i++) {
        if (!pool->shots[i].active) continue;

        Shot* shot = &pool->shots[i].shot;

        if (shot->destroyed) continue;
        
        shot->position.x += GetFrameTime() * shot->direction.x;
        shot->position.y += GetFrameTime() * shot->direction.y;

        outOfBoundsCheck(&shot->position, shot->size);
    }
}

void initShotObjectPool(ShotObjectPool* pool) {
    for (int i = 0; i < MAX_SHOTS; i++) {
        pool->shots[i].active = false;
    }

    pool->activeCount = 0;
}

void renderShots(GameContext* ctx) {
    for (int i = 0; i < ctx->objectPools.shots.activeCount; i++) {

        if (!ctx->objectPools.shots.shots[i].active) continue;
        Shot* shot = &ctx->objectPools.shots.shots[i].shot;

        if (shot->destroyed) continue;

        DrawTexturePro(
            *shot->sprite,
            (Rectangle){0, 0, ctx->assets.sprites.shot.width, ctx->assets.sprites.shot.height},
            (Rectangle){shot->position.x, shot->position.y, shot->size, shot->size},
            (Vector2){shot->size / 2.0f, shot->size / 2.0f},
            0,
            WHITE  
        );
    }
}