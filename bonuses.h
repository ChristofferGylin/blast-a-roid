#ifndef BONUSES_H
#define BONUSES_H

#include "animation.h"
#include <stdbool.h>
#include "raylib.h"
#include "shooting.h"
#include "constants.h"
#include "utils.h"

#define MAX_BONUSES 256
#define NUMBER_OF_BONUS_TYPES 7

typedef struct Enemy Enemy;
typedef struct Player Player;

typedef enum BonusType {
    AUTO_STOP_POWERUP,
    BONUS_POINTS,
    FULL_AUTO_POWERUP,
    LOCK_POWERUP,
    LONG_SHOT_POWERUP,
    MULTI_SHOT_POWERUP,
    SHIELD_REFILL
}BonusType;

typedef struct Bonus {
    BonusType type;
    double spawnTime;
    Vector2 position;
    Vector2 velocity;
    Vector2 size;
    float rotation;
    float rotationSpeed;
    float value;
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

typedef struct BonusSpawnOption {
    BonusType type;
    int weight;
}BonusSpawnOption;

typedef struct BonusSpawnPoolObject {
    bool active;
    BonusSpawnOption option;
}BonusSpawnPoolObject;

typedef struct BonusSpawnPool {
    BonusSpawnPoolObject options[NUMBER_OF_BONUS_TYPES];
    int activeCount;
} BonusSpawnPool;

void dropNewBonus(GameContext* ctx, Enemy* enemy);
BonusMultiplierIcon getBonusMultiplierIcon(float level);
double getNextSpawnTime();
void handleBonuses(GameContext* ctx, Bonuses* bonuses);
void handleBonusesCollisions(GameContext* ctx, Bonuses* bonuses);
void initBonuses(Bonuses* bonuses);
void initBonusPool(BonusObjectPool* pool);
void initBonusSpawnPool(GameContext* ctx);
void renderBonuses(Bonuses* bonuses, BonusObjectPool* pool);
void renderBonusMultiplier(int level, Vector2 position);
void resetPowerups(Player* player);
void updateBonuses(GameContext* ctx);

#endif