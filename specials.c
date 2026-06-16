#include "specials.h"

void initSpecialsPool(SpecialsSpawnPool* pool) {
    for (int i = 0; i < NUMBER_OF_SPECIALS; i++) {
        pool->specials[i].active = false;
    }

    pool->activeCount = 0;
}