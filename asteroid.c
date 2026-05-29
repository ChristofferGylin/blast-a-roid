#include "raylib.h"
#include "asteroid.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "constants.h"
#include "utils.h"
#include "ship.h"
#include "shooting.h"
#include "player.h"
#include "score.h"
#include "outOfBoundsCheck.h"
#include "animation.h"
#include "explosion.h"
#include "gameContext.h"

void addNewAsteroid(AsteroidPool* pool, Asteroid ast) {
    
    if (pool->activeCount >= MAX_ASTEROIDS) {
        printf("Error: Memory overflow in addNewAsteroid\n");
        return;
    }

    if (pool->asteroids[pool->activeCount].active) {
        printf("Error: Could not add new asteroid, index allready in use in addNewAsteroid\n");
        return;
    }

    pool->asteroids[pool->activeCount].asteroid = ast;
    pool->asteroids[pool->activeCount].active = true;
    pool->activeCount++;
}

void compactAsteroidPool(AsteroidPool* pool) {
    int write = 0;

    for (int i = 0; i < pool->activeCount; i++) {
        if (pool->asteroids[i].active) {
            if (write != i) {
                pool->asteroids[write] = pool->asteroids[i];
            }
            write++;
        }
    }

    for (int i = write; i < pool->activeCount; i++) {
        pool->asteroids[i].active = false;
    }

    pool->activeCount = write;
}

int countAsteroids(AsteroidPool* pool) {
    int count = 0;

    for (int i = 0; i < pool->activeCount; i++) {
        if (!pool->asteroids[i].active) continue;

        if (
            pool->asteroids[i].asteroid.type != METAL_ASTEROID &&
            pool->asteroids[i].asteroid.type != SPIKY_ASTEROID
        ) {
            count++;
        }
    }

    return count;
}

void destroyAsteroid(DestroyedAsteroidPool* pool, AsteroidPoolObject* ast) {
    ast->asteroid.destroyed = true;
    pool->asteroids[pool->activeCount] = ast;
    pool->activeCount++;
}

int getAsteroidSize(AsteroidType type) {
    
    int size = 0;
    
    switch (type)
    {
        case ASTEROID_LEVEL_1: size = ASTEROID_SIZE_1; break;
        case ASTEROID_LEVEL_2: size = ASTEROID_SIZE_2; break;
        case ASTEROID_LEVEL_3: size = ASTEROID_SIZE_3; break;
        case METAL_ASTEROID: size = METAL_ASTEROID_SIZE; break;
        case SPIKY_ASTEROID: size = SPIKY_ASTEROID_SIZE; break;
        default: printf("Error: Invalid asteroid type (%d) in getAsteroidSize\n", type);
    }

    return size;
}

void handleAsteroidCollisions(GameContext* ctx) {

    Ship* ship = &ctx->ship;

    if (ship->destroyed) return; 

    for (int i = 0; i < ctx->objectPools.asteroids.activeCount; i++) {

        if (!ctx->objectPools.asteroids.asteroids[i].active) continue;

        Asteroid* ast = &ctx->objectPools.asteroids.asteroids[i].asteroid;

        if (ast->destroyed) continue;

        float asteroidRadius = ast->size / 2.0f;

        if (ship->isShieldActive) {
            if (CheckCollisionCircles(ship->position, SHIELD_SIZE / 2.0f, ast->position, asteroidRadius)) {
                
                if (ast->type == METAL_ASTEROID || ast->type == SPIKY_ASTEROID) {
                    knockbackByImpact(ship->position, &ship->velocity, ast->position, ast->velocity);
                } else {
                    newExplosion(ctx, ast->position);
                    destroyAsteroid(&ctx->objectPools.destroyedAsteroids, &ctx->objectPools.asteroids.asteroids[i]);
                }
            
                continue;
            }
        } else {
            if (CheckCollisionCircles(ctx->ship.position, SHIP_SIZE / 2.0f, ast->position, asteroidRadius)) {
                destroyShip(ctx);
                destroyAsteroid(&ctx->objectPools.destroyedAsteroids, &ctx->objectPools.asteroids.asteroids[i]);
                continue;
            }
        }

        for (int j = 0; j < ctx->objectPools.shots.activeCount; j++) {
            ShotPoolObject* shotObj = &ctx->objectPools.shots.shots[j];

            if (CheckCollisionCircles(shotObj->shot.position, shotObj->shot.size / 2.0f, ast->position, asteroidRadius)) {
                
                if (shotObj->shot.owner == PLAYER_SHOT) {
                    addScore(&ctx->player, ast);
                }

                if (shotObj->shot.level >= 1) {

                    ast->health--;

                    if (ast->health <= 0) {
                        newExplosion(ctx, ast->position);
                        destroyAsteroid(&ctx->objectPools.destroyedAsteroids, &ctx->objectPools.asteroids.asteroids[i]);
                    } else {
                        const int knockbackForce = 200;
                        knockback(ast->position, &ast->velocity, shotObj->shot.position, knockbackForce);
                    }
                } 

                if (shotObj->shot.level <= 1) {
                    destroyShot(shotObj);
                }

                break;
            }
        }
    }
}

