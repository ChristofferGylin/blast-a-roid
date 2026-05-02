#include "enemies.h"
#include "constants.h"
#include "gameContext.h"
#include <stdio.h>
#include "outOfBoundsCheck.h"
#include "ship.h"
#include <math.h>

void initEnemy(GameContext* ctx, Enemy* enemy, EnemyType type);
void initUfo1(GameContext* ctx, Enemy* enemy);
void handleUfoMovement(Enemy* enemy);
void updateUfo1(Enemy* enemy, Ship* ship);

void addNewEnemy(GameContext* ctx, EnemyType type) {

    EnemyObjectPool* pool = &ctx->objectPools.enemies; 

    if (pool->activeCount >= MAX_ENEMIES) {
        printf("Error: Memory overflow in addNewEnemy\n");
        return;
    }

    if (pool->enemies[pool->activeCount].active) {
        printf("Error: Could not add new enemy, index allready in use in addNewEnemy\n");
        return;
    }

    Enemy newEnemy;

    initEnemy(ctx, &newEnemy, type);

    pool->enemies[pool->activeCount].enemy = newEnemy;
    pool->enemies[pool->activeCount].active = true;
    pool->activeCount++;

}

void handleEnemiesMovement(GameContext* ctx) {
    
    EnemyObjectPool* pool = &ctx->objectPools.enemies;

    for (int i = 0; i < pool->activeCount; i++) {
        if (!pool->enemies[i].active) continue;

        Enemy* enemy = &pool->enemies[i].enemy;

        switch (enemy->type) {
            case UFO_1:
                handleUfoMovement(enemy);
                break;
    
            default:
                break;
        }

        outOfBoundsCheck(&enemy->position, enemy->size);
    }
}

void handleUfoMovement(Enemy* enemy) {
    float angle = atan2(enemy->destination.y - enemy->position.y, enemy->destination.x - enemy->position.x);

    enemy->velocity.x += GetFrameTime() * (cosf(angle) * enemy->acceleration);
    enemy->velocity.y += GetFrameTime() * (sinf(angle) * enemy->acceleration);

    if (enemy->velocity.x < -enemy->maxVelocity) {
        enemy->velocity.x = -enemy->maxVelocity;
    } else if (enemy->velocity.x > enemy->maxVelocity) {
        enemy->velocity.x = enemy->maxVelocity;
    }

    if (enemy->velocity.y < -enemy->maxVelocity) {
        enemy->velocity.y = -enemy->maxVelocity;
    } else if (enemy->velocity.y > enemy->maxVelocity) {
        enemy->velocity.y = enemy->maxVelocity;
    }

    enemy->position.x += GetFrameTime() * enemy->velocity.x;
    enemy->position.y += GetFrameTime() * enemy->velocity.y;
}

void initEnemy(GameContext* ctx, Enemy* enemy, EnemyType type) {

    enemy->health = 1.0f;
    enemy->rotation = 0.0f;
    enemy->shotCount = 0;
    enemy->lastReaction = GetTime();
    enemy->lastShot = GetTime();
    enemy->spawnTime = GetTime();

    switch (type)
    {
    case UFO_1:
        initUfo1(ctx, enemy);
        break;
    
    default:
        break;
    }
}

void initEnemyPool(EnemyObjectPool* pool) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        pool->enemies[i].active = false;
    }

    pool->activeCount = 0;
}

void initUfo1(GameContext* ctx, Enemy* enemy) {

    float y = 50.0f;

    enemy->acceleration = 100.0f;
    enemy->destination = (Vector2){SCREEN_WIDTH + UFO_1_SIZE, y};
    enemy->maxVelocity = 50.0f;
    enemy->position = (Vector2){SIDEBAR_WIDTH - UFO_1_SIZE, y};
    enemy->reactionTime = 0.3f;
    enemy->size = UFO_1_SIZE;
    enemy->type = UFO_1;
    enemy->velocity = (Vector2){0, 0};
    enemy->visualType = VISUAL_ANIMATION;

    AnimationInstance animation;

    animation.animation = &ctx->assets.animations.ufo1;
    animation.currentFrame = 0;
    animation.frameTimer = 0.0f;
    animation.isFinished = false;
    animation.position = enemy->position;
    animation.rotation = enemy->rotation;

    enemy->animation = animation;
}

void renderEnemies(EnemyObjectPool* pool) {
    for (int i = 0; i < pool->activeCount; i++) {
        if (!pool->enemies[i].active) continue;

        Enemy* enemy = &pool->enemies[i].enemy;

        if (enemy->visualType == VISUAL_SPRITE) {

            Texture2D sprite = *enemy->sprite;

            DrawTexturePro(
                sprite,
                (Rectangle){0, 0, sprite.width, sprite.height},
                (Rectangle){enemy->position.x, enemy->position.y, sprite.width, sprite.height},
                (Vector2){sprite.width / 2.0f, sprite.height / 2.0f},
                enemy->rotation,
                WHITE  
            );
        } else {
            renderAnimation(&enemy->animation);
        }
    }
}

void updateEnemies(GameContext* ctx) {

    EnemyObjectPool* pool = &ctx->objectPools.enemies;

    for (int i = 0; i < pool->activeCount; i++) {
        if (!pool->enemies[i].active) continue;

        Enemy* enemy = &pool->enemies[i].enemy;

        if (enemy->visualType == VISUAL_ANIMATION) {
            enemy->animation.position = enemy->position;
            updateAnimation(&enemy->animation);
        }

        switch (enemy->type)
        {
        case UFO_1:
            updateUfo1(enemy, &ctx->ship);
            break;
        
        default:
            break;
        }
    }
    
}

void updateUfo1(Enemy* enemy, Ship* ship) {
    
    double now = GetTime();
    
    if (now < enemy->lastReaction + enemy->reactionTime) return;

    enemy->lastReaction = now;
    
    int attackDurationTime = 45;

    if (now <= enemy->spawnTime + attackDurationTime) {
        enemy->destination = ship->position;
    } else {
        enemy->destination.x = SCREEN_WIDTH + UFO_1_SIZE / 2;
        enemy->destination.y = 50;
    }
}