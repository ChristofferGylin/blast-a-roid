#ifndef ASTEROID_H
#define ASTEROID_H
#define MAX_ASTEROIDS 128

#include "raylib.h"
#include "ship.h"
#include "shooting.h"
#include <stdbool.h>
typedef struct Player Player;
typedef struct AnimationPool AnimationPool;
typedef struct Animation Animation;

typedef enum AsteroidType {
    ASTEROID_LEVEL_1,
    ASTEROID_LEVEL_2,
    ASTEROID_LEVEL_3,
    METAL_ASTEROID,
    SPIKY_ASTEROID
}AsteroidType;

typedef struct Asteroid {
    AsteroidType type;
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
int getAsteroidSize(AsteroidType type);
void handleAsteroidCollisions(GameContext* ctx);
void handleAsteroidsMovement(AsteroidPool* pool);
void handleDestroyedAsteroids(GameContext* ctx);
void initAsteroidPool(AsteroidPool* pool);
void initDestroyedAsteroidPool(DestroyedAsteroidPool* pool);
void initAsteroids(GameContext* ctx);
void renderAsteroids(GameContext* ctx);
void resetAllAsteroids(AsteroidPool* pool);
void resetAsteroid(Asteroid* ast);

#endif