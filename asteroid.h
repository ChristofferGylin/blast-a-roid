#include "raylib.h"

#ifndef ASTEROID_H
#define ASTEROID_H

typedef struct 
{
    int level;
    float rotation;
    Vector2 velocity;
    Vector2 position;
}Asteroid;

#endif