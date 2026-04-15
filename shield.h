#ifndef SHIELD_H
#define SHIELD_H

#include "ship.h"
typedef struct Player Player;

void handleShield(Ship* ship, Player* player);
void renderShield(Ship* ship);

#endif