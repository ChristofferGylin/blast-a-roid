#include "enemies.h"
#include "explosion.h"
#include "constants.h"
#include "gameContext.h"
#include <stdio.h>
#include "outOfBoundsCheck.h"
#include "ship.h"
#include "shooting.h"
#include "raylib.h"
#include "raymath.h"
#include <math.h>

void compactShotPool(ShotObjectPool* pool);
void initEnemy(GameContext* ctx, Enemy* enemy, EnemyType type);
void initUfo1(GameContext* ctx, Enemy* enemy);
void handleEnemyShooting(GameContext* ctx, Enemy* enemy);
void handleUfoMovement(Enemy* enemy);
void ufoGoOffScreen(Enemy* enemy);
void updateUfo1(GameContext* ctx, Enemy* enemy);

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

void compactEnemyPool(EnemyObjectPool* pool) {
    int write = 0;

    for (int i = 0; i < pool->activeCount; i++) {
        if (pool->enemies[i].active) {
            if (write != i) {
                pool->enemies[write] = pool->enemies[i];
            }
            write++;
        }
    }

    for (int i = write; i < pool->activeCount; i++) {
        pool->enemies[i].active = false;
    }

    pool->activeCount = write;
}

void handleEnemiesHitDetection(GameContext* ctx) {

    bool enemyPoolHasChanges = false;

    for (int i = 0; i < ctx->objectPools.shots.activeCount; i++) {
        ShotPoolObject* shotObj = &ctx->objectPools.shots.shots[i];
        
        if (shotObj->shot.owner == ENEMY_SHOT && !ctx->ship.destroyed) {

            if (ctx->ship.isShieldActive && CheckCollisionCircles(shotObj->shot.position, shotObj->shot.size / 2.0f, ctx->ship.position, SHIELD_SIZE / 2.0f)) {
                destroyShot(shotObj);
                continue;

            } else if (CheckCollisionCircles(shotObj->shot.position, shotObj->shot.size / 2.0f, ctx->ship.position, SHIP_SIZE / 2.0f)) {
                destroyShip(ctx);

                if (shotObj->shot.level <= 1) {
                    destroyShot(shotObj);
                    continue;
                }
            }

        }

        if (shotObj->shot.owner == ENEMY_SHOT) continue;

        for (int j = 0; j < ctx->objectPools.enemies.activeCount; j++) {

            Enemy* enemy = &ctx->objectPools.enemies.enemies[j].enemy;

            if (CheckCollisionCircles(enemy->position, enemy->size / 2.0f, shotObj->shot.position, shotObj->shot.size / 2.0f)) {
                
                int damage = 10 + (shotObj->shot.level * 10);
                
                enemy->health -= damage;

                if (enemy->health <= 0) {
                    newExplosion(ctx, enemy->position);
                    ctx->objectPools.enemies.enemies[j].active = false;
                    enemyPoolHasChanges = true;

                    if (shotObj->shot.level <= 1) {
                        destroyShot(shotObj);
                    }
                } else {

                    if (enemy->isMoveable) {

                        const int knockbackForce = 35;
                        Vector2 hitDirection = Vector2Subtract(enemy->position, shotObj->shot.position);
                        hitDirection = Vector2Normalize(hitDirection);

                        enemy->velocity.x += hitDirection.x * knockbackForce;
                        enemy->velocity.y += hitDirection.y * knockbackForce;
                    }

                    destroyShot(shotObj);
                }
            }
        }
    }

    if (enemyPoolHasChanges) {
        compactEnemyPool(&ctx->objectPools.enemies);
    }
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

void handleEnemyShooting(GameContext* ctx, Enemy* enemy) {

    ShootingProperties* shotProps = &enemy->shooting;

    int coolDownTime = 0;

    if (shotProps->shotCount == 0) {
        coolDownTime = shotProps->salvoRate;
    } else {
        coolDownTime = shotProps->fireRate;
    }

    if ((GetTime() * 1000) > shotProps->lastShot + coolDownTime) {

        Vector2 aimPos = ctx->ship.position;

        if (GetRandomValue(1, 10) > shotProps->perfectHitChance) {
            float theta = GetRandomValue(0, 360);
            float radius = sqrtf(GetRandomValue(0, 1000) / 1000.0f) * shotProps->spreadRadian;

            aimPos.x += cosf(theta) * radius;
            aimPos.y += sinf(theta) * radius;
        }
        
        float angle = atan2(aimPos.y - enemy->position.y, aimPos.x - enemy->position.x);

        Shot newShot = {
            ENEMY_SHOT,
            shotProps->shot.level,
            shotProps->shot.sprite,
            shotProps->shot.size,
            enemy->position,
            {cosf(angle) * shotProps->shot.velocity, sinf(angle) * shotProps->shot.velocity},
            (GetTime() * 1000.0) + shotProps->shot.lifetime,
            false
        };

        addNewShot(&ctx->objectPools.shots, newShot);
        shotProps->lastShot = GetTime() * 1000.0;
        shotProps->shotCount++;

        if (shotProps->shotCount >= shotProps->salvoSize) {
            shotProps->shotCount = 0;
        }
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

    enemy->rotation = 0.0f;
    enemy->lastReaction = GetTime();
    enemy->spawnTime = GetTime();

    enemy->shooting.shotCount = 0;
    enemy->shooting.lastShot = GetTime() * 1000;

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
    enemy->health = 100;
    enemy->maxVelocity = 50.0f;
    enemy->isMoveable = true;
    enemy->position = (Vector2){SIDEBAR_WIDTH - (UFO_1_SIZE / 2), y};
    enemy->reactionTime = 0.3f;
    enemy->size = UFO_1_SIZE;
    enemy->type = UFO_1;
    enemy->velocity = (Vector2){0, 0};
    enemy->visualType = VISUAL_ANIMATION;

    enemy->shooting.fireRate = 500;
    enemy->shooting.perfectHitChance = 3;
    enemy->shooting.salvoRate = 3000;
    enemy->shooting.salvoSize = 1;
    enemy->shooting.spreadRadian = SHIP_SIZE * 3;

    enemy->shooting.shot = getShotProps(ctx, GREEN_SHOT_1);

    AnimationInstance animation;

    animation.animation = &ctx->assets.animations.ufo1;
    animation.currentFrame = 0;
    animation.frameTimer = 0.0f;
    animation.isFinished = false;
    animation.position = enemy->position;
    animation.rotation = enemy->rotation;

    enemy->animation = animation;
}

void removeEnemy(EnemyObjectPool* pool, Enemy* enemy) {
    for (int i = 0; i < pool->activeCount; i++) {
        if (&pool->enemies[i].enemy == enemy) {
            pool->enemies[i].active = false;
            return;
        }
    }
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
            updateUfo1(ctx, enemy);
            break;
        
        default:
            break;
        }
    }
    
}

void ufoGoOffScreen(Enemy* enemy) {

    Vector2 destination;
    destination.x = SCREEN_WIDTH - SIDEBAR_WIDTH + (UFO_1_SIZE / 2);

    if (enemy->position.y <= SCREEN_HEIGHT / 2) {
        destination.y = 50;
    } else {
        destination.y = SCREEN_HEIGHT - 50;
    }

    enemy->destination = destination;
        
    if (enemy->position.x > destination.x) {
        // remove enemy
    }
}

void updateUfo1(GameContext* ctx, Enemy* enemy) {
    handleEnemyShooting(ctx, enemy);
    ufoGoOffScreen(enemy);
}

void updateUfo2(Enemy* enemy, Ship* ship) {
    
    double now = GetTime();
    
    if (now < enemy->lastReaction + enemy->reactionTime) return;

    enemy->lastReaction = now;
    
    int attackDurationTime = 45;

    if (now <= enemy->spawnTime + attackDurationTime) {
        enemy->destination = ship->position;
    } else {

        ufoGoOffScreen(enemy);
    }
}