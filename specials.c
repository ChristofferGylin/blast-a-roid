#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "animation.h"
#include "gameContext.h"
#include "specials.h"
#include "utils.h"

void addSpecialToPool(GameContext* ctx, SpecialType type);
void addSpecialToSpawnPool(SpecialsSpawnPool* pool, SpecialType type);
void handleSpecialsMovement(SpecialsPool* pool);
void spawnSpecials(GameContext* ctx);

void addSpecialToPool(GameContext* ctx, SpecialType type) {
    
    SpecialsPool* pool = &ctx->objectPools.specials;
    
    Special newSpecial;

    newSpecial.position = (Vector2){0,0};
    newSpecial.rotation = 0;
    newSpecial.rotationSpeed = 0;
    newSpecial.size = (Vector2){32, 32};
    newSpecial.spawnTime = GetTime() - ctx->pausTimer;
    newSpecial.type = type;
    newSpecial.value = 0;
    newSpecial.velocity = (Vector2){0,0};

    switch (type) {

        case MULTIPLIER:
            // TODO: set attributes specific to type
            break;
    
        case COMET:
            // TODO: set attributes specific to type
            break;
    
        case EXTRA_LIFE:
            // TODO: set attributes specific to type
            break;
    
        case SUPERNOVA:
            // TODO: set attributes specific to type
            break;
    
        case BLACK_HOLE:
            // TODO: set attributes specific to type
            break;
    
        default:
            break;
    }

    bool success = false;

    for (int i = 0; i < NUMBER_OF_SPECIALS; i++) {
        if (pool->specials[i].active) continue;

        pool->specials[i].active = true;
        pool->specials[i].special = newSpecial;

        success = true;
        break;
    }

    if (!success) {
        printf("Error: Could not add new Special in addSpecialToPool");
    }
}

void addSpecialToSpawnPool(SpecialsSpawnPool* pool, SpecialType type) {
    SpecialSpawn newSpecial;

    const int minSpawnDelay = 5;
    const int maxSpawnDelay = 30;

    newSpecial.type = type;
    newSpecial.spawnTime = GetTime() + GetRandomValue(minSpawnDelay, maxSpawnDelay);

    bool success = false;

    for (int i = 0; i < NUMBER_OF_SPECIALS; i++) {
        if (pool->specials[i].active) continue;

        pool->specials[i].active = true;
        pool->specials[i].special = newSpecial;

        success = true;
        break;
    }

    if (!success) {
        printf("Error: Could not add new SpecialSpawn in addSpecialToSpawnPool");
    }
}

void handleSpecialsMovement(SpecialsPool* pool) {
    for (int i = 0; i < pool->activeCount; i++) {
        SpecialPoolObject* specialObj = &pool->specials[i];
        if (!specialObj->active) continue;
        updatePosition(&specialObj->special.position, specialObj->special.velocity);
    }
}

void initSpecialsPool(SpecialsPool* pool) {
    for (int i = 0; i < NUMBER_OF_SPECIALS; i++) {
        pool->specials[i].active = false;
    }

    pool->activeCount = 0;
}

void initSpecialsSpawnPool(SpecialsSpawnPool* pool) {
    for (int i = 0; i < NUMBER_OF_SPECIALS; i++) {
        pool->specials[i].active = false;
    }

    pool->activeCount = 0;
}

void populateSpecialsSpawnPool(GameContext* ctx) {
    
    SpecialsSpawnOptionPool optionPool;
    SpecialsSpawnPool* spawnPool = &ctx->objectPools.specialsSpawn;

    for (int i = 0; i < NUMBER_OF_SPECIALS; i++) {
        optionPool.options[i].active = true;
    }

    optionPool.activeCount = NUMBER_OF_SPECIALS;

    optionPool.options[0].option.type = MULTIPLIER;
    optionPool.options[0].option.weight = 100;

    optionPool.options[1].option.type = COMET;
    optionPool.options[1].option.weight = 100;

    optionPool.options[2].option.type = EXTRA_LIFE;
    optionPool.options[2].option.weight = 30;
    
    optionPool.options[3].option.type = BLACK_HOLE;
    optionPool.options[3].option.weight = 20;

    optionPool.options[4].option.type = SUPERNOVA;
    optionPool.options[4].option.weight = 10;
    
    int minNumberOfSpecials = 0;
    int maxNumberOfSpecials = ceil((ctx->player.level / 2));

    if (maxNumberOfSpecials > NUMBER_OF_SPECIALS) maxNumberOfSpecials = NUMBER_OF_SPECIALS;

    int numberToPopulate = GetRandomValue(minNumberOfSpecials, maxNumberOfSpecials);

    for (int i = 0; i < numberToPopulate; i++) {
        
        int sumOfWeight = 0;

        for (int j = 0; j < NUMBER_OF_SPECIALS; j++) {
        
            if (!optionPool.options[j].active) continue;
            sumOfWeight += optionPool.options[j].option.weight;
        }

        int randomSelect = GetRandomValue(0, sumOfWeight - 1);

        for (int j = 0; j < NUMBER_OF_SPECIALS; j++) {

            if (!optionPool.options[j].active) continue;

            SpecialSpawnOptionPoolObject* optionObj = &optionPool.options[j];

            if (randomSelect < optionObj->option.weight) {
                addSpecialToSpawnPool(&ctx->objectPools.specialsSpawn, optionObj->option.type);
                optionObj->active = false;
                break;
            }

            randomSelect -= optionObj->option.weight;
        }
    }

}

void spawnSpecials(GameContext* ctx) {
    for (int i = 0; i < NUMBER_OF_SPECIALS; i++) {
        SpecialSpawnPoolObject* spawnObj = &ctx->objectPools.specialsSpawn.specials[i];

        if (!spawnObj->active) continue;

        if (spawnObj->special.spawnTime + ctx->pausTimer <= GetTime()) {
            addSpecialToPool(ctx, spawnObj->special.type);
            spawnObj->active = false;
        }
    }
}

void updateSpecialsAnimations(SpecialsPool* pool) {
    for (int i = 0; i < pool->activeCount; i++) {

    } 
}