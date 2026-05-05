#ifndef ENEMYSPAWNPOOL_H
#define ENEMYSPAWNPOOL_H

#include "enemies.h"
#include "utils.h"

EnemySpawnOption enemyOptionsLevel1[] = {
   {UFO_1, 100.0f, 0, 1},
};

EnemySpawnOption enemyOptionsLevel2[] = {
   {UFO_1, 100.0f, 0, 2},
};

EnemySpawnOption enemyOptionsLevel3[] = {
   {UFO_1, 100.0f, 0, 2},
};

EnemySpawnOption enemyLevelsOptions[][NUMBER_OF_ENEMY_TYPES] = {
    *enemyOptionsLevel1,
    *enemyOptionsLevel2,
    *enemyOptionsLevel3
};

#endif