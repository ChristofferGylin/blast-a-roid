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

void destroyAsteroid(DestroyedAsteroidPool* pool, AsteroidPoolObject* ast) {
    ast->asteroid.destroyed = true;
    pool->asteroids[pool->activeCount] = ast;
    pool->activeCount++;
}

int getAsteroidSize(int level) {
    
    int size = 0;
    
    switch (level)
    {
        case 1: size = ASTEROID_SIZE_1; break;
        case 2: size = ASTEROID_SIZE_2; break;
        case 3: size = ASTEROID_SIZE_3; break;
        default: printf("Error: Invalid asteroid level (%d) in getAsteroidSize\n", level);
    }

    return size;
}

void handleAsteroidCollisions(GameContext* ctx) {

    if (ctx->ship.destroyed) return; 

    for (int i = 0; i < ctx->objectPools.asteroids.activeCount; i++) {

        if (!ctx->objectPools.asteroids.asteroids[i].active) continue;

        Asteroid* ast = &ctx->objectPools.asteroids.asteroids[i].asteroid;

        if (ast->destroyed) continue;

        float asteroidRadius = getAsteroidSize(ast->level) / 2.0f;

        if (ctx->ship.isShieldActive) {
            if (CheckCollisionCircles(ctx->ship.position, SHIELD_SIZE / 2.0f, ast->position, asteroidRadius)) {
                newExplosion(ctx, ast->position);
                destroyAsteroid(&ctx->objectPools.destroyedAsteroids, &ctx->objectPools.asteroids.asteroids[i]);
                continue;
            }
        } else {
            if (CheckCollisionCircles(ctx->ship.position, SHIP_SIZE / 2.0f, ast->position, asteroidRadius)) {
                ctx->ship.destroyed = true;
                newExplosion(ctx, ctx->ship.position);
                destroyAsteroid(&ctx->objectPools.destroyedAsteroids, &ctx->objectPools.asteroids.asteroids[i]);
                continue;
            }
        }

        for (int j = 0; j < ctx->objectPools.shots.activeCount; j++) {
            if (CheckCollisionCircles(ctx->objectPools.shots.shots[j].shot.position, SHOT_SIZE / 2.0f, ast->position, asteroidRadius)) {
                addScore(&ctx->player, ast);
                newExplosion(ctx, ast->position);
                destroyShot(&ctx->objectPools.shots.shots[j]);
                destroyAsteroid(&ctx->objectPools.destroyedAsteroids, &ctx->objectPools.asteroids.asteroids[i]);
            
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

        ast->rotation += GetFrameTime() * ast->rotationSpeed;
        ast->rotation = fmodf(ast->rotation, 360.0f);

        if (ast->rotation < 0.0f)
        {
            ast->rotation += 360.0f;
        }
        
        ast->position.x += GetFrameTime() * ast->velocity.x;
        ast->position.y += GetFrameTime() * ast->velocity.y;

        int asteroidSize = getAsteroidSize(ast->level);

        outOfBoundsCheck(&ast->position, asteroidSize);
    }
}

void handleDestroyedAsteroids(GameContext* ctx) {
    if (ctx->objectPools.destroyedAsteroids.activeCount == 0) return;

    for (int i = 0; i < ctx->objectPools.destroyedAsteroids.activeCount; i++) {
        ctx->objectPools.destroyedAsteroids.asteroids[i]->active = false;
    }

    for (int i = 0; i < ctx->objectPools.destroyedAsteroids.activeCount; i++) {
        int numberOfNew = 0;
        int astLevel = ctx->objectPools.destroyedAsteroids.asteroids[i]->asteroid.level;

        switch (astLevel) {
            case 1: numberOfNew = 3; break;
            case 2: numberOfNew = 2; break;
            case 3: numberOfNew = 0; break; 
            default: printf("Error: Invalid asteroid level (%d) in handleDestroyedAsteroids\n", astLevel);
        }

        for (int j = 0; j < numberOfNew; j++) {
            Asteroid newAst = {0};
            resetAsteroid(&newAst);
            newAst.level = astLevel + 1;
            newAst.position = ctx->objectPools.destroyedAsteroids.asteroids[i]->asteroid.position;

            addNewAsteroid(&ctx->objectPools.asteroids, newAst);
        }
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

void initAsteroids(GameContext* ctx) {
    int numberOfAsteroids = getNumberOfAsteroids(ctx->player.level);
    for (int i = 0; i < numberOfAsteroids; i++) {
        Asteroid ast = {0};
        resetAsteroid(&ast);
        ast.level = 1;
        ast.destroyed = false;
        addNewAsteroid(&ctx->objectPools.asteroids, ast);
    }
}

void renderAsteroids(AsteroidPool* pool, Texture2D* asteroidSprite) {
    for (int i = 0; i < pool->activeCount; i++) {

        if (!pool->asteroids[i].active) continue;
        Asteroid* ast = &pool->asteroids[i].asteroid;

        if (ast->destroyed) continue;

        int asteroidSize = getAsteroidSize(ast->level);

        DrawTexturePro(
            *asteroidSprite,
            (Rectangle){0, 0, asteroidSprite->width, asteroidSprite->height},
            (Rectangle){ast->position.x, ast->position.y, asteroidSize, asteroidSize},
            (Vector2){asteroidSize / 2.0f, asteroidSize / 2.0f},
            ast->rotation,
            WHITE  
        );
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
    ast->rotationSpeed = GetRandomValue(-100, 100);
    ast->velocity = velocity;
}





