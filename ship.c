#include "ship.h"
#include "raylib.h"

void resetShip(Ship* ship) {
    ship->destroyed = false;
    ship->position.x = GetScreenWidth() / 2;
    ship->position.y = GetScreenHeight() / 2;
    ship->rotation = 0;
    ship->velocity.x = 0;
    ship->velocity.y = 0;
}