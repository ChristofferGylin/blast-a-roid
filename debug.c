#include <stdbool.h>
#include <stdio.h>

#include "debug.h"
#include "gameContext.h"
#include "raylib.h"

#define POOL_COUNTS(X)               \
    X(asteroids)                     \
    X(bonuses)                       \
    X(destroyedAsteroids)            \
    X(enemies)                       \
    X(explosions)                    \
    X(shots)                         \
    X(spawnableBonuses)              \
    X(spawnableEnemies)              \
    X(specials)                      \
    X(specialsSpawn)

void outputDebugToTerminal(Debug* debug);
void outputObjectCountToTerminal(const char* name, ObjectCount oc);
void resetObjectCount(ObjectCount* oc, int capacity);

void initDebug(Debug* debug, bool active) {
    debug->active = active;
    debug->updateTimer = 0.0f;

    resetObjectCount(&debug->poolCount.asteroids, MAX_ASTEROIDS);
    resetObjectCount(&debug->poolCount.bonuses, MAX_BONUSES);
    resetObjectCount(&debug->poolCount.destroyedAsteroids, MAX_ASTEROIDS);
    resetObjectCount(&debug->poolCount.enemies, MAX_ENEMIES);
    resetObjectCount(&debug->poolCount.explosions, MAX_ANIMATIONS);
    resetObjectCount(&debug->poolCount.shots, MAX_SHOTS);
    resetObjectCount(&debug->poolCount.spawnableBonuses, NUMBER_OF_BONUS_TYPES);
    resetObjectCount(&debug->poolCount.spawnableEnemies, NUMBER_OF_ENEMY_TYPES);
    resetObjectCount(&debug->poolCount.specials, NUMBER_OF_SPECIALS);
    resetObjectCount(&debug->poolCount.specialsSpawn, NUMBER_OF_SPECIALS);
    
}

void outputObjectCountToTerminal(const char* name, ObjectCount oc) {
    printf("%-21s Active count: %4d / %4d   Spike: %4d\n", name, oc.activeCount, oc.capacity, oc.spike);
}

void outputDebugToTerminal(Debug* debug) {

    #define OUTPUT(name) \
        outputObjectCountToTerminal(#name, debug->poolCount.name);

        POOL_COUNTS(OUTPUT)
    #undef OUTPUT
    printf("\n");
}

void resetObjectCount(ObjectCount* oc, int capacity) {
    oc->activeCount = 0;
    oc->capacity = capacity;
    oc->spike = 0;
}

void updateDebug(GameContext* ctx) {

    Debug* debug = &ctx->debug;

    if (!debug->active) return;

    const float updateFrequency = 1.0f;

    debug->updateTimer += GetFrameTime();

    if (debug->updateTimer < updateFrequency) return;
    
    debug->updateTimer = 0.0f;

    #define OUTPUT(name)                                                     \
        do {                                                                 \
            debug->poolCount.name.activeCount = ctx->objectPools.name.activeCount; \
            if (debug->poolCount.name.activeCount > debug->poolCount.name.spike) { \
                debug->poolCount.name.spike = debug->poolCount.name.activeCount;    \
            }                                                                \
        } while (0);

    POOL_COUNTS(OUTPUT)

    #undef OUTPUT

    outputDebugToTerminal(debug);
}