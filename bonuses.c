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
void compactBonusPool(BonusObjectPool* pool);
void compactBonusSpawnPool(BonusSpawnPool* pool);
void initBonus(GameContext* ctx, Bonus* bonus, BonusType type, Vector2 position);

void addNewBonus(GameContext* ctx, Bonus bonus) {
    BonusObjectPool* pool = &ctx->objectPools.bonuses;

    if (pool->activeCount >= MAX_BONUSES) return;
    
    pool->bonuses[pool->activeCount].bonus = bonus;
    pool->bonuses[pool->activeCount].active = true;
    pool->activeCount++;
}

void compactBonusPool(BonusObjectPool* pool) {
    int write = 0;

    for (int i = 0; i < pool->activeCount; i++) {
        if (pool->bonuses[i].active) {
            if (write != i) {
                pool->bonuses[write] = pool->bonuses[i];
            }
            write++;
        }
    }

    for (int i = write; i < pool->activeCount; i++) {
        pool->bonuses[i].active = false;
    }

    pool->activeCount = write;
}

void compactBonusSpawnPool(BonusSpawnPool* pool) {
    int write = 0;

    for (int i = 0; i < pool->activeCount; i++) {
        if (pool->options[i].active) {
            if (write != i) {
                pool->options[write] = pool->options[i];
            }
            write++;
        }
    }

    for (int i = write; i < pool->activeCount; i++) {
        pool->options[i].active = false;
    }

    pool->activeCount = write;
}

void dropNewBonus(GameContext* ctx, Enemy* enemy) {
    
    BonusSpawnPool* pool = &ctx->objectPools.spawnableBonuses;
    
    int sumOfWeight = 0;

    for (int i = 0; i < pool->activeCount; i++) {
        
        BonusSpawnOption* option = &pool->options[i].option;

        if (option->count < option->maxCount) {
            sumOfWeight += pool->options[i].option.weight;
        }
    }

    if (sumOfWeight == 0) return;
    
    int randomSelect = GetRandomValue(0, sumOfWeight - 1);    
        
    for (int i = 0; i < pool->activeCount; i++) {

        BonusSpawnOption* option = &pool->options[i].option;

        if (option->count >= option->maxCount) continue;

        if (randomSelect < option->weight) {
            Bonus newBonus;

            initBonus(ctx, &newBonus, SHIELD_REFILL, enemy->position);
            addNewBonus(ctx, newBonus);
            option->count++;

            return;    
        }

        randomSelect -= option->weight;
    }
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

    bool objectPoolHasChanged = false;
    bool spawnPoolHasChanged = false;
    BonusSpawnPool* spawnPool = &ctx->objectPools.spawnableBonuses;

    for (int i = 0; i < ctx->objectPools.bonuses.activeCount; i++) {
        Bonus* bonus = &ctx->objectPools.bonuses.bonuses[i].bonus;

        if (CheckCollisionCircles(ctx->ship.position, SHIP_SIZE, bonus->position, bonus->size.x)) {
            
            switch (bonus->type)
            {
            case SHIELD_REFILL:
                ctx->player.shieldPower += bonus->value;
                if (ctx->player.shieldPower > 1.0f) ctx->player.shieldPower = 1.0f;

                PlaySound(ctx->assets.samples.shieldUp);
                break;

            case BONUS_POINTS:
                ctx->player.score += bonus->value;
                // TODO: Play sound
                break;

            case FULL_AUTO_POWERUP:
                ctx->player.powerups.fullAuto = true;
                // TODO: Play sound
                break;
            
            case MULTI_SHOT_POWERUP:
                ctx->player.powerups.trippleShot = true;
                // TODO: Play sound
                break;
            
            case AUTO_STOP_POWERUP:
                ctx->player.powerups.autoStop = true;
                // TODO: Play sound
                break;
            
            case LOCK_POWERUP:
                ctx->player.powerups.lock = true;
                // TODO: Play sound
                break;
            
            case LONG_SHOT_POWERUP:
                ctx->player.powerups.longShot = true;
                // TODO: Play sound
                break;
            
            default:
                printf("Error: Invalid BonusType in handleBonusesCollisions\n");
                break;
            }

            ctx->objectPools.bonuses.bonuses[i].active = false;
            objectPoolHasChanged = true;

            for (int j = 0; j < spawnPool->activeCount; j++) {
                if (bonus->type == spawnPool->options[j].option.type) {
                    if (bonus->type == SHIELD_REFILL || bonus->type == BONUS_POINTS) {
                        spawnPool->options[j].option.count--;
                    } else {
                        spawnPool->options[j].active = false;
                        spawnPoolHasChanged = true;
                    }
                    break;
                }
            }
        }
    }

    if (objectPoolHasChanged) {
        compactBonusPool(&ctx->objectPools.bonuses);
    }

    if (spawnPoolHasChanged) {
        compactBonusSpawnPool(spawnPool);
    }
}