void handleAsteroidsMovement(AsteroidPool* pool) {
    int spriteWidth = 32;

    for (int i = 0; i < pool->activeCount; i++) {
        if (!pool->asteroids[i].active) continue;

        Asteroid* ast = &pool->asteroids[i].asteroid;

        if (ast->destroyed) continue;

        updateRotation(&ast->rotation, ast->rotationSpeed);
        updatePosition(&ast->position, ast->velocity);

        int asteroidSize = getAsteroidSize(ast->type);

        outOfBoundsCheck(&ast->position, asteroidSize);
    }
}

void handleDestroyedAsteroids(GameContext* ctx) {
    if (ctx->objectPools.destroyedAsteroids.activeCount == 0) return;

    for (int i = 0; i < ctx->objectPools.destroyedAsteroids.activeCount; i++) {
        Asteroid* ast = &ctx->objectPools.destroyedAsteroids.asteroids[i]->asteroid;

        int numberOfNew = 0;
        AsteroidType newType = 0;

        switch (ast->type) {
            case ASTEROID_LEVEL_1:
                numberOfNew = 3;
                newType = ASTEROID_LEVEL_2;
                break;
            case ASTEROID_LEVEL_2:
                numberOfNew = 2;
                newType = ASTEROID_LEVEL_3;
                break;
            case ASTEROID_LEVEL_3:
            case SPIKY_ASTEROID:
                break;
            case METAL_ASTEROID:

                int levelSquared = ctx->player.level * ctx->player.level;
                int chanceOfSpikyAsteroid = levelSquared;
                int chanceOfNothing = 50 - levelSquared;
                
                if (chanceOfSpikyAsteroid > 90) chanceOfSpikyAsteroid = 90;
                if (chanceOfNothing < 0) chanceOfNothing = 0;

                int chance = GetRandomValue(0, 99);

                if (chance < chanceOfNothing) {
                    continue;
                } else if (chance < chanceOfNothing + chanceOfSpikyAsteroid) {
                    numberOfNew = 1;
                    newType = SPIKY_ASTEROID;
                } else {
                    numberOfNew = 3;
                    newType = ASTEROID_LEVEL_2;
                }

                break; 
            default:
                printf("Error: Invalid asteroid type (%d) in handleDestroyedAsteroids\n", ast->type);
                continue;
        }

        for (int j = 0; j < numberOfNew; j++) {
            Asteroid newAst;
            initAsteroid(ctx, &newAst, newType);
            newAst.position = ctx->objectPools.destroyedAsteroids.asteroids[i]->asteroid.position;

            addNewAsteroid(&ctx->objectPools.asteroids, newAst);
        }
    }

    for (int i = 0; i < ctx->objectPools.destroyedAsteroids.activeCount; i++) {
        ctx->objectPools.destroyedAsteroids.asteroids[i]->active = false;
    }

    compactAsteroidPool(&ctx->objectPools.asteroids);
    ctx->objectPools.destroyedAsteroids.activeCount = 0;
}

void initAsteroidPool(AsteroidPool* pool) {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        pool->asteroids[i].active = false;
    }

    pool->activeCount = 0;
}

void initDestroyedAsteroidPool(DestroyedAsteroidPool* pool) {
    for (int i = 0; i < pool->activeCount; i++) {
        AsteroidPoolObject* poolObj = pool->asteroids[i];
        poolObj->active = false;
    }

    pool->activeCount = 0;
}

void initAsteroid(GameContext* ctx, Asteroid* ast, AsteroidType type) {
    
    ast->type = type;
    ast->destroyed = false;
    ast->size = getAsteroidSize(type);
    resetAsteroid(ast);

    if (type == METAL_ASTEROID) {
        ast->health = 100;
        ast->visualType = VISUAL_ANIMATION;

        int fps = GetRandomValue(20, 60);
        bool isReversed = GetRandomValue(0, 1);

        AnimationInstance aniInst;
        initAnimtionInstance(&aniInst, &ctx->assets.animations.metalAsteroid, ast->position, 0, (float)fps, isReversed);
        ast->animation = aniInst;

    } else  if (type == SPIKY_ASTEROID) {
        ast->health = 30;
        ast->visualType = VISUAL_ANIMATION;

        AnimationInstance aniInst;
        initAnimtionInstance(&aniInst, &ctx->assets.animations.metalAsteroid, ast->position, 0, ctx->assets.animations.metalAsteroid.fps, false);
        ast->animation = aniInst;
    } else {
        ast->health = 1;
        ast->visualType = VISUAL_SPRITE;

        ast->sprite = &ctx->assets.sprites.asteroid;
    }
    
}

