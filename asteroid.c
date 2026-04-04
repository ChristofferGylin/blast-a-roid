#include "raylib.h"
#include "asteroid.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "constants.h"

float spriteWidth = 32.0f;
float spriteHeight = 32.0f;

void initAsteroidPool(AsteroidPool* pool) {
    pool->activeCount = 0;
}

void spawnAsteroid(AsteroidPool* pool, Asteroid ast)
{
    if (pool->activeCount >= MAX_ASTEROIDS) return;

    int i = pool->activeCount;

    pool->asteroids[i] = ast;
    pool->active[pool->activeCount] = i;

    pool->activeCount++;
}

void destroyAsteroid(AsteroidPool* pool, int index) {
    pool->asteroids[index].destroyed = true;
}

void updateAsteroidsPool(AsteroidPool* pool)
{
    int oldActive[MAX_ASTEROIDS];
    int oldCount = pool->activeCount;

    for (int i = 0; i < oldCount; i++)
        oldActive[i] = pool->active[i];

    int newActive[MAX_ASTEROIDS];
    int newCount = 0;

    for (int i = 0; i < oldCount; i++)
    {
        int idx = oldActive[i];
        Asteroid* a = &pool->asteroids[idx];

        if (a->destroyed)
        {
            int count = 0;

            switch (a->level)
            {
                case 1: count = 3; break;
                case 2: count = 2; break;
                case 3: count = 0; break;
            }

            for (int j = 0; j < count; j++)
            {
                Asteroid child = {0};
                resetAsteroid(&child);

                child.level = a->level + 1;
                child.position = a->position;

                spawnAsteroid(pool, child);
            }

            continue;
        }

        newActive[newCount++] = idx;
    }

    for (int i = 0; i < newCount; i++)
        pool->active[i] = newActive[i];

    pool->activeCount = newCount;
}

void initAsteroids(AsteroidPool* pool, int number) {
    for (int i = 0; i < number; i++) {
        Asteroid ast = {0};
        resetAsteroid(&ast);
        ast.level = 1;
        ast.destroyed = false;
        spawnAsteroid(pool, ast);
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

void resetAllAsteroids(AsteroidPool* pool) {
    for (int i = 0; i < pool->activeCount; i++) {
        resetAsteroid(&pool->asteroids[pool->active[i]]);
    }
}

void initAsteroidArray(AsteroidArray* arr, int capacity) {
    arr->data = malloc(sizeof(Asteroid) * capacity);
    arr->size = 0;
    arr->capacity = capacity;
}

void addAsteroidToArray(AsteroidArray* arr, Asteroid ast) {
    if (arr->size >= arr->capacity)
    {
        arr->capacity *= 2;
        Asteroid* newData = realloc(arr->data, sizeof(Asteroid) * arr->capacity);
        if (newData != NULL)
        {
            arr->data = newData;
        }
    }
    arr->data[arr->size++] = ast;
}

void removeAsteroidFromArray(AsteroidArray* arr, int index) {
    if (index < 0 || index >= arr->size) return;

    arr->data[index] = arr->data[arr->size - 1];
    arr->size--;
}

void freeAsteroidArray(AsteroidArray* arr) {
    free(arr->data);
    arr->data = NULL;
    arr->size = 0;
    arr->capacity = 0;
}

void handleDestroyedAsteroids(AsteroidArray* arr) {
    for (int i = arr->size; i >= 0; i--) {
        Asteroid* ast = &arr->data[i];

        if (!(ast->destroyed)) continue;

        int numberOfNew = 0;

        switch (ast->level)
        {
        case 1:
            numberOfNew = 3;
            break;
        case 2:
            numberOfNew = 2;
            break;
        case 3:
            numberOfNew = 0;
            break;
        
        default:
            printf("Invalid asteroid level %d\n", ast->level);
            break;
        }

        for (int j = 0; j < numberOfNew; j++) {
            Asteroid newAsteroid = {0};
            resetAsteroid(&newAsteroid);
            newAsteroid.level = ast->level + 1;
            newAsteroid.destroyed = false;
            newAsteroid.position = ast->position;

            addAsteroidToArray(arr, newAsteroid);
        }

        removeAsteroidFromArray(arr, i);
    }
}