#ifndef SCORE_H
#define SCORE_H

#include "asteroid.h"
#include "player.h"


void addScore(Player* player, Asteroid* ast);

void resetTimeBonusMultiplier(Player* player);
void updateLevelBonus(Player* player);
void updateTimeBonusMultiplier(Player* player);

#endif
