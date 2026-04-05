#include "shooting.h"

void addNewShot(ShotObjectPool* pool, Shot shot) {
    if (pool->activeCount >= MAX_SHOTS) {
        printf("Error: Memory overflow in addNewShot\n");
        return;
    }

    if (pool->shots[pool->activeCount].active) {
        printf("Error: Could not add new shot, index allready in use in addNewShot\n");
        return;
    }
    
    pool->shots[pool->activeCount].shot = shot;
    pool->shots[pool->activeCount].active = true;
    pool->activeCount++;
}