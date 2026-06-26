#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "animation.h"
#include "asteroid.h"
#include "constants.h"
#include "explosion.h"
#include "gameContext.h"
#include "outOfBoundsCheck.h"
#include "ship.h"
#include "shooting.h"
#include "specials.h"
#include "utils.h"

void addSpecialToPool(GameContext* ctx, SpecialType type);
void addSpecialToSpawnPool(SpecialsSpawnPool* pool, SpecialType type);
void compactSpecialsPool(SpecialsPool* pool);
void compactSpecialsSpawnPool(SpecialsSpawnPool* pool);
void updateSpecialsAnimations(SpecialsPool* pool);

static const int SPECIALS_LIFETIME = 30;
static const int COMET_VELOCITY = 200;
static const int EXTRA_LIFE_VELOCITY = 140;
static const int EXTRA_LIFE_ROTATION_SPEED = 500;

void addSpecialToPool(GameContext* ctx, SpecialType type) {
    
    SpecialsPool* pool = &ctx->objectPools.specials;
    
    Special newSpecial;
    AnimationInstance aniInstance;

    newSpecial.rotation = 0;
    newSpecial.rotationSpeed = 0;
    newSpecial.spawnTime = GetTime() - ctx->pausTimer;
    newSpecial.type = type;
    newSpecial.value = 0;
    newSpecial.velocity = (Vector2){0,0};

    float radians = 0;

    switch (type) {

        case MULTIPLIER:

            newSpecial.position = getRandomPosition();
            
            initAnimtionInstance(&aniInstance, &ctx->assets.animations.multiplier, newSpecial.position, 0, 2.0f, false);
            newSpecial.size = (Vector2){MULTIPLIER_COLLISION_SIZE, MULTIPLIER_COLLISION_SIZE};
            playSoundPositioned(ctx->assets.samples.multiplier_spawn, newSpecial.position.x);
            
            break;
    
        case COMET:
            newSpecial.position = getRandomPositionOffScreen(COMET_RENDER_SIZE_Y);
            newSpecial.rotation = GetRandomValue(0,359);
            newSpecial.size = (Vector2){COMET_RENDER_SIZE_X, COMET_RENDER_SIZE_Y};

            radians = (newSpecial.rotation - 90.0f) * (PI / 180.0f);

            newSpecial.velocity = (Vector2){cosf(radians) * COMET_VELOCITY, sinf(radians) * COMET_VELOCITY};

            initAnimtionInstance(&aniInstance, &ctx->assets.animations.comet, newSpecial.position, newSpecial.rotation, ctx->assets.animations.comet.fps, false);
            playSoundPositioned(ctx->assets.samples.multiplier_spawn, newSpecial.position.x);
            // TODO: Play unique sound
            
            break;
    
        case EXTRA_LIFE:
            newSpecial.position = getRandomPositionOffScreen(SHIP_SIZE);
            newSpecial.rotation = GetRandomValue(0,359);
            newSpecial.rotationSpeed = GetRandomValue(0,1) == 1 ? EXTRA_LIFE_ROTATION_SPEED : -EXTRA_LIFE_ROTATION_SPEED;
            newSpecial.size = (Vector2){SHIP_SIZE, SHIP_SIZE};

            radians = (newSpecial.rotation - 90.0f) * (PI / 180.0f);

            newSpecial.velocity = (Vector2){cosf(radians) * EXTRA_LIFE_VELOCITY, sinf(radians) * EXTRA_LIFE_VELOCITY};

            Ship ship;
            initShip(ctx, &ship);
            ship.position = newSpecial.position;
            ship.rotation = newSpecial.rotation;
            ship.velocity = newSpecial.velocity;
            ship.isAutoShieldActive = false;

            newSpecial.ship = ship;

            playSoundPositioned(ctx->assets.samples.multiplier_spawn, newSpecial.position.x);
            // TODO: Play unique sound
            break;
    
        case SUPERNOVA:
            // TODO: set attributes specific to type
            break;
    
        case BLACK_HOLE:
            newSpecial.position = getRandomPosition();
            newSpecial.size = (Vector2){BLACK_HOLE_SIZE, BLACK_HOLE_SIZE};
            
            initAnimtionInstance(&aniInstance, &ctx->assets.animations.blackHole, newSpecial.position, newSpecial.rotation, ctx->assets.animations.blackHole.fps, false);
            PlaySound(ctx->assets.samples.alarm);
            break;
    
        default:
            break;
    }

    if (type != EXTRA_LIFE) {
        newSpecial.animation = aniInstance;
    }

    pool->specials[pool->activeCount].active = true;
    pool->specials[pool->activeCount].special = newSpecial;

    pool->activeCount++;
}

