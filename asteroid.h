#include "raylib.h"
#include "ship.h"
#include <stdbool.h>

#ifndef ASTEROID_H
#define ASTEROID_H

typedef struct {
    int level;
    float rotation;
    float rotationSpeed;
    Vector2 velocity;
    Vector2 position;
    bool destroyed;
}Asteroid;

typedef struct {
    Asteroid* data;
    int size;
    int capacity;
}AsteroidArray;

typedef struct {
    Asteroid asteroids[MAX_ASTEROIDS];
    int active[MAX_ASTEROIDS];
    int activeCount;
}AsteroidPool;

void initAsteroids(AsteroidPool* pool, int number);
void initAsteroidPool(AsteroidPool* pool);
void addAsteroidToArray(AsteroidArray* arr, Asteroid ast);
void removeAsteroidFromArray(AsteroidArray* arr, int index);
void freeAsteroidArray(AsteroidArray* arr);
void handleAsteroidsMovement(AsteroidArray* arr);
void handleCollisions(AsteroidArray* astArr, Ship* ship);
void resetAsteroid(Asteroid* ast);
void resetAllAsteroids(AsteroidPool* pool);
void handleDestroyedAsteroids(AsteroidArray* arr);
void spawnAsteroid(AsteroidPool* pool, Asteroid ast);
void destroyAsteroid(AsteroidPool* pool, int index);
void updateAsteroidsPool(AsteroidPool* pool);

#endif