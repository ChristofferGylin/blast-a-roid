#ifndef EXPLOSION
#define EXPLOSION

#include "raylib.h"

typedef struct GameContext GameContext;

void newExplosion(GameContext* ctx, Vector2 position);

#endif