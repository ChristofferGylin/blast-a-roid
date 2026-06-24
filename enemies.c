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

void compactEnemyPool(EnemyObjectPool* pool);
void compactEnemySpawnPool(EnemySpawnPool* pool);
void initEnemy(GameContext* ctx, Enemy* enemy, EnemyType type);
void initSpikyAsteroid(GameContext* ctx, Enemy* enemy);
void initUfo1(GameContext* ctx, Enemy* enemy);
void initUfo2(GameContext* ctx, Enemy* enemy);
void initUfo3(GameContext* ctx, Enemy* enemy);
void handleEnemyShooting(GameContext* ctx, Enemy* enemy);
void handleUfoMovement(GameContext* ctx, Enemy* enemy);
Vector2 predictiveAim(Vector2 targetPosition, Vector2 targetVelocity, Vector2 sourcePosition, float time);
bool ufoGoOffScreen(GameContext* ctx, Enemy* enemy);
void updateSpikyAsteroid(GameContext* ctx, Enemy* enemy);
bool updateUfo1(GameContext* ctx, Enemy* enemy);
bool updateUfo2(GameContext* ctx, Enemy* enemy);
bool updateUfo3(GameContext* ctx, Enemy* enemy);

bool addNewEnemy(GameContext* ctx, EnemyType type, bool atPosition, Vector2 position) {

    bool success = false;

    EnemyObjectPool* pool = &ctx->objectPools.enemies;

    if (pool->activeCount >= MAX_ENEMIES) {
        printf("Error: Memory overflow in addNewEnemy\n");
        return success;
    }

    if (pool->enemies[pool->activeCount].active) {
        printf("Error: Could not add new enemy, index allready in use in addNewEnemy\n");
        return success;
    }

    success = true;

    Enemy newEnemy;

    initEnemy(ctx, &newEnemy, type);

    if (atPosition) {
        newEnemy.position = position;
    }

    pool->enemies[pool->activeCount].enemy = newEnemy;
    pool->enemies[pool->activeCount].active = true;
    pool->activeCount++;

    return success;

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

void compactEnemySpawnPool(EnemySpawnPool* pool) {
    int write = 0;

    for (int i = 0; i < pool->activeCount; i++) {
        if (pool->options[i].active) {
            if (write != i) {
                pool->options[write] = pool->options[i];
            }
            write++;
        }
    }

    for (int i = write; i < pool->activeCount; i++) {
        pool->options[i].active = false;
    }

    pool->activeCount = write;
}

void handleEnemiesCollisions(GameContext* ctx) {
    
    EnemyObjectPool* enemyPool = &ctx->objectPools.enemies;
    Ship* ship = &ctx->ship;

    if (enemyPool->activeCount == 0) return;
    
    bool enemyPoolHasChanges = false;

    for (int i = 0; i < enemyPool->activeCount; i++) {
        EnemyPoolObject* enemyObject = &enemyPool->enemies[i]; 

        if (!enemyObject->active) continue;

        Enemy* enemy = &enemyObject->enemy;

        if (ship->isShieldActive && CheckCollisionCircles(enemy->position, enemy->size / 2, ship->position, SHIELD_SIZE / 2)) {
            
            if (enemy->type == SPIKY_ASTEROID) {
                knockbackByImpact(ship->position, &ship->velocity, enemy->position, enemy->velocity);
                playSoundPositioned(ctx->assets.samples.metalPlink, enemy->position.x);
            } else {
                newExplosion(ctx, enemy->position);
                dropNewBonus(ctx, &enemyObject->enemy);

                ctx->player.score += enemy->score;
                ctx->player.levelBonus += enemy->score * ctx->player.timeBonusMultiplier; 

                enemyObject->active = false;
                enemyPoolHasChanges = true;
            }
        } else if (CheckCollisionCircles(enemy->position, enemy->size / 2, ship->position, SHIP_SIZE / 2)) {
            
            newExplosion(ctx, ship->position);
            newExplosion(ctx, enemy->position);

            if (!ship->destroyed) destroyShip(ctx, &ctx->ship);
            
            enemyObject->active = false;
            enemyPoolHasChanges = true;

        }
    }

    if (enemyPoolHasChanges) {
        compactEnemyPool(&ctx->objectPools.enemies);
    }
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
                destroyShip(ctx, &ctx->ship);

                if (shotObj->shot.level <= 1) {
                    destroyShot(shotObj);
                    continue;
                }
            }

        }

        if (shotObj->shot.owner == ENEMY_SHOT) continue;

        for (int j = 0; j < ctx->objectPools.enemies.activeCount; j++) {

            if (!ctx->objectPools.enemies.enemies[j].active) continue;

            Enemy* enemy = &ctx->objectPools.enemies.enemies[j].enemy;

            if (CheckCollisionCircles(enemy->position, enemy->size / 2.0f, shotObj->shot.position, shotObj->shot.size / 2.0f)) {
                
                enemy->health--;

                if (enemy->health <= 0) {
                    newExplosion(ctx, enemy->position);
                    dropNewBonus(ctx, enemy);
                    ctx->objectPools.enemies.enemies[j].active = false;
                    enemyPoolHasChanges = true;
                    
                    ctx->player.score += enemy->score;
                    ctx->player.levelBonus += enemy->score * ctx->player.timeBonusMultiplier; 

                    if (shotObj->shot.level <= 1) {
                        destroyShot(shotObj);
                    }
                } else {

                    if (enemy->isMoveable) {
                        const int knockbackForce = 35;
                        knockback(enemy->position, &enemy->velocity, shotObj->shot.position, knockbackForce);

                        if (enemy->type == SPIKY_ASTEROID) {
                            playSoundPositioned(ctx->assets.samples.metalPlink, enemy->position.x);
                        }
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
            case SPIKY_ASTEROID:
            case UFO_1:
            case UFO_2:
            case UFO_3:
                handleUfoMovement(ctx, enemy);
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
            float theta = GetRandomValue(0, 1000) / 1000.0f * 2.0f * PI;
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

        playSoundPositioned(ctx->assets.samples.enemyShot1, enemy->position.x);
        addNewShot(&ctx->objectPools.shots, newShot);
        shotProps->lastShot = GetTime() * 1000.0;
        shotProps->shotCount++;

        if (shotProps->shotCount >= shotProps->salvoSize) {
            shotProps->shotCount = 0;
        }
    }
}

void handleUfoMovement(GameContext* ctx, Enemy* enemy) {

    if (enemy->isAttacking && CheckCollisionCircles(enemy->position, (float)enemy->attackRange, ctx->ship.position, SHIP_SIZE / 2)) {
        brakeShip(&enemy->velocity, enemy->brakeFactor);
    } else {

        Vector2 toDestination = Vector2Subtract(enemy->destination, enemy->position);
        Vector2 desiredVelocity = Vector2Scale(Vector2Normalize(toDestination), enemy->maxVelocity);
        Vector2 steering = Vector2Subtract(desiredVelocity, enemy->velocity);

        float steerLength = Vector2Length(steering);

        if (steerLength > enemy->acceleration) {
            steering = Vector2Scale(Vector2Normalize(steering), enemy->acceleration);
        }

        enemy->velocity.x += steering.x * GetFrameTime();
        enemy->velocity.y += steering.y * GetFrameTime();

    }
    
    enemy->position.x += GetFrameTime() * enemy->velocity.x;
    enemy->position.y += GetFrameTime() * enemy->velocity.y;
}

void initEnemy(GameContext* ctx, Enemy* enemy, EnemyType type) {

    enemy->rotation = 0.0f;
    enemy->lastReaction = GetTime();
    enemy->spawnTime = GetTime();
    enemy->isAttacking = false;

    enemy->shooting.shotCount = 0;
    enemy->shooting.lastShot = GetTime() * 1000;

    switch (type)
    {
    case SPIKY_ASTEROID:
        initSpikyAsteroid(ctx, enemy);
        break;

    case UFO_1:
        initUfo1(ctx, enemy);
        break;

    case UFO_2:
        initUfo2(ctx, enemy);
        break;

    case UFO_3:
        initUfo3(ctx, enemy);
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

void initEnemySpawnPool(GameContext* ctx) {

    EnemySpawnPool* pool = &ctx->objectPools.spawnableEnemies;
    
    for (int i = 0; i < NUMBER_OF_ENEMY_TYPES; i++) {
        pool->options[i].active = false;
    }

    pool->activeCount = 0;

    int index = 0;

    if (ctx->player.level > NUMBER_OF_LEVEL_ENEMY_OPTIONS) {
        index = NUMBER_OF_LEVEL_ENEMY_OPTIONS - 1;
    } else {
        index = ctx->player.level - 1;

        if (index < 0) index = 0;
    }

    EnemySpawnOption* spawnOptions = levelsEnemyOptions[index];

    int activeIndex = 0;

    for (int i = 0; i < NUMBER_OF_ENEMY_TYPES; i++) {
        
        if (spawnOptions[i].maxCount > 0) {
            pool->options[activeIndex].option = spawnOptions[i];
            pool->options[activeIndex].active = true;
            activeIndex++;
        }
    }

    pool->activeCount = activeIndex;
}

void initSpikyAsteroid(GameContext* ctx, Enemy* enemy) {

    enemy->endPosition = (Vector2){0, 0};
    enemy->startPosition = getRandomPositionOffScreen(SPIKY_ASTEROID_SIZE);

    enemy->acceleration = 300.0f;
    enemy->brakeFactor = 3.0f;
    enemy->attackRange = 0;
    enemy->destination = ctx->ship.position;
    enemy->health = 30;
    enemy->maxVelocity = 300.0f;
    enemy->isAttacking = true;
    enemy->isMoveable = true;
    enemy->position = enemy->startPosition;
    enemy->reactionTime = 0.3f;
    enemy->size = SPIKY_ASTEROID_SIZE;
    enemy->score = 2000;
    enemy->type = SPIKY_ASTEROID;
    enemy->velocity = (Vector2){0, 0};
    enemy->visualType = VISUAL_ANIMATION;

    enemy->shooting.fireRate = 500;
    enemy->shooting.perfectHitChance = 3;
    enemy->shooting.salvoRate = 3000;
    enemy->shooting.salvoSize = 0;
    enemy->shooting.spreadRadian = 0;

    enemy->shooting.shot = getShotProps(ctx, GREEN_SHOT_1);

    AnimationInstance instance;

    bool isReversed = GetRandomValue(0, 1);

    initAnimtionInstance(&instance, &ctx->assets.animations.spikyAsteroid, enemy->position, enemy->rotation, ctx->assets.animations.spikyAsteroid.fps, isReversed);

    enemy->animation = instance;
}

void initUfo1(GameContext* ctx, Enemy* enemy) {

    float y = 50.0f;

    enemy->endPosition = (Vector2){SCREEN_WIDTH - SIDEBAR_WIDTH + (UFO_1_SIZE / 2) - 1, y};
    enemy->startPosition = (Vector2){SIDEBAR_WIDTH - (UFO_1_SIZE / 2), y};

    enemy->acceleration = 100.0f;
    enemy->brakeFactor = 3.0f;
    enemy->attackRange = 0;
    enemy->destination = enemy->endPosition;
    enemy->health = 5;
    enemy->maxVelocity = 50.0f;
    enemy->isMoveable = true;
    enemy->position = enemy->startPosition;
    enemy->reactionTime = 0.3f;
    enemy->size = UFO_1_SIZE;
    enemy->score = 500;
    enemy->type = UFO_1;
    enemy->velocity = (Vector2){0, 0};
    enemy->visualType = VISUAL_ANIMATION;

    enemy->shooting.fireRate = 500;
    enemy->shooting.perfectHitChance = 3;
    enemy->shooting.salvoRate = 3000;
    enemy->shooting.salvoSize = 1;
    enemy->shooting.spreadRadian = SHIP_SIZE * 3;

    enemy->shooting.shot = getShotProps(ctx, GREEN_SHOT_1);

    AnimationInstance instance;

    initAnimtionInstance(&instance, &ctx->assets.animations.ufo1, enemy->position, enemy->rotation, ctx->assets.animations.ufo1.fps, false);

    enemy->animation = instance;
}

void initUfo2(GameContext* ctx, Enemy* enemy) {

    float y = 50.0f;
    int size = 32;

    enemy->startPosition = getRandomPositionOffScreen(size);
    enemy->endPosition = getRandomPositionOffScreen(size);

    enemy->acceleration = 130.0f;
    enemy->brakeFactor = 0.5f;
    enemy->attackRange = 200;
    enemy->destination = ctx->ship.position;
    enemy->health = 10;
    enemy->maxVelocity = 70.0f;
    enemy->isMoveable = true;
    enemy->position = enemy->startPosition;
    enemy->reactionTime = 0.3f;
    enemy->size = size;
    enemy->score = 700;
    enemy->type = UFO_2;
    enemy->velocity = (Vector2){0, 0};
    enemy->visualType = VISUAL_ANIMATION;

    enemy->shooting.fireRate = 500;
    enemy->shooting.perfectHitChance = 3;
    enemy->shooting.salvoRate = 2000;
    enemy->shooting.salvoSize = 2;
    enemy->shooting.spreadRadian = SHIP_SIZE * 3;

    enemy->shooting.shot = getShotProps(ctx, GREEN_SHOT_1);

    AnimationInstance instance;

    initAnimtionInstance(&instance, &ctx->assets.animations.ufo2, enemy->position, enemy->rotation, ctx->assets.animations.ufo2.fps, false);

    enemy->animation = instance;
}

void initUfo3(GameContext* ctx, Enemy* enemy) {

    float y = 50.0f;
    int size = UFO_3_COLLISION_SIZE;

    enemy->startPosition = getRandomPositionOffScreen(size);
    enemy->endPosition = getRandomPositionOffScreen(size);

    enemy->acceleration = 160.0f;
    enemy->brakeFactor = 0.5f;
    enemy->attackRange = 0;
    enemy->destination = ctx->ship.position;
    enemy->health = 15;
    enemy->maxVelocity = 100.0f;
    enemy->isMoveable = true;
    enemy->position = enemy->startPosition;
    enemy->reactionTime = 0.3f;
    enemy->size = size;
    enemy->score = 1000;
    enemy->type = UFO_3;
    enemy->velocity = (Vector2){0, 0};
    enemy->visualType = VISUAL_ANIMATION;

    enemy->shooting.fireRate = 300;
    enemy->shooting.perfectHitChance = 3;
    enemy->shooting.salvoRate = 2000;
    enemy->shooting.salvoSize = 5;
    enemy->shooting.spreadRadian = SHIP_SIZE * 3;

    enemy->shooting.shot = getShotProps(ctx, GREEN_SHOT_1);

    AnimationInstance instance;

    initAnimtionInstance(&instance, &ctx->assets.animations.ufo3, enemy->position, enemy->rotation, ctx->assets.animations.ufo1.fps, false);

    enemy->animation = instance;
}

Vector2 predictiveAim(Vector2 targetPosition, Vector2 targetVelocity, Vector2 sourcePosition, float time) {
    Vector2 target = targetPosition;

    target.x += targetVelocity.x * time;
    target.y += targetVelocity.y * time;

    float angle = atan2f(
        target.y - sourcePosition.y,
        target.x - sourcePosition.x
    );

    return target;
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

void setNextEnemySpawnTime(GameContext* ctx) {
    ctx->spawning.nextSpawn = GetTime() + GetRandomValue(ctx->spawning.spawnDelay.min, ctx->spawning.spawnDelay.max);
}

void setSpawnDelay(GameContext* ctx) {
    FloatRange minDelay = {5, 10};
    FloatRange maxDelay = {15, 30};
    
    float multiplier = (ctx->player.level * 5) / 100;

    FloatRange spawnDelay = {
        maxDelay.min - (maxDelay.min * multiplier),
        maxDelay.max - (maxDelay.max * multiplier)
    };

    if (spawnDelay.min < minDelay.min) spawnDelay.min = minDelay.min;
    if (spawnDelay.max < minDelay.max) spawnDelay.max = minDelay.max;

    ctx->spawning.spawnDelay = spawnDelay;
}

void spawnEnemy(GameContext* ctx) {
    
    EnemySpawnPool* pool = &ctx->objectPools.spawnableEnemies;
    
    if (pool->activeCount == 0 || ctx->spawning.nextSpawn + ctx->pausTimer > GetTime()) return;
    setNextEnemySpawnTime(ctx);

    float sumOfWeight = 0.0f;

    for (int i = 0; i < pool->activeCount; i++) {
        sumOfWeight += pool->options[i].option.weight;
    }

    int randomSelect = GetRandomValue(0, sumOfWeight);

    for (int i = 0; i < pool->activeCount; i++) {

        EnemySpawnOption* option = &pool->options[i].option;

        if (randomSelect < option->weight) {

            bool addSuccess = addNewEnemy(ctx, option->type, false, (Vector2){0, 0});

            if (addSuccess) {

                PlaySound(ctx->assets.samples.alarm);

                option->count++;

                if (option->count >= option->maxCount) {
                    pool->options[i].active = false;

                    compactEnemySpawnPool(pool);
                }
            }

            return;
        }

        randomSelect -= option->weight;
    }
}

void updateEnemies(GameContext* ctx) {

    EnemyObjectPool* pool = &ctx->objectPools.enemies;
    bool hasPoolChanged = false;

    for (int i = 0; i < pool->activeCount; i++) {
        if (!pool->enemies[i].active) continue;

        Enemy* enemy = &pool->enemies[i].enemy;

        if (enemy->visualType == VISUAL_ANIMATION) {
            enemy->animation.position = enemy->position;
            updateAnimation(&enemy->animation);
        }

        bool change = false;

        switch (enemy->type)
        {
        case SPIKY_ASTEROID:
            updateSpikyAsteroid(ctx, enemy);
            break;
        case UFO_1:
            change = updateUfo1(ctx, enemy);
            break;

        case UFO_2:
        case UFO_3:
            change = updateUfo2(ctx, enemy);
            break;
        
        default:
            break;
        }

        if (change) {
            hasPoolChanged = true;
        }
    }

    if (hasPoolChanged) {
        compactEnemyPool(&ctx->objectPools.enemies);
    }
    
}

bool ufoGoOffScreen(GameContext* ctx, Enemy* enemy) {

    bool remove = false;

    Vector2 destination = enemy->endPosition;

    if (enemy->type == UFO_1) {
        if (enemy->position.y <= SCREEN_HEIGHT / 2) {
            destination.y = 50;
        } else {
        destination.y = SCREEN_HEIGHT - 50;
        }
    }

    enemy->destination = destination;

    if (destination.y < 0 && enemy->position.y <= destination.y) {
        remove = true;
    } else if (destination.y > SCREEN_HEIGHT && enemy->position.y >= destination.y) {
        remove = true;
    } else if (destination.x < SIDEBAR_WIDTH && enemy->position.x <= destination.x) {
        remove = true;
    } else if (destination.x > SCREEN_WIDTH - SIDEBAR_WIDTH && enemy->position.x >= destination.x) {
         remove = true;
    }

    if (remove) removeEnemy(&ctx->objectPools.enemies, enemy);

    return remove;
}

void updateSpikyAsteroid(GameContext* ctx, Enemy* enemy) {
    enemy->destination = predictiveAim(ctx->ship.position, ctx->ship.velocity, enemy->position, 0.5f);
}

bool updateUfo1(GameContext* ctx, Enemy* enemy) {
    handleEnemyShooting(ctx, enemy);
    bool hasBeenRemoved = ufoGoOffScreen(ctx, enemy);

    return hasBeenRemoved;
}

bool updateUfo2(GameContext* ctx, Enemy* enemy) {
    
    double now = GetTime();
    int attackDurationTime = 45;
    bool hasBeenRemoved = false;

    if (now <= enemy->spawnTime + attackDurationTime + ctx->pausTimer) {
        enemy->isAttacking = true;
        enemy->destination = predictiveAim(ctx->ship.position, ctx->ship.velocity, enemy->position, 0.5f);
    } else {
        enemy->isAttacking = false;
       hasBeenRemoved = ufoGoOffScreen(ctx, enemy);
    }

    handleEnemyShooting(ctx, enemy);

    return hasBeenRemoved;
}