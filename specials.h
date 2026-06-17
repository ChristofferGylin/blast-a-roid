#ifndef SPECIALS_H
#define SPECIALS_H

#include <stdbool.h>

#include "animation.h"
#include "raylib.h"
#include "utils.h"

static const int NUMBER_OF_SPECIALS = 5;

typedef enum SpecialType {
    MULTIPLIER,
    COMET,
    EXTRA_LIFE,
    BLACK_HOLE,
    SUPERNOVA
}SpecialType;

typedef struct Special {
    SpecialType type;
    double spawnTime;
    Vector2 position;
    Vector2 velocity;
    Vector2 size;
    float rotation;
    float rotationSpeed;
    float value;
    AnimationInstance animation;
}Special;

typedef struct SpecialPoolObject {
    bool active;
    Special special;
}SpecialPoolObject;

typedef struct SpecialsPool {
    SpecialPoolObject specials[NUMBER_OF_SPECIALS];
    int activeCount;
}SpecialsPool;

typedef struct SpecialSpawn {
    SpecialType type;
    double spawnTime;
}SpecialSpawn;

typedef struct SpecialSpawnPoolObject {
    bool active;
    SpecialSpawn special;
}SpecialSpawnPoolObject;

typedef struct SpecialsSpawnPool {
    SpecialSpawnPoolObject specials[NUMBER_OF_SPECIALS];
    int activeCount;
}SpecialsSpawnPool;

typedef struct SpecialSpawnOption {
    SpecialType type;
    int weight;
}SpecialSpawnOption;

typedef struct SpecialSpawnOptionPoolObject {
    bool active;
    SpecialSpawnOption option;
}SpecialSpawnOptionPoolObject;

typedef struct SpecialsSpawnOptionPool {
    SpecialSpawnOptionPoolObject options[NUMBER_OF_SPECIALS];
    int activeCount;
}SpecialsSpawnOptionPool;

void initSpecialsPool(SpecialsPool* pool);
void initSpecialsSpawnPool(SpecialsSpawnPool* pool);
void populateSpecialsSpawnPool(GameContext* ctx);

#endif