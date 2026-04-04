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
    AsteroidPoolObject asteroids[MAX_ASTEROIDS];
    int activeCount;
}AsteroidPool;

void addNewAsteroid(AsteroidPool* pool, Asteroid ast);
void handleAsteroidCollisions(Ship* ship);
void handleAsteroidsMovement();
void handleDestroyedAsteroids();
void initAsteroids(int gameLevel);
void renderAsteroids(Texture2D* asteroidSprite);
void resetAllAsteroids();
void resetAsteroid(Asteroid* ast);

#endif