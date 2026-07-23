#ifndef CONFIG_H
#define CONFIG_H

#include <inttypes.h>
#include <stdbool.h>

#include "debug.h"
#include "options.h"

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
    Options options;
}Config;

typedef struct ConfigSaveData
{
    Config config;
    uint32_t checksum;
}ConfigSaveData;

bool compareConfig(Config* config1, Config* config2);
Config getConfig(GameContext* ctx);
void initConfig(GameContext* ctx);
void resetConfig(GameContext* ctx);
void resetDebugConfig(GameContext* ctx);
void saveConfigToFile(GameContext* ctx);

#endif