#ifndef SHIELD_H
#define SHIELD_H

#include "ship.h"
#include "raylib.h"
typedef struct Player Player;

void handleShield(Ship* ship, Player* player);
Color lerpColor(Color a, Color b, float t);
void renderShield(Ship* ship);
void updateShieldAnimation();

#endif