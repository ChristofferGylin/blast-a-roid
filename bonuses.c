#include "bonuses.h"
#include "colors.h"
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
#include "utils.h"

const int MIN_BONUS_SPAWN_TIME = 5;
const int MAX_BONUS_SPAWN_TIME = 30;
const int BONUS_LIFE_TIME = 30;

void addNewBonus(GameContext* ctx, Bonus bonus);
void addNewBonusSpawnOption(BonusSpawnPool* pool, BonusType type);
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

void addNewBonusSpawnOption(BonusSpawnPool* pool, BonusType type) {
    
    BonusSpawnOption option;
    option.type = type;

    switch (type) {

        case AUTO_STOP_POWERUP:
            option.weight = 30;
            break;
    
        case BONUS_POINTS:
            option.weight = 150;
            break;
    
        case FULL_AUTO_POWERUP:
            option.weight = 50;
            break;
    
        case LOCK_POWERUP:
            option.weight = 20;
            break;
    
        case LONG_SHOT_POWERUP:
            option.weight = 50;
            break;
    
        case MULTI_SHOT_POWERUP:
            option.weight = 50;
            break;
    
        case SHIELD_REFILL:
            option.weight = 75;
            break;
    
        default:
            printf("Error: Invalid BonusType in addNewBonusSpawnOption\n");
            break;
    }
    
    pool->options[pool->activeCount].active = true;
    pool->options[pool->activeCount].option = option;

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
        
        if (!pool->options[i].active) continue;
        sumOfWeight += pool->options[i].option.weight;
    }

    if (sumOfWeight == 0) return;
    
    int randomSelect = GetRandomValue(0, sumOfWeight - 1);    
        
    for (int i = 0; i < pool->activeCount; i++) {

        if (!pool->options[i].active) continue;

        BonusSpawnOption* option = &pool->options[i].option;

        if (randomSelect < option->weight) {

            Bonus newBonus;

            initBonus(ctx, &newBonus, option->type, enemy->position);
            addNewBonus(ctx, newBonus);

            if (option->type != BONUS_POINTS && option->type != SHIELD_REFILL) {
                pool->options[i].active = false;
                compactBonusSpawnPool(pool);
            }

            return;    
        }

        randomSelect -= option->weight;
    }
}

void handleBonusesCollisions(GameContext* ctx) {
    
    bool objectPoolHasChanged = false;
    BonusSpawnPool* spawnPool = &ctx->objectPools.spawnableBonuses;

    for (int i = 0; i < ctx->objectPools.bonuses.activeCount; i++) {
        if (!ctx->objectPools.bonuses.bonuses[i].active) continue;

        Bonus* bonus = &ctx->objectPools.bonuses.bonuses[i].bonus;

        if (CheckCollisionCircles(ctx->ship.position, SHIP_SIZE / 2.0f, bonus->position, bonus->size.x / 2.0f)) {
            
            switch (bonus->type)
            {
            case SHIELD_REFILL:
                ctx->player.shieldPower += bonus->value;
                if (ctx->player.shieldPower > 1.0f) ctx->player.shieldPower = 1.0f;

                PlaySound(ctx->assets.samples.shieldUp);
                break;

            case BONUS_POINTS:
                ctx->player.score += bonus->value;
                // TODO: Play unique sound
                playSoundPositioned(ctx->assets.samples.multiplier_collect, bonus->position.x);
                break;

            case FULL_AUTO_POWERUP:
                ctx->player.powerups.fullAuto = true;
                // TODO: Play unique sound
                playSoundPositioned(ctx->assets.samples.multiplier_collect, bonus->position.x);
                break;
            
            case MULTI_SHOT_POWERUP:
                ctx->player.powerups.trippleShot = true;
                // TODO: Play unique sound
                playSoundPositioned(ctx->assets.samples.multiplier_collect, bonus->position.x);
                break;
            
            case AUTO_STOP_POWERUP:
                ctx->player.powerups.autoStop = true;
                // TODO: Play unique sound
                playSoundPositioned(ctx->assets.samples.multiplier_collect, bonus->position.x);
                break;
            
            case LOCK_POWERUP:
                ctx->player.powerups.lock = true;
                // TODO: Play unique sound
                playSoundPositioned(ctx->assets.samples.multiplier_collect, bonus->position.x);
                break;
            
            case LONG_SHOT_POWERUP:
                ctx->player.powerups.longShot = true;
                // TODO: Play unique sound
                playSoundPositioned(ctx->assets.samples.multiplier_collect, bonus->position.x);
                break;
            
            default:
                printf("Error: Invalid BonusType in handleBonusesCollisions\n");
                break;
            }

            ctx->objectPools.bonuses.bonuses[i].active = false;
            objectPoolHasChanged = true;
        }
    }

    if (objectPoolHasChanged) {
        compactBonusPool(&ctx->objectPools.bonuses);
    }
}