void initBonus(GameContext* ctx, Bonus* bonus, BonusType type, Vector2 position) {    
    bonus->position = position;
    bonus->rotation = 0;
    bonus->rotationSpeed = GetRandomValue(-100, 100),
    bonus->type = type;
    bonus->spawnTime = GetTime();
    bonus->velocity = getRandomVelocity((FloatRange){30.0f, 60.0f});

    if (type == BONUS_POINTS) {
        bonus->sprite = &ctx->assets.sprites.blueGem;
        bonus->size = (Vector2){GEM_COLLISION_SIZE, GEM_COLLISION_SIZE};
        bonus->visualType = VISUAL_SPRITE;
    } else {
        AnimationInstance animationInstance;
        initAnimtionInstance(&animationInstance, &ctx->assets.animations.crate, position, 0.0f);
        
        bonus->animation = animationInstance;
        bonus->size = (Vector2){CRATE_COLLISION_SIZE, CRATE_COLLISION_SIZE};
        bonus->visualType = VISUAL_ANIMATION;
    }

    if (type == SHIELD_REFILL) {
        float shieldValue = 0.0f;

        if (GetRandomValue(0, 100) < 70) {
            shieldValue = 0.5f;
        } else {
            shieldValue = 1.0f;
        }

        bonus->value = shieldValue;
    } else if (type == BONUS_POINTS) {
        int rnd = GetRandomValue(0, 100);

        if (rnd < 50) {
            bonus->value = 500.0f;
        } else  if (rnd < 75) {
            bonus->value = 1000.0f;
        } else if (rnd < 95) {
            bonus->value = 2000.0f;
        } else {
            bonus->value = 5000.0f;
        }

    } else {
        bonus->value = 0.0f;
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

void initBonusSpawnPool(GameContext* ctx) {

    BonusSpawnPool* pool = &ctx->objectPools.spawnableBonuses;
    Powerups* playerPowerups = &ctx->player.powerups;

    for (int i = 0; i < NUMBER_OF_BONUS_TYPES; i++) {
        pool->options[i].active = false;
    }

    pool->activeCount = 0;

    pool->options[pool->activeCount].option = (BonusSpawnOption){
        BONUS_POINTS,
        100.0f,
        0,
        MAX_BONUSES
    };

    pool->activeCount++;

    pool->options[pool->activeCount].option = (BonusSpawnOption){
        SHIELD_REFILL,
        75.0f,
        0,
        MAX_BONUSES
    };

    pool->activeCount++;

    if (!playerPowerups->autoStop) {
        pool->options[pool->activeCount].option = (BonusSpawnOption){
            AUTO_STOP_POWERUP,
            30.0f,
            0,
            1
        };

        pool->activeCount++;
    }

    if (!playerPowerups->fullAuto) {
        pool->options[pool->activeCount].option = (BonusSpawnOption){
            FULL_AUTO_POWERUP,
            50.0f,
            0,
            1
        };

        pool->activeCount++;
    }

    if (!playerPowerups->lock) {
        pool->options[pool->activeCount].option = (BonusSpawnOption){
            LOCK_POWERUP,
            20.0f,
            0,
            1
        };

        pool->activeCount++;
    }

    if (!playerPowerups->longShot) {
        pool->options[pool->activeCount].option = (BonusSpawnOption){
            LONG_SHOT_POWERUP,
            50.0f,
            0,
            1
        };

        pool->activeCount++;
    }

    if (!playerPowerups->trippleShot) {
        pool->options[pool->activeCount].option = (BonusSpawnOption){
            MULTI_SHOT_POWERUP,
            50.0f,
            0,
            1
        };

        pool->activeCount++;
    }

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

void resetPowerups(Player* player) {
    Powerups* powerups = &player->powerups;

    player->shieldPower = 0.5f;

    if (powerups->lock) {
        powerups->lock = false;
    } else {
        powerups->autoStop = false;
        powerups->fullAuto = false;
        powerups->longShot = false;
        powerups->trippleShot = false;
    }
}

void updateBonuses(BonusObjectPool* pool) {

    if (pool->activeCount == 0) return;

    const float lifeTime = 30;
    const double deactivateTime = GetTime() + lifeTime;

    bool poolHasChanged = false;

    for (int i = 0; i < pool->activeCount; i++) {
        
        Bonus* bonus = &pool->bonuses[i].bonus;
        
        if (bonus->spawnTime + lifeTime <= GetTime()) {
            pool->bonuses[i].active = false;
            poolHasChanged = true;
        } else {
            updateRotation(&bonus->rotation, bonus->rotationSpeed);
            updatePosition(&bonus->position, bonus->velocity);

            outOfBoundsCheck(&bonus->position, bonus->size.x);

            if (bonus->visualType == VISUAL_ANIMATION) {
                bonus->animation.position = bonus->position;
                bonus->animation.rotation = bonus->rotation;
                updateAnimation(&bonus->animation);
            }
        }
    }

    if (poolHasChanged) {
        compactBonusPool(pool);
    }
}