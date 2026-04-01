#include "raylib.h"

#ifndef SHIP_H
#define SHIP_H

typedef struct {
    Texture2D sprite;
    Vector2 position;
    Vector2 velocity;
    float rotation;
}Ship; 

#endif