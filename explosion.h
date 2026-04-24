#ifndef EXPLOSION
#define EXPLOSION

#include "raylib.h"

typedef struct Animation Animation;
typedef struct AnimationPool AnimationPool;

void newExplosion(Animation* animation, AnimationPool* pool, Sound* sample, Vector2 position);

#endif