void initBonus(GameContext* ctx, Bonus* bonus, BonusType type, Vector2 position) {    
    bonus->position = position;
    bonus->rotation = 0;
    bonus->rotationSpeed = GetRandomValue(-100, 100),
    bonus->type = type;
    bonus->spawnTime = GetTime() - ctx->pausTimer;
    bonus->velocity = getRandomVelocity((FloatRange){30.0f, 60.0f});

    if (type == BONUS_POINTS) {
        bonus->sprite = &ctx->assets.sprites.blueGem;
        bonus->size = (Vector2){GEM_COLLISION_SIZE, GEM_COLLISION_SIZE};
        bonus->visualType = VISUAL_SPRITE;
    } else {
        AnimationInstance animationInstance;
        initAnimtionInstance(&animationInstance, &ctx->assets.animations.crate, position, 0.0f, ctx->assets.animations.crate.fps, false);
        
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

    addNewBonusSpawnOption(pool, BONUS_POINTS);
    addNewBonusSpawnOption(pool, SHIELD_REFILL);

    if (!playerPowerups->autoStop) {
        addNewBonusSpawnOption(pool, AUTO_STOP_POWERUP);
    }

    if (!playerPowerups->fullAuto) {
        addNewBonusSpawnOption(pool, FULL_AUTO_POWERUP);
    }

    if (!playerPowerups->lock) {
        addNewBonusSpawnOption(pool, LOCK_POWERUP);
    }

    if (!playerPowerups->longShot) {
        addNewBonusSpawnOption(pool, LONG_SHOT_POWERUP);
    }

    if (!playerPowerups->trippleShot) {
        addNewBonusSpawnOption(pool, MULTI_SHOT_POWERUP);
    }
}

void renderBonuses(BonusObjectPool* pool) {
    for (int i = 0; i < pool->activeCount; i++) {

        if (!pool->bonuses[i].active) continue;
        
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

void updateBonuses(GameContext* ctx) {

    BonusObjectPool* pool = &ctx->objectPools.bonuses;
    BonusSpawnPool* spawnPool = &ctx->objectPools.spawnableBonuses;

    if (pool->activeCount == 0) return;

    const float lifeTime = 30;

    bool poolHasChanged = false;

    for (int i = 0; i < pool->activeCount; i++) {

        if (!pool->bonuses[i].active) continue;
        
        Bonus* bonus = &pool->bonuses[i].bonus;
        const double deactivateTime = bonus->spawnTime + lifeTime;
        const double gameTime = GetTime() - ctx->pausTimer;

        if (gameTime >= deactivateTime) {
            
            if (bonus->type != BONUS_POINTS && bonus->type != SHIELD_REFILL) {
                addNewBonusSpawnOption(spawnPool, bonus->type);
            }
            
            pool->bonuses[i].active = false;
            poolHasChanged = true;
        } else {
            updateRotation(&bonus->rotation, bonus->rotationSpeed);

            Vector2 velocity = applySupernovaEffects(ctx, bonus->velocity);

            updatePosition(&bonus->position, velocity);

            handleOutOfBounds(&bonus->position, bonus->size.x);

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