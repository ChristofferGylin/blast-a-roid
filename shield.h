#ifndef SHIELD_H
#define SHIELD_H

typedef struct Player Player;
typedef struct Ship Ship;

void handleShield(Ship* ship, Player* player);
void renderShield(Ship* ship);

#endif