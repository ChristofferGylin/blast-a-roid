#include "bonuses.h"
#include "enemies.h"
#include "raylib.h"
#include "player.h"
#include "constants.h"
#include "shooting.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "gameContext.h"
#include "outOfBoundsCheck.h"

const int MIN_BONUS_SPAWN_TIME = 5;
const int MAX_BONUS_SPAWN_TIME = 30;
const int BONUS_LIFE_TIME = 30;
const float BONUS_MULTIPLIER_ROLL_RATE = 2.0f;

void addNewBonus(GameContext* ctx, Bonus bonus);
void initBonus(GameContext* ctx, Bonus* bonus, BonusType type, Vector2 position, int value);

void addNewBonus(GameContext* ctx, Bonus bonus) {
    BonusObjectPool* pool = &ctx->objectPools.bonuses;
    
    pool->bonuses[pool->activeCount].bonus = bonus;
    pool->bonuses[pool->activeCount].active = true;
    pool->activeCount++;
}

void dropNewBonus(GameContext* ctx, Enemy* enemy) {
    // TODO: Logic for random selecting bonus

    float rotationVelocity = GetRandomValue(-50, 50);
    float shieldValue = 0.0f;

    if (GetRandomValue(0, 100) < 70) {
        shieldValue = 0.5f;
    } else {
        shieldValue = 1.0f;
    }

    AnimationInstance animationInstance;

    initAnimtionInstance(&animationInstance, &ctx->assets.animations.crate, enemy->position, 0.0f);

    Bonus newBonus;

    initBonus(ctx, &newBonus, SHIELD_REFILL, enemy->position, shieldValue);
    addNewBonus(ctx, newBonus);
}

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
        ShotPoolObject* shotObj = &ctx->objectPools.shots.shots[i];

        if (shotObj->shot.owner != PLAYER_SHOT) continue;
        
        if (bonuses->bonusMultiplier.base.isActive && CheckCollisionCircles(shotObj->shot.position, shotObj->shot.size / 2.0f, bonuses->bonusMultiplier.base.position, BONUS_MULTIPLIER_RADIUS)) {
            ctx->player.powerups.levelBonusMultiplier = round(bonuses->bonusMultiplier.level);
            bonuses->bonusMultiplier.base.isActive = false;
            destroyShot(&ctx->objectPools.shots.shots[i]);
            PlaySound(ctx->assets.samples.multiplier_collect);
        }
    }
}

void initBonus(GameContext* ctx, Bonus* bonus, BonusType type, Vector2 position, int value) {
    bonus->position = position;
    bonus->rotation = 0;
    bonus->rotationVelocity = GetRandomValue(-50, 50),
    bonus->type = type;
    bonus->value;
    bonus->spawnTime = GetTime();
    
    switch (type)
    {
    case SHIELD_REFILL:
        AnimationInstance animationInstance;
        initAnimtionInstance(&animationInstance, &ctx->assets.animations.crate, position, 0.0f);
        
        bonus->animation = animationInstance;
        bonus->size = ctx->assets.animations.crate.size;
        bonus->velocity = getRandomVelocity((FloatRange){30.0f, 60.0f});
        bonus->visualType = VISUAL_ANIMATION;
        break;
    
    default:
        printf("Error: Invalid BonusType in initBonus\n");
        break;
    }

}

void initBonuses(Bonuses* bonuses) {
    bonuses->nextSpawnTime = getNextSpawnTime();

    bonuses->bonusMultiplier.base.isActive = false;
    bonuses->bonusMultiplier.base.position = (Vector2){0, 0};
    bonuses->bonusMultiplier.base.spawnTime = 0.0;
    bonuses->bonusMultiplier.level = 2;
}

void initBonusPool(BonusObjectPool* pool) {
    for (int i = 0; i < MAX_BONUSES; i++) {
        pool->bonuses[i].active = false;
    }
    pool->activeCount = 0;
}

void renderBonuses(Bonuses* bonuses, BonusObjectPool* pool) {
    if (bonuses->bonusMultiplier.base.isActive) {
        renderBonusMultiplier(bonuses->bonusMultiplier.level, bonuses->bonusMultiplier.base.position);
    }

    for (int i = 0; i < pool->activeCount; i++) {
        
        Bonus* bonus = &pool->bonuses[i].bonus;
        
        if (bonus->visualType == VISUAL_ANIMATION) {
            renderAnimation(&bonus->animation);
        } else {
            DrawTexturePro(
                *bonus->sprite,
                (Rectangle){0, 0, bonus->sprite->width, bonus->sprite->height},
                (Rectangle){bonus->position.x, bonus->position.y, bonus->sprite->width, bonus->sprite->height},
                (Vector2){ bonus->sprite->width / 2.0f, bonus->sprite->height / 2.0f},
                bonus->rotation,
                WHITE
            );
        }
    }
}

void renderBonusMultiplier(int level, Vector2 position) {
    BonusMultiplierIcon icon = getBonusMultiplierIcon(level);

    Vector2 textSize = MeasureTextEx(GetFontDefault(), icon.text, 12, 2);
    Vector2 textPos = {position.x - (textSize.x / 2), position.y - (textSize.y / 2)};

    DrawCircleV(position, BONUS_MULTIPLIER_RADIUS, icon.color);
    DrawTextPro(GetFontDefault(), icon.text, textPos, (Vector2){0, 0}, 0, 12, 2, RAYWHITE);
}

void updateBonuses(BonusObjectPool* pool) {

    const float lifeTime = 30;
    const double deactivateTime = GetTime() + lifeTime;

    bool poolHasChanged = false;

    for (int i = 0; i < pool->activeCount; i++) {
        
        Bonus* bonus = &pool->bonuses[i].bonus;
        
        if (bonus->spawnTime + lifeTime <= GetTime()) {
            pool->bonuses[i].active = false;
            poolHasChanged = true;
        } else {
            updateRotation(&bonus->rotation, bonus->rotationVelocity);
            updatePosition(&bonus->position, bonus->velocity);

            outOfBoundsCheck(&bonus->position, bonus->size.x);
        }
    }
}