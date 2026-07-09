#include <stdbool.h>
#include <stdio.h>

#include "debug.h"
#include "gameContext.h"

void outputObjectCountToTerminal(char* name[], ObjectCount oc);
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

void outputObjectCountToTerminal(char* name[], ObjectCount oc) {
    printf("%s:   Active count: %d / %d   Spike: %d\n", name, oc.activeCount, oc.capacity, oc.spike);
}

void outputDebugToTerminal(Debug* debug) {
    outputObjectCountToTerminal("asteroids", debug->poolCount.asteroids);
    outputObjectCountToTerminal("bonuses", debug->poolCount.bonuses);
    outputObjectCountToTerminal("destroyedAsteroids", debug->poolCount.destroyedAsteroids);
    outputObjectCountToTerminal("enemies", debug->poolCount.enemies);
    outputObjectCountToTerminal("explosions", debug->poolCount.explosions);
    outputObjectCountToTerminal("shots", debug->poolCount.shots);
    outputObjectCountToTerminal("spawnableBonuses", debug->poolCount.spawnableBonuses);
    outputObjectCountToTerminal("spawnableEnemies", debug->poolCount.spawnableEnemies);
    outputObjectCountToTerminal("specials", debug->poolCount.specials);
    outputObjectCountToTerminal("specialsSpawn", debug->poolCount.specialsSpawn);
}

void resetObjectCount(ObjectCount* oc, int capacity) {
    oc->activeCount = 0;
    oc->capacity = capacity;
    oc->spike = 0;
}