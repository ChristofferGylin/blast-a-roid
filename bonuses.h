#ifndef BONUSES_H
#define BONUSES_H

#include <stdbool.h>
#include "raylib.h"
#include "shooting.h"

typedef struct Player Player;

typedef struct BonusBase {
    bool isActive;
    double spawnTime;
    Vector2 position;
}BonusBase;

typedef struct BonusMultiplier {
    BonusBase base;
    float level;
}BonusMultiplier;

typedef struct Bonuses {
    double nextSpawnTime;
    BonusMultiplier bonusMultiplier;
}Bonuses; 

double getNextSpawnTime();
Vector2 getRandomPosition();
void handleBonuses(Bonuses* bonuses, Player* player);
void handleBonusesCollisions(ShotObjectPool* shotPool, Bonuses* bonuses, Player* player);

#endif