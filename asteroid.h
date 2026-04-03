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

void initAsteroids(AsteroidArray* arr, int number);
void initAsteroidArray(AsteroidArray* arr, int capacity);
void addAsteroidToArray(AsteroidArray* arr, Asteroid ast);
void removeAsteroidFromArray(AsteroidArray* arr, int index);
void freeAsteroidArray(AsteroidArray* arr);
void handleAsteroidsMovement(AsteroidArray* arr);
void handleCollisions(AsteroidArray* astArr, Ship* ship);
void resetAsteroid(Asteroid* ast);
void resetAllAsteroids(AsteroidArray* arr);
void handleDestroyedAsteroids(AsteroidArray* arr);

#endif