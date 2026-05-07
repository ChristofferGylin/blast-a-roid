#ifndef BONUSES_H
#define BONUSES_H

#include "animation.h"
#include <stdbool.h>
#include "raylib.h"
#include "shooting.h"
#include "constants.h"
#include "utils.h"

#define MAX_BONUSES 256

typedef struct Enemy Enemy;
typedef struct Player Player;

typedef enum BonusType {
    BONUS_POINTS,
    SHIELD_REFILL,
    FULL_AUTO_POWERUP,
    MULTI_SHOT_POWERUP,
    AUTO_STOP_POWERUP,
    
}BonusType;

typedef struct Bonus {
    BonusType type;
    double spawnTime;
    Vector2 position;
    Vector2 velocity;
    float rotation;
    float rotationVelocity;
    int value;
    VisualType visualType;
    union {
        Texture2D* sprite;
        AnimationInstance animation;
    };
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

typedef struct BonusPoolObject {
    bool active;
    Bonus bonus;
}BonusPoolObject;

typedef struct BonusObjectPool {
    BonusPoolObject bonuses[MAX_BONUSES];
    int activeCount;
}BonusObjectPool;

void dropNewBonus(GameContext* ctx, Enemy* enemy);
BonusMultiplierIcon getBonusMultiplierIcon(float level);
double getNextSpawnTime();
Vector2 getRandomPosition();
void handleBonuses(GameContext* ctx, Bonuses* bonuses);
void handleBonusesCollisions(GameContext* ctx, Bonuses* bonuses);
void initBonuses(Bonuses* bonuses);
void initBonusPool(BonusObjectPool* pool);
void renderBonuses(Bonuses* bonuses, BonusObjectPool* pool);
void renderBonusMultiplier(int level, Vector2 position);

#endif