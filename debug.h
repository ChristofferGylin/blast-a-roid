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

typedef struct DebugOutputOptions {
    bool* onlyOutputOnChange;
    float* outputFrequency;
    Button increaseButton;
    Button decreaseButton;
    CheckboxWithTitle outputOnChangeCheckbox;
    Rectangle valueDisplay;
    char frequencyHeadingText[10];
    Vector2 frequencyHeadingPosition;
}DebugOutputOptions;

typedef struct DebugOutputOptionsSection {
    LayoutSection section;
    DebugOutputOptions options;
}DebugOutputOptionsSection;

typedef struct ObjectCount {
    int activeCount;
    int capacity;
    int spike;
    bool showInDebug;
}ObjectCount;

typedef struct ObjectCountSection {
    LayoutSection section;
    CheckboxWithTitle options[NUMBER_OF_POOL_COUNTS];
    DebugOutputOptions outputOptions;
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
    DebugOutputOptionsSection outputOptionsSection;
    ObjectCountSection objectCountSection;
    bool exit;
}DebugMenu;

bool debugMenu(GameContext* ctx);
void initDebug(Debug* debug, bool active);
void resetDebug(Debug* debug);
void updateDebug(GameContext* ctx);

#endif