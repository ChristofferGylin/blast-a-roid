#ifndef ASTEROID_H
#define ASTEROID_H
#define MAX_ASTEROIDS 128

#include "raylib.h"
#include "ship.h"
#include <stdbool.h>

typedef struct {
    int level;
    float rotation;
    float rotationSpeed;
    Vector2 velocity;
    Vector2 position;
    bool destroyed;
}Asteroid;

typedef struct {
    bool active;
    Asteroid asteroid; 
}AsteroidPoolObject;

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

void initAsteroids(int gameLevel);
void initAsteroidPool(AsteroidPool* pool);
void addAsteroidToArray(AsteroidArray* arr, Asteroid ast);
void removeAsteroidFromArray(AsteroidArray* arr, int index);
void freeAsteroidArray(AsteroidArray* arr);
void handleAsteroidsMovement();
void handleCollisions(AsteroidPool* astPool, Ship* ship);
void resetAsteroid(Asteroid* ast);
void resetAllAsteroids();
void handleDestroyedAsteroids();
void spawnAsteroid(AsteroidPool* pool, Asteroid ast);
void destroyAsteroid(AsteroidPool* pool, int index);
void updateAsteroidsPool(AsteroidPool* pool);

#endif