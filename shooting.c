#include "shooting.h"
#include "constants.h"
#include "ship.h"
#include <math.h>
#include <stdio.h>

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

        if (pool->shots[i].shot.destroyTime >= GetTime() * 1000.0) {
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

void handleShooting(Ship* ship, ShotObjectPool* pool) {
    if (ship->destroyed) return;
    if (pool->activeCount >= MAX_SHOTS) return;

    if (IsKeyPressed(KEY_RIGHT_CONTROL) && GetTime() * 1000 > lastShot + SHOT_COOLDOWN_TIME) {
        float radians = (ship->rotation - 90.0f) * (PI / 180.0f);

        Shot newShot = {
            ship->position,
            {cosf(radians) * SHOT_VELOCITY, sinf(radians) * SHOT_VELOCITY},
            GetTime() + SHOT_LIFE_TIME,
            false
        };

        addNewShot(pool, newShot);
        lastShot = GetTime() * 1000;
    }
}

void handleShotsMovement(ShotObjectPool* pool) {
    int spriteWidth = 32;

    for (int i = 0; i < pool->activeCount; i++) {
        if (!pool->shots[i].active) continue;

        Shot* shot = &pool->shots[i].shot;

        if (shot->destroyed) continue;
        
        shot->position.x += GetFrameTime() * shot->direction.x;
        shot->position.y += GetFrameTime() * shot->direction.y;

        if (shot->position.x < 0.0f - spriteWidth)
        {
            shot->position.x = GetScreenWidth() + spriteWidth;
        } else if (shot->position.x > GetScreenWidth() + spriteWidth)
        {
            shot->position.x = 0.0f - spriteWidth; 
        }

        if (shot->position.y < 0.0f - spriteWidth)
        {
            shot->position.y = GetScreenHeight() + spriteWidth;
        } else if (shot->position.y > GetScreenHeight() + spriteWidth)
        {
            shot->position.y = 0.0f - spriteWidth; 
        }
    }
}

void initShotObjectPool(ShotObjectPool* pool) {
    for (int i = 0; i < MAX_SHOTS; i++) {
        pool->shots->active = false;
    }

    pool->activeCount = 0;
}

void renderShots(ShotObjectPool* pool, Texture2D* shotSprite) {
    for (int i = 0; i < pool->activeCount; i++) {

        if (!pool->shots[i].active) continue;
        Shot* shot = &pool->shots[i].shot;

        if (shot->destroyed) continue;

        DrawTexturePro(
            *shotSprite,
            (Rectangle){0, 0, shotSprite->width, shotSprite->height},
            (Rectangle){shot->position.x, shot->position.y, SHOT_SIZE, SHOT_SIZE},
            (Vector2){shotSprite->width / 2.0f, shotSprite->height / 2.0f},
            0,
            WHITE  
        );
    }
}