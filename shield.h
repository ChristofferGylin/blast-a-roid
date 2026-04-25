#ifndef SHIELD_H
#define SHIELD_H

#include "ship.h"
#include "raylib.h"
typedef struct Player Player;

void handleShield(GameContext* ctx);
Color lerpColor(Color a, Color b, float t);
void renderShield(Ship* ship);
void updateShieldAnimation();

#endif