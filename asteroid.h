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

void initAsteroids(int gameLevel);
void handleAsteroidsMovement();
void resetAsteroid(Asteroid* ast);
void resetAllAsteroids();
void handleDestroyedAsteroids();
void handleAsteroidCollisions(Ship* ship);
void renderAsteroids(Texture2D* asteroidSprite);

#endif