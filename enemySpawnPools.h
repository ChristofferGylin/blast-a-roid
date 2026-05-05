#ifndef ENEMYSPAWNPOOL_H
#define ENEMYSPAWNPOOL_H

#include "enemies.h"
#include "utils.h"

const EnemySpawnOption enemyOptionsLevel1[] = {
   {UFO_1, 100, 0, 1},
};

EnemySpawnPool enemySpawnPoolLevel1 = {
    enemyOptionsLevel1,
    sizeof(enemySpawnPoolLevel1) / sizeof(EnemySpawnOption),
    (FloatRange){10, 60}
};

const EnemySpawnOption enemyOptionsLevel2[] = {
   {UFO_1, 100, 0, 2},
};

EnemySpawnPool enemySpawnPoolLevel2 = {
    enemyOptionsLevel2,
    sizeof(enemySpawnPoolLevel2) / sizeof(EnemySpawnOption),
    (FloatRange){10, 50}
};

const EnemySpawnOption enemyOptionsLevel3[] = {
   {UFO_1, 100, 0, 2},
};

EnemySpawnPool enemySpawnPoolLevel3 = {
    enemyOptionsLevel3,
    sizeof(enemySpawnPoolLevel3) / sizeof(EnemySpawnOption),
    (FloatRange){5, 40}
};

#endif