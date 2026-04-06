#ifndef ASTEROID_H
#define ASTEROID_H
#define MAX_ASTEROIDS 128

#include "raylib.h"
#include "ship.h"
#include "shooting.h"
#include <stdbool.h>
typedef struct Player Player;

typedef struct Asteroid {
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

typedef struct {
    AsteroidPoolObject* asteroids[MAX_ASTEROIDS];
    int activeCount;
}DestroyedAsteroidPool;

void addNewAsteroid(AsteroidPool* pool, Asteroid ast);
int getAsteroidSize(int level);
void handleAsteroidCollisions(AsteroidPool* pool, DestroyedAsteroidPool* destroyedPool, ShotObjectPool* shotPool, Ship* ship, Player* player);
void handleAsteroidsMovement(AsteroidPool* pool);
void handleDestroyedAsteroids(AsteroidPool* pool, DestroyedAsteroidPool* destroyedPool);
void initAsteroids(AsteroidPool* pool, int gameLevel);
void renderAsteroids(AsteroidPool* pool, Texture2D* asteroidSprite);
void resetAllAsteroids(AsteroidPool* pool);
void resetAsteroid(Asteroid* ast);

#endif