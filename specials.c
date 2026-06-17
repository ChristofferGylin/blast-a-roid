#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "animation.h"
#include "gameContext.h"
#include "specials.h"
#include "utils.h"

void addSpecialToPool(GameContext* ctx, SpecialType type);
void addSpecialToSpawnPool(SpecialsSpawnPool* pool, SpecialType type);
void compactSpecialsSpawnPool(SpecialsSpawnPool* pool);
void handleSpecialsMovement(SpecialsPool* pool);
void spawnSpecials(GameContext* ctx);
void updateSpecialsAnimations(SpecialsPool* pool);

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

void compactSpecialsSpawnPool(SpecialsSpawnPool* pool) {
    int write = 0;
    
    for (int i = 0; i < pool->activeCount; i++) {
        if (pool->specials[i].active) {
            if (write != i) {
                pool->specials[write] = pool->specials[i];
            }
            write++;
        }
    }

    for (int i = write; i < pool->activeCount; i++) {
        pool->specials[i].active = false;
    }

    pool->activeCount = write;
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
    
    SpecialSpawnOption optionPool[NUMBER_OF_SPECIALS] = {
        (SpecialSpawnOption){true, MULTIPLIER, 100},
        (SpecialSpawnOption){true, COMET, 100},
        (SpecialSpawnOption){true, EXTRA_LIFE, 30},
        (SpecialSpawnOption){true, BLACK_HOLE, 20},
        (SpecialSpawnOption){true, SUPERNOVA, 10},
    };
    SpecialsSpawnPool* spawnPool = &ctx->objectPools.specialsSpawn;
    
    int minNumberOfSpecials = 0;
    int maxNumberOfSpecials = ceil((ctx->player.level / 2));

    if (maxNumberOfSpecials > NUMBER_OF_SPECIALS) maxNumberOfSpecials = NUMBER_OF_SPECIALS;

    int numberToPopulate = GetRandomValue(minNumberOfSpecials, maxNumberOfSpecials);

    for (int i = 0; i < numberToPopulate; i++) {
        
        int sumOfWeight = 0;

        for (int j = 0; j < NUMBER_OF_SPECIALS; j++) {
        
            if (!optionPool[j].active) continue;
            sumOfWeight += optionPool[j].weight;
        }

        int randomSelect = GetRandomValue(0, sumOfWeight - 1);

        for (int j = 0; j < NUMBER_OF_SPECIALS; j++) {

            if (!optionPool[j].active) continue;

            SpecialSpawnOption* option = &optionPool[j];

            if (randomSelect < option->weight) {
                addSpecialToSpawnPool(&ctx->objectPools.specialsSpawn, option->type);
                option->active = false;
                break;
            }

            randomSelect -= option->weight;
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
        if (!pool->specials[i].active) continue;

        Special* special = &pool->specials[i].special;

        special->animation.position = special->position;
        updateAnimation(&special->animation);
    } 
}