void addSpecialToSpawnPool(SpecialsSpawnPool* pool, SpecialType type) {
    SpecialSpawn newSpecial;

    const int minSpawnDelay = 5;
    const int maxSpawnDelay = 30;

    newSpecial.type = type;
    newSpecial.spawnTime = GetTime() + GetRandomValue(minSpawnDelay, maxSpawnDelay);

    pool->specials[pool->activeCount].active = true;
    pool->specials[pool->activeCount].special = newSpecial;

    pool->activeCount++;
}

void compactSpecialsSpawnPool(SpecialsSpawnPool* pool) {
    int write = 0;
    
    for (int i = 0; i < pool->activeCount; i++) {
        if (pool->specials[i].active) {
            if (write != i) {
                pool->specials[write] = pool->specials[i];
            }
            write++;
        }
    }

    for (int i = write; i < pool->activeCount; i++) {
        pool->specials[i].active = false;
    }

    pool->activeCount = write;
}

void compactSpecialsPool(SpecialsPool* pool) {
    int write = 0;
    
    for (int i = 0; i < pool->activeCount; i++) {
        if (pool->specials[i].active) {
            if (write != i) {
                pool->specials[write] = pool->specials[i];
            }
            write++;
        }
    }

    for (int i = write; i < pool->activeCount; i++) {
        pool->specials[i].active = false;
    }

    pool->activeCount = write;
}

void handleSpecialsCollisions(GameContext* ctx) {
    Ship* ship = &ctx->ship;
    SpecialsPool* specialsPool = &ctx->objectPools.specials;
    AsteroidPool* asteroidsPool = &ctx->objectPools.asteroids;

    bool specialsPoolHasChanged = false;

    for (int i = 0; i < specialsPool->activeCount; i++) {
        if (!specialsPool->specials[i].active) continue;

        Special* special = &specialsPool->specials[i].special;

        if (special->type != EXTRA_LIFE && special->type != BLACK_HOLE) continue;

        if (special->type == EXTRA_LIFE) {

            if (special->ship.destroyed) continue;

            for (int j = 0; j < asteroidsPool->activeCount; j++) {
                if (!asteroidsPool->asteroids[j].active) continue;

                Asteroid* ast = &asteroidsPool->asteroids[j].asteroid;

                if (CheckCollisionCircles(ast->position, ast->size / 2, special->position, special->size.x / 2)) {
                    // TODO: Play extra life ship destroy sample
                    destroyShip(ctx, &special->ship);
                    destroyAsteroid(&ctx->objectPools.destroyedAsteroids, &asteroidsPool->asteroids[j]);
                    break;
                }
            }
        }

        if (CheckCollisionCircles(ship->position, SHIP_SIZE / 2, special->position, special->size.x / 2)) {
            if (special->type == EXTRA_LIFE) {
                if (special->ship.destroyed) continue;
                ctx->player.lives++;
                playSoundPositioned(ctx->assets.samples.multiplier_collect, special->position.x);
                // TODO: Play unique extra life collect sample
            } else {
                newExplosion(ctx, ship->position);
                if (!ship->destroyed) destroyShip(ctx, &ctx->ship);
            }

            specialsPool->specials[i].active = false;
            specialsPoolHasChanged = true;
            continue;
        }

    }

    if (specialsPoolHasChanged) {
        compactSpecialsPool(specialsPool);
    }
}

