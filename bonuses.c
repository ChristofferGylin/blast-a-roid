#include "bonuses.h"
#include "raylib.h"
#include "player.h"
#include "constants.h"
#include "shooting.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "gameContext.h"

const int MIN_BONUS_SPAWN_TIME = 5;
const int MAX_BONUS_SPAWN_TIME = 30;
const int BONUS_LIFE_TIME = 30;
const float BONUS_MULTIPLIER_ROLL_RATE = 2.0f;

BonusMultiplierIcon getBonusMultiplierIcon(float level) {
    int roundedLevel = round(level);
    BonusMultiplierIcon icon = {0};
    switch (roundedLevel) {
        case 2:
            strcpy(icon.text, "x2");
            icon.color = GREEN;
            break;

        case 3:
            strcpy(icon.text, "x3");
            icon.color = BLUE;
            break;

        case 4:
            strcpy(icon.text, "x4");
            icon.color = RED;
            break;

        case 5:
            strcpy(icon.text, "x5");
            icon.color = PURPLE;
            break;

        default:
            printf("Error: Invalid level (%d) in getBonusMultiplierIcon\n", roundedLevel);
            break;
    }
    return icon;
}

double getNextSpawnTime() {
    return GetTime() + GetRandomValue(MIN_BONUS_SPAWN_TIME, MAX_BONUS_SPAWN_TIME);
}

Vector2 getRandomPosition() {
    return (Vector2){GetRandomValue(SIDEBAR_WIDTH + 10, SCREEN_WIDTH - 10 - SIDEBAR_WIDTH), GetRandomValue(10, SCREEN_HEIGHT - 10)};
}

void handleBonuses(GameContext* ctx, Bonuses* bonuses) {
    
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

        if (randomSelect < 50) {
            return;
        } else if (randomSelect < 70) {
            if (bonuses->bonusMultiplier.base.isActive || ctx->player.powerups.levelBonusMultiplier > 1) return;

            bonuses->bonusMultiplier.base.isActive = true;
            bonuses->bonusMultiplier.base.spawnTime = now;
            bonuses->bonusMultiplier.base.position = getRandomPosition();
            bonuses->bonusMultiplier.level = 2.0f;
            PlaySound(ctx->assets.samples.multiplier_spawn);

        } else if (randomSelect < 85) {
            // spawn powerup or shield
        } else {
            // spawn extra life or extra ship
        }

    }
}

void handleBonusesCollisions(GameContext* ctx, Bonuses* bonuses) {
    for (int i = 0; i < ctx->objectPools.shots.activeCount; i++) {
        if (bonuses->bonusMultiplier.base.isActive && CheckCollisionCircles(ctx->objectPools.shots.shots[i].shot.position, SHOT_SIZE / 2.0f, bonuses->bonusMultiplier.base.position, BONUS_MULTIPLIER_RADIUS)) {
            ctx->player.powerups.levelBonusMultiplier = round(bonuses->bonusMultiplier.level);
            bonuses->bonusMultiplier.base.isActive = false;
            destroyShot(&ctx->objectPools.shots.shots[i]);
        }
    }
}

void initBonuses(Bonuses* bonuses) {
    bonuses->nextSpawnTime = getNextSpawnTime();

    bonuses->bonusMultiplier.base.isActive = false;
    bonuses->bonusMultiplier.base.position = (Vector2){0, 0};
    bonuses->bonusMultiplier.base.spawnTime = 0.0;
    bonuses->bonusMultiplier.level = 2;
}

void renderBonuses(Bonuses* bonuses) {
    if (bonuses->bonusMultiplier.base.isActive) {
        renderBonusMultiplier(bonuses->bonusMultiplier.level, bonuses->bonusMultiplier.base.position);
    }
}

void renderBonusMultiplier(int level, Vector2 position) {
    BonusMultiplierIcon icon = getBonusMultiplierIcon(level);

    Vector2 textSize = MeasureTextEx(GetFontDefault(), icon.text, 12, 2);
    Vector2 textPos = {position.x - (textSize.x / 2), position.y - (textSize.y / 2)};

    DrawCircleV(position, BONUS_MULTIPLIER_RADIUS, icon.color);
    DrawTextPro(GetFontDefault(), icon.text, textPos, (Vector2){0, 0}, 0, 12, 2, RAYWHITE);
}