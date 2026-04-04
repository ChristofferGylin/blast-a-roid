#include "raylib.h"
#include "asteroid.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include "constants.h"
#include "utils.h"
#include "ship.h"

AsteroidPool asteroidObjectPool = {0};

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

void handleAsteroidCollisions(AsteroidPool* pool, Ship* ship) {

    if (ship->destroyed) return; 

    for (int i = 0; i < MAX_ASTEROIDS; i++) {

        if (!pool->asteroids[i].active) continue;

        Asteroid* ast = &pool->asteroids[i].asteroid;

        if (ast->destroyed) continue;

        float asteroidRadius = 0.0f;

        switch (ast->level)
        {
        case 1: asteroidRadius = ASTEROID_SIZE_1 / 2.0f; break;
        case 2: asteroidRadius = ASTEROID_SIZE_2 / 2.0f; break;
        case 3: asteroidRadius = ASTEROID_SIZE_3 / 2.0f; break;
        default: printf("Error: Invalid asteroid level (%d) in handleAsteroidCollisions\n", ast->level);
        }

        if (CheckCollisionCircles(ship->position, SHIP_SIZE / 2.0f, ast->position, asteroidRadius)) {
            ship->destroyed = true;
            ast->destroyed = true;
            return;
        } 
    }
}

void handleAsteroidsMovement(AsteroidPool* pool) {
    int spriteWidth = 32;

    for (int i = 0; i < MAX_ASTEROIDS; i++) {
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

        if (ast->position.x < 0.0f - spriteWidth)
        {
            ast->position.x = GetScreenWidth() + spriteWidth;
        } else if (ast->position.x > GetScreenWidth() + spriteWidth)
        {
            ast->position.x = 0.0f - spriteWidth; 
        }

        if (ast->position.y < 0.0f - spriteWidth)
        {
            ast->position.y = GetScreenHeight() + spriteWidth;
        } else if (ast->position.y > GetScreenHeight() + spriteWidth)
        {
            ast->position.y = 0.0f - spriteWidth; 
        }
    }
}

void handleDestroyedAsteroids(AsteroidPool* pool) {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (pool->asteroids[i].active && pool->asteroids[i].asteroid.destroyed) {
            Asteroid* oldAst = &pool->asteroids[i].asteroid;
            int numberOfNew = 0;

            switch (oldAst->level)
            {
            case 1: numberOfNew = 3; break;
            case 2: numberOfNew = 2; break;
            case 3: numberOfNew = 0; break;
            
            default: printf("Error: Invalid asteroid level (%d) in handleDestroyedAsteroids\n", oldAst->level);
            }

            for (int j = 0; j < numberOfNew; j++) {
                Asteroid newAst = {0};
                resetAsteroid(&newAst);
                newAst.level = oldAst->level + 1;
                newAst.position = oldAst->position;
                addNewAsteroid(pool, newAst);
            }

            pool->asteroids[i].active = false;
        }
    }
}

void initAsteroids(AsteroidPool* pool, int gameLevel) {
    int numberOfAsteroids = getNumberOfAsteroids(gameLevel);
    for (int i = 0; i < numberOfAsteroids; i++) {
        Asteroid ast = {0};
        resetAsteroid(&ast);
        ast.level = 1;
        ast.destroyed = false;
        addNewAsteroid(pool, ast);
    }
}

void renderAsteroids(AsteroidPool* pool, Texture2D* asteroidSprite) {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {

        if (!pool->asteroids[i].active) continue;
        Asteroid* ast = &pool->asteroids[i].asteroid;

        if (ast->destroyed) continue;

        int asteroidSize = 0;

        switch (ast->level)
        {
            case 1: asteroidSize = ASTEROID_SIZE_1; break;
            case 2: asteroidSize = ASTEROID_SIZE_2; break;
            case 3: asteroidSize = ASTEROID_SIZE_3; break;
            default: printf("Error: Invalid asteroid level (%d) in renderAsteroids\n", ast->level);
        }

        DrawTexturePro(
            *asteroidSprite,
            (Rectangle){0, 0, asteroidSprite->width, asteroidSprite->height},
            (Rectangle){ast->position.x, ast->position.y, asteroidSize, asteroidSize},
            (Vector2){asteroidSprite->width / 2.0f, asteroidSprite->height / 2.0f},
            ast->rotation,
            WHITE  
        );
    }
}

void resetAllAsteroids(AsteroidPool* pool) {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
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