void handleSpecialsHitDetection(GameContext* ctx) {
    SpecialsPool* specialsPool = &ctx->objectPools.specials;
    ShotObjectPool* shotsPool = &ctx->objectPools.shots;
    Player* player = &ctx->player;

    if (specialsPool->activeCount == 0 || shotsPool->activeCount == 0) return;

    bool specialsPoolHasChanged = false;

    for (int i = 0; i < specialsPool->activeCount; i++) {

        SpecialPoolObject* specialObj = &specialsPool->specials[i];

        if (!specialObj->active) continue;

        for (int j = 0; j < shotsPool->activeCount; j++) {
            ShotPoolObject* shotObj = &shotsPool->shots[j];

            if (!shotObj->active) continue;

            Vector2 specialPos = specialObj->special.position;
            float specialRadius = specialObj->special.size.x / 2.0f;

            if (specialObj->special.type == COMET) {
                
                float radians = (specialObj->special.rotation - 90.0f) * (PI / 180.0f);

                specialPos.x = specialPos.x + (cosf(radians) * 25);
                specialPos.y = specialPos.y + (sinf(radians) * 25);

                specialRadius = COMET_COLLISION_SIZE / 2.0f;
            }

            if (CheckCollisionCircles(specialPos, specialRadius, shotObj->shot.position, shotObj->shot.size / 2.0f)) {
                
                if (shotObj->shot.owner == ENEMY_SHOT && specialObj->special.type != EXTRA_LIFE) continue;
                
                switch (specialObj->special.type) {
                    case MULTIPLIER:
                        player->powerups.levelBonusMultiplier = specialObj->special.animation.currentFrame + 2;
                        playSoundPositioned(ctx->assets.samples.multiplier_collect, specialObj->special.position.x);
                        break;
                    
                    case COMET:
                        int bonus = GetRandomValue(1,5) * 1000;
                        player->levelBonus += bonus;
                        playSoundPositioned(ctx->assets.samples.multiplier_collect, specialObj->special.position.x);
                        // TODO: Play unique sound
                        // TODO: Flash score on screen at hit pos
                        break;
                    
                    case EXTRA_LIFE:
                        if (!specialObj->special.ship.destroyed) {
                            destroyShip(ctx, &specialObj->special.ship);
                            // TODO: Play extra ship destroy sound
                        }
                        break;
                    
                    case BLACK_HOLE:
                        newExplosion(ctx, specialObj->special.position);
                        player->levelBonus += 5000;
                        break;
                    
                    case SUPERNOVA:
                        // handle special hit
                        break;
                    default:
                        printf("Error: Invalid SpecialType (%d) in handleEnemiesHitDetection\n", specialObj->special.type);
                }

                if (specialObj->special.type != EXTRA_LIFE || specialObj->special.type == EXTRA_LIFE && !specialObj->special.ship.destroyed) {
                    destroyShot(shotObj);
                    specialObj->active = false;
                    specialsPoolHasChanged = true;
                }      
            }
        }
    }

    if (specialsPoolHasChanged) {
        compactSpecialsPool(specialsPool);
    }
}

void handleSpecialsMovement(SpecialsPool* pool) {
    for (int i = 0; i < pool->activeCount; i++) {
        SpecialPoolObject* specialObj = &pool->specials[i];
        if (!specialObj->active) continue;

        if (specialObj->special.type == EXTRA_LIFE) {
            if (specialObj->special.ship.destroyed) continue;

            updateRotation(&specialObj->special.rotation, specialObj->special.rotationSpeed);
            updatePosition(&specialObj->special.position, specialObj->special.velocity);
            handleOutOfBounds(&specialObj->special.position, specialObj->special.size.y);

            specialObj->special.ship.rotation = specialObj->special.rotation;
            specialObj->special.ship.position = specialObj->special.position;

        } else {
            updatePosition(&specialObj->special.position, specialObj->special.velocity);
            handleOutOfBounds(&specialObj->special.position, specialObj->special.size.y);
        }
    }
}

void initSpecialsPool(SpecialsPool* pool) {
    for (int i = 0; i < NUMBER_OF_SPECIALS; i++) {
        pool->specials[i].active = false;
    }

    pool->activeCount = 0;
}

void initSpecialsSpawnPool(GameContext* ctx) {

    SpecialsSpawnPool* pool = &ctx->objectPools.specialsSpawn;

    for (int i = 0; i < NUMBER_OF_SPECIALS; i++) {
        pool->specials[i].active = false;
    }

    pool->activeCount = 0;

    populateSpecialsSpawnPool(ctx);
}

