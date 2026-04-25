#ifndef EXPLOSION
#define EXPLOSION

#include "raylib.h"

typedef struct Animation Animation;
typedef struct AnimationPool AnimationPool;

void newExplosion(GameContext* ctx, Vector2 position);

#endif