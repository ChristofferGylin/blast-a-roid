#ifndef BONUSES_H
#define BONUSES_H

#include <stdbool.h>
#include "raylib.h"
#include "shooting.h"

typedef struct Player Player;

typedef enum BonusType {
    BONUS_POINTS,
    SHIELD_REFILL,
    FULL_AUTO_POWERUP,
    MULTI_SHOT_POWERUP,
}BonusType;

typedef struct Bonus {
    BonusType type;
    double spawnTime;
    Vector2 position;
    Vector2 Velocity;
    float rotation;
    float rotationVelocity;
    int value;
}Bonus;


typedef struct BonusBase {
    bool isActive;
    double spawnTime;
    Vector2 position;
}BonusBase;

typedef struct BonusMultiplier {
    BonusBase base;
    float level;
}BonusMultiplier;

typedef struct BonusMultiplierIcon {
    char text[3];
    Color color;
}BonusMultiplierIcon;

typedef struct Bonuses {
    double nextSpawnTime;
    BonusMultiplier bonusMultiplier;
}Bonuses;

BonusMultiplierIcon getBonusMultiplierIcon(float level);
double getNextSpawnTime();
Vector2 getRandomPosition();
void handleBonuses(GameContext* ctx, Bonuses* bonuses);
void handleBonusesCollisions(GameContext* ctx, Bonuses* bonuses);
void initBonuses(Bonuses* bonuses);
void renderBonuses(Bonuses* bonuses);
void renderBonusMultiplier(int level, Vector2 position);

#endif