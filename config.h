#ifndef CONFIG_H
#define CONFIG_H

#include <inttypes.h>
#include <stdbool.h>

#include "debug.h"

typedef struct ObjectPoolCountConfig {
    bool asteroids;
    bool bonuses;
    bool spawnableBonuses;
    bool destroyedAsteroids;
    bool enemies;
    bool spawnableEnemies;
    bool explosions;
    bool shots;
    bool specials;
    bool specialsSpawn;
}ObjectPoolCountConfig;

typedef struct DebugConfig {
    bool onlyOutputOnChange;
    float outputFrequency;
    ObjectPoolCountConfig poolCount;
}DebugConfig;

typedef struct Config {
    DebugConfig debug;
}Config;

typedef struct ConfigSaveData
{
    Config config;
    uint32_t checksum;
}ConfigSaveData;

#endif