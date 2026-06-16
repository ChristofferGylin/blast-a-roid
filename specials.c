#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "gameContext.h"
#include "specials.h"

void addSpecialToSpawnPool(SpecialsSpawnPool* pool, SpecialType type);

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

    optionPool.options[3].option.type = SUPERNOVA;
    optionPool.options[3].option.weight = 20;
    
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

            SpecialSpawnOptionPoolObject* optionObj = &optionPool.options[j].option;

            if (randomSelect < optionObj->option.weight) {
                addSpecialToSpawnPool(&ctx->objectPools.specialsSpawn, optionObj->option.type);
                optionObj->active = false;
                break;
            }

            randomSelect -= optionObj->option.weight;
        }
    }

}