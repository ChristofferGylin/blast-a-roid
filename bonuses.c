#include "bonuses.h"
#include "raylib.h"
#include "player.h"
#include "constants.h"
#include "shooting.h"
#include <math.h>

const int MIN_BONUS_SPAWN_TIME = 10;
const int MAX_BONUS_SPAWN_TIME = 30;
const int BONUS_LIFE_TIME = 30;
const float BONUS_MULTIPLIER_ROLL_RATE = 0.5f;
const int BONUS_MULTIPLIER_RADIUS = 6;

double getNextSpawnTime() {
    return GetTime() + GetRandomValue(MIN_BONUS_SPAWN_TIME, MAX_BONUS_SPAWN_TIME);
}

Vector2 getRandomPosition() {
    return (Vector2){GetRandomValue(10, SCREEN_WIDTH - 10), GetRandomValue(10, SCREEN_HEIGHT - 10)};
}

void handleBonuses(Bonuses* bonuses, Player* player) {
    
    double now = GetTime();

    if (bonuses->bonusMultiplier.base.isActive) {
            
        if (bonuses->bonusMultiplier.base.spawnTime + BONUS_LIFE_TIME < now) {
            bonuses->bonusMultiplier.base.isActive = false;
        } else {
            bonuses->bonusMultiplier.level += GetFrameTime() * BONUS_MULTIPLIER_ROLL_RATE;
            if (bonuses->bonusMultiplier.level > 5.4f) {
                bonuses->bonusMultiplier.level = 2.0f;
            }
        }
    }

    if (bonuses->nextSpawnTime < now) {

        bonuses->nextSpawnTime = getNextSpawnTime();
        int randomSelect = GetRandomValue(1, 100);

        if (randomSelect < 70) {
            return;
        } else if (randomSelect < 85) {
            if (bonuses->bonusMultiplier.base.isActive || player->powerups.levelBonusMultiplier > 2) return;

            bonuses->bonusMultiplier.base.isActive = true;
            bonuses->bonusMultiplier.base.spawnTime = now;
            bonuses->bonusMultiplier.base.position = getRandomPosition();
            bonuses->bonusMultiplier.level = 2.0f;

        } else if (randomSelect < 95) {
            // spawn powerup or shield
        } else {
            // spawn extra life or extra shield
        }

    }
}

void handleBonusesCollisions(ShotObjectPool* shotPool, Bonuses* bonuses, Player* player) {
    for (int i = 0; i < shotPool->activeCount; i++) {
        if (CheckCollisionCircles(shotPool->shots[i].shot.position, SHOT_SIZE / 2.0f, bonuses->bonusMultiplier.base.position, BONUS_MULTIPLIER_RADIUS)) {
            player->powerups.levelBonusMultiplier = round(bonuses->bonusMultiplier.level);
            bonuses->bonusMultiplier.base.isActive = false;
            destroyShot(&shotPool[i]);
        }
    }
}