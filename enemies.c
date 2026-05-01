#include "enemies.h"
#include "constants.h"
#include "gameContext.h"
#include <stdio.h>

void initEnemy(GameContext* ctx, Enemy* enemy, EnemyType type);
void initUfo1(GameContext* ctx, Enemy* enemy);
void handleUfoMovement(Enemy* enemy);

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

void handleEnemiesMovement(EnemyObjectPool* pool) {
    for (int i = 0; i < pool->activeCount; i++) {
        if (!pool->enemies[i].active) continue;

        Enemy* enemy = &pool->enemies[i];

        switch (enemy->type) {
            case UFO_1:
                handleUfoMovement(enemy);
                break;
    
            default:
                break;
        }
    }
}

void handleUfoMovement(Enemy* enemy) {
    if (enemy->position.x < enemy->destinaton.x) {
        enemy->velocity.x += enemy->acceleration;
    } else if (enemy->position.x > enemy->destinaton.x) {
        enemy->velocity.x -= enemy->acceleration;
    }

    if (enemy->velocity.x > enemy->maxVelocity) {
        enemy->velocity.x = enemy->maxVelocity;
    } else if (enemy->velocity.x < -enemy->maxVelocity) {
        enemy->velocity.x = -enemy->maxVelocity;
    }

    if (enemy->position.y < enemy->destinaton.y) {
        enemy->velocity.y += enemy->acceleration;
    } else if (enemy->position.y > enemy->destinaton.y) {
        enemy->velocity.y -= enemy->acceleration;
    }

    if (enemy->velocity.y > enemy->maxVelocity) {
        enemy->velocity.y = enemy->maxVelocity;
    } else if (enemy->velocity.y < -enemy->maxVelocity) {
        enemy->velocity.y = -enemy->maxVelocity;
    }

    enemy->position.x += GetFrameTime() * enemy->velocity.x;
    enemy->position.y += GetFrameTime() * enemy->velocity.y;
}

void initEnemy(GameContext* ctx, Enemy* enemy, EnemyType type) {

    enemy->health = 1.0f;
    enemy->rotation = 0.0f;
    enemy->shotCount = 0;
    enemy->shotTime = GetTime();

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

    AnimationInstance animation;

    animation.animation = &ctx->assets.animations.ufo1;
    animation.currentFrame = 0;
    animation.frameTimer = 0.0f;
    animation.isFinished = false;
    animation.position = enemy->position;
    animation.rotation = enemy->rotation;

    enemy->acceleration = 1.0f;
    enemy->destinaton = (Vector2){SCREEN_WIDTH + 10, y};
    enemy->maxVelocity = 100.0f;
    enemy->position = (Vector2){-10, y};
    enemy->type = UFO_1;
    enemy->velocity = (Vector2){0, 0};
    enemy->visualType = VISUAL_ANIMATION;
    enemy->animation = animation;
}

void renderEnemies(EnemyObjectPool* pool) {
    for (int i = 0; i < pool->activeCount; i++) {
        if (!pool->enemies[i].active) continue;

        Enemy* enemy = &pool->enemies[i];

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

void updateEnemies(EnemyObjectPool* pool) {
    for (int i = 0; i < pool->activeCount; i++) {
        if (!pool->enemies[i].active) continue;

        Enemy* enemy = &pool->enemies[i];

        if (enemy->visualType == VISUAL_ANIMATION) {
            enemy->animation.position = enemy->position;
            updateAnimation(&enemy->animation);
        } 
    }
}