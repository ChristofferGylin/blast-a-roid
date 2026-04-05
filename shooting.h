#include "raylib.h"
#include <stdbool.h>

#define MAX_SHOTS 1024

typedef struct {    
    Vector2 posistion;
    Vector2 direction;
    double destroyTime;
    bool destroyed;
}Shot;

typedef struct {
    bool active;
    Shot shot;
}ShotPoolObject;

typedef struct {
    ShotPoolObject shots[MAX_SHOTS];
    int activeCount;
}ShotObjectPool;