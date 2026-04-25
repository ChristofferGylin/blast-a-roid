#include "raylib.h"

#ifndef SHIP_H
#define SHIP_H

typedef struct {
    Vector2 position;
    Vector2 velocity;
    float rotation;
    bool destroyed;
    bool isShieldActive;
}Ship; 

void handleShipControls(Ship *ship);
void resetShip(Ship* ship);

#endif