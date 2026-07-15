#ifndef DEBUG_H
#define DEBUG_H

#include <stdbool.h>

#include "ui.h"
#define NUMBER_OF_POOL_COUNTS 10

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


typedef struct GameContext GameContext;

typedef struct ObjectCount {
    int activeCount;
    int capacity;
    int spike;
    bool showInDebug;
}ObjectCount;

typedef struct ObjectCountOption {
    Checkbox checkbox;
    Vector2 titlePosition;
    char title[32];
}ObjectCountOption;

typedef struct ObjectCountSection {
    LayoutSection section;
    ObjectCountOption options[NUMBER_OF_POOL_COUNTS];
    Vector2 outputOptionsPosition;
    bool* onlyOutputOnChange;
    float* outputFrequency;
}ObjectCountSection;

typedef struct ObjectPoolCount {
    ObjectCount asteroids;
    ObjectCount bonuses;
    ObjectCount spawnableBonuses;
    ObjectCount destroyedAsteroids;
    ObjectCount enemies;
    ObjectCount spawnableEnemies;
    ObjectCount explosions;
    ObjectCount shots;
    ObjectCount specials;
    ObjectCount specialsSpawn;
}ObjectPoolCount;

typedef struct Debug {
    bool active;
    float updateTimer;
    bool onlyOutputOnChange;
    float outputFrequency;
    ObjectPoolCount poolCount;
}Debug;

typedef struct DebugMenu {
    BasicLayoutContainer layout;
    Button backButton;
    ObjectCountSection objectCountSection;
    bool exit;
}DebugMenu;

bool debugMenu(GameContext* ctx);
void initDebug(Debug* debug, bool active);
void updateDebug(GameContext* ctx);

#endif