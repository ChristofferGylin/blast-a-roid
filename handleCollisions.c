#include "raylib.h"
#include "ship.h"
#include "asteroid.h"
#include "constants.h"
#include <stdio.h>

void handleCollisions(AsteroidArray* astArr, Ship* ship) {

    if (ship->destroyed) return; 

    for (int i = 0; i < astArr->size; i++) {
        Asteroid* ast = &astArr->data[i];
        float asteroidRadius;

        switch (ast->level)
        {
        case 1:
            asteroidRadius = ASTEROID_SIZE_1 / 2.0f;
            break;
        case 2:
            asteroidRadius = ASTEROID_SIZE_2 / 2.0f;
            break;
        case 3:
            asteroidRadius = ASTEROID_SIZE_3 / 2.0f;
            break;
        
        default:
            break;
        }

        if (CheckCollisionCircles(ship->position, SHIP_SIZE / 2.0f, ast->position, asteroidRadius)) {
            ship->destroyed = true;
            ast->destroyed = true;
        } 
    }
}