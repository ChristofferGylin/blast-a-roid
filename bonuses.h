#ifndef BONUSES_H
#define BONUSES_H

#include <stdbool.h>
#include "raylib.h"

typedef struct BonusBase {
    bool isActive;
    float spawnTime;
    Vector2 position;
}BonusBase;

typedef struct BonusMultiplier {
    BonusBase base;
    int level;
}BonusMultiplier;

typedef struct Bonuses {
    float nextSpawnTime;
    BonusMultiplier bonusMultiplier;
}Bonuses; 

#endif