void populateSpecialsSpawnPool(GameContext* ctx) {
    
    SpecialSpawnOption optionPool[NUMBER_OF_SPECIALS] = {
        (SpecialSpawnOption){true, MULTIPLIER, 100},
        (SpecialSpawnOption){true, COMET, 100},
        (SpecialSpawnOption){true, EXTRA_LIFE, 30},
        (SpecialSpawnOption){true, BLACK_HOLE, 20},
        (SpecialSpawnOption){true, SUPERNOVA, 10},
    };
    SpecialsSpawnPool* spawnPool = &ctx->objectPools.specialsSpawn;
    
    int minNumberOfSpecials = 0;
    int maxNumberOfSpecials = ceil((ctx->player.level / 2));

    if (maxNumberOfSpecials > NUMBER_OF_SPECIALS) maxNumberOfSpecials = NUMBER_OF_SPECIALS;

    int numberToPopulate = GetRandomValue(minNumberOfSpecials, maxNumberOfSpecials);

    for (int i = 0; i < numberToPopulate; i++) {
        
        int sumOfWeight = 0;

        for (int j = 0; j < NUMBER_OF_SPECIALS; j++) {
        
            if (!optionPool[j].active) continue;
            sumOfWeight += optionPool[j].weight;
        }

        int randomSelect = GetRandomValue(0, sumOfWeight - 1);

        for (int j = 0; j < NUMBER_OF_SPECIALS; j++) {

            if (!optionPool[j].active) continue;

            SpecialSpawnOption* option = &optionPool[j];

            if (randomSelect < option->weight) {
                addSpecialToSpawnPool(&ctx->objectPools.specialsSpawn, option->type);
                option->active = false;
                break;
            }

            randomSelect -= option->weight;
        }
    }

}

void renderMultiplierIcon(GameContext* ctx, int level, Vector2 position) {

    AnimationInstance aniInst;
    initAnimtionInstance(&aniInst, &ctx->assets.animations.multiplier, (Vector2){0,0}, 0, 0, false);
    aniInst.currentFrame = level - 2;
    aniInst.position = position;
    renderAnimation(&aniInst);
}

void renderSpecials(SpecialsPool* pool) {
    for (int i = 0; i < pool->activeCount; i++) {
        if (!pool->specials[i].active) continue;

        Special* special = &pool->specials[i].special;

        if (special->type == EXTRA_LIFE) {
            renderShip(&special->ship);
        } else {
            renderAnimation(&special->animation);
        }
    }
}

void spawnSpecials(GameContext* ctx) {
    for (int i = 0; i < ctx->objectPools.specialsSpawn.activeCount; i++) {
        SpecialSpawnPoolObject* spawnObj = &ctx->objectPools.specialsSpawn.specials[i];

        if (!spawnObj->active) continue;

        if (spawnObj->special.spawnTime + ctx->pausTimer <= GetTime()) {
            addSpecialToPool(ctx, spawnObj->special.type);
            spawnObj->active = false;
        }
    }
}

void updateSpecials(GameContext* ctx) {

    SpecialsPool* pool = &ctx->objectPools.specials;

    if (pool->activeCount == 0) return;

    updateSpecialsAnimations(pool);

    bool specialsPoolHasChanged = false;
    
    for (int i = 0; i < pool->activeCount; i++) {

        SpecialPoolObject* specialObj = &pool->specials[i];

        if (!specialObj->active) continue;

        if (((specialObj->special.spawnTime + SPECIALS_LIFETIME) < (GetTime() - ctx->pausTimer)) ||
              (specialObj->special.type == EXTRA_LIFE && specialObj->special.ship.destroyed)) {

            if (specialObj->special.type == EXTRA_LIFE) {
                
                if (!specialObj->special.ship.destroyed) {
                    destroyShip(ctx, &specialObj->special.ship);
                } 

                bool outOfBounds = handleDestroyedPiecesMovement(&specialObj->special.ship);

                if (outOfBounds) {
                    specialObj->active = false;
                    specialsPoolHasChanged = true;
                    continue;
                }

            } else {
                specialObj->active = false;
                specialsPoolHasChanged = true;
                continue;
            }
        }
 
        switch (specialObj->special.type) {

            case MULTIPLIER:
            case COMET:
            case EXTRA_LIFE:
                break;
    
            case SUPERNOVA:
                // TODO: set attributes specific to type
                break;
    
            case BLACK_HOLE:
                const float MAX_PULL_VELOCITY = 600.0f;
                const float ACCELERATION = 5.0f;
                goToDestination(ctx->ship.position, specialObj->special.position, &ctx->ship.velocity, MAX_PULL_VELOCITY, ACCELERATION);
                break;
    
            default:
                printf("Error: Invalid SpecialType (%d) in updateSpecials\n", specialObj->special.type);
                break;
        }
    }

    if (specialsPoolHasChanged) {
        compactSpecialsPool(pool);
    }
}

void updateSpecialsAnimations(SpecialsPool* pool) {
    for (int i = 0; i < pool->activeCount; i++) {
        if (!pool->specials[i].active || pool->specials[i].special.type == EXTRA_LIFE) continue;

        Special* special = &pool->specials[i].special;

        special->animation.position = special->position;
        updateAnimation(&special->animation);
    } 
}