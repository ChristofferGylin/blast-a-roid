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

void compactShotPool(ShotObjectPool* pool) {
    int write = 0;

    for (int i = 0; i < pool->activeCount; i++) {
        if (pool->shots[i].active) {
            if (write != i) {
                pool->shots[write] = pool->shots[i];
            }
            write++;
        }
    }

    for (int i = write; i < pool->activeCount; i++) {
        pool->shots[i].active = false;
    }

    pool->activeCount = write;
}