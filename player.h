#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include <stdint.h>
#include "asteroid.h"

typedef struct {
    bool fullAuto;
    bool longShot;
    bool trippleShot;
    bool lock;
    int levelBonusMultiplier;
}Powerups;

typedef struct Player {
    int lives;
    int level;
    uint64_t score;
    uint64_t levelBonus;
    double levelBonusTimer;
    int timeBonusMultiplier;
    double timeBonusTimer;
    int bombs;
    float shieldPower;
    Powerups powerups;
}Player;

void initPlayer(Player* player);

#endif