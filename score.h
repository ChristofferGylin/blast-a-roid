#ifndef SCORE_H
#define SCORE_H

#include <inttypes.h>

#include "asteroid.h"
#include "player.h"

typedef struct Highscore {
    uint64_t score;
    char name[32];
    int level;
}Highscore;


void addScore(Player* player, Asteroid* ast);

void resetTimeBonusMultiplier(GameContext* ctx);
void updateLevelBonus(Player* player);
void updateTimeBonusMultiplier(Player* player);

#endif
