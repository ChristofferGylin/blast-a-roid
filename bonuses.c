#include "bonuses.h"
#include "raylib.h"

const int MIN_BONUS_SPAWN_TIME = 10;
const int MAX_BONUS_SPAWN_TIME = 60;
const int BONUS_LIFE_TIME = 30;

double getNextSpawnTime() {
    return GetTime() + GetRandomValue(MIN_BONUS_SPAWN_TIME, MAX_BONUS_SPAWN_TIME);
}