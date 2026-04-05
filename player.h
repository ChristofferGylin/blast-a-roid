#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    bool fullAuto;
    bool longShot;
    bool trippleShot;
    bool lock;
    int levelBonusMultiplier;
}Powerups;

typedef struct {
    int lives;
    int level;
    uint64_t score;
    uint64_t levelBonus;
    int timeBonusMultiplier;
    int bombs;
    float shieldPower;
    Powerups powerups;
}Player;

#endif