void initAsteroids(GameContext* ctx) {
    int numberOfAsteroids = getNumberOfAsteroids(ctx->player.level);
    int numberOfMetalAsteroids = 4;
    int level = ctx->player.level;

    int chanceOfMetal = (level + 1) * (level + 1);

    if (chanceOfMetal > 70) chanceOfMetal = 70;

    int chance = GetRandomValue(0, 99);

    if (chance < chanceOfMetal) {
        int minNumberOfMetal = 1;
        float maxNumberOfMetal = ceil(level / 2.0f);

        if (maxNumberOfMetal < minNumberOfMetal) {
            maxNumberOfMetal = minNumberOfMetal;
        } else if (maxNumberOfMetal > numberOfAsteroids / 2.0f) {
            maxNumberOfMetal = numberOfAsteroids / 2.0f;
        }

        numberOfMetalAsteroids = GetRandomValue(minNumberOfMetal, maxNumberOfMetal);
        numberOfAsteroids = numberOfAsteroids - numberOfMetalAsteroids;
    }

    for (int i = 0; i < numberOfAsteroids; i++) {
        Asteroid ast;
        initAsteroid(ctx, &ast, ASTEROID_LEVEL_1);
        addNewAsteroid(&ctx->objectPools.asteroids, ast);
    }

    for (int i = 0; i < numberOfMetalAsteroids; i++) {
        Asteroid ast;
        initAsteroid(ctx, &ast, METAL_ASTEROID);
        addNewAsteroid(&ctx->objectPools.asteroids, ast);
    }
}

void renderAsteroids(GameContext* ctx) {
    for (int i = 0; i < ctx->objectPools.asteroids.activeCount; i++) {

        if (!ctx->objectPools.asteroids.asteroids[i].active) continue;
        Asteroid* ast = &ctx->objectPools.asteroids.asteroids[i].asteroid;

        if (ast->destroyed) continue;

        if (ast->visualType == VISUAL_ANIMATION) {
            renderAnimation(&ast->animation);
        } else {
            DrawTexturePro(
                *ast->sprite,
                (Rectangle){0, 0, ast->sprite->width, ast->sprite->height},
                (Rectangle){ast->position.x, ast->position.y, ast->size, ast->size},
                (Vector2){ast->size / 2.0f, ast->size / 2.0f},
                ast->rotation,
                WHITE  
            );
        }
    }
}

void resetAllAsteroids(AsteroidPool* pool) {
    for (int i = 0; i < pool->activeCount; i++) {
        if (!pool->asteroids[i].active) continue;
        
        resetAsteroid(&pool->asteroids[i].asteroid);
    }
}

void resetAsteroid(Asteroid* ast) {
    bool directionX = GetRandomValue(0,1);
    bool directionY = GetRandomValue(0,1);
    int tempVelocityX = GetRandomValue(30, 100);
    int tempVelocityY = GetRandomValue(30, 100);
    Vector2 velocity = {directionX ? tempVelocityX : -tempVelocityX, directionY ? tempVelocityY : -tempVelocityY};
    Vector2 position;
        
    if (GetRandomValue(0,1) == 1) {
        position.x = -32.0f;
        position.y = GetRandomValue(0, GetScreenHeight());
    } else {
        position.x = GetRandomValue(0, GetScreenWidth());
        position.y = -32.0f;
    }

    ast->position = position;
    ast->rotation = 0;

    if (ast->type == METAL_ASTEROID || ast->type == SPIKY_ASTEROID) {
        ast->rotationSpeed = 0;
    } else {
        ast->rotationSpeed = GetRandomValue(-100, 100);
    }
    
    ast->velocity = velocity;
}

void updateAsteroidsAnimations(GameContext* ctx) {
    
    AsteroidPool* pool = &ctx->objectPools.asteroids;

    if (pool->activeCount == 0) return;
    
    for (int i = 0; i < pool->activeCount; i++) {
        if (!pool->asteroids[i].active) continue;

        Asteroid* ast = &pool->asteroids[i].asteroid;

        if (ast->visualType == VISUAL_ANIMATION) {
            updateAnimation(&ast->animation);
            ast->animation.position = ast->position;
        }
    }
}




