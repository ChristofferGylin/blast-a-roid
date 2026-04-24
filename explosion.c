#include "explosion.h"
#include "raylib.h"
#include "animation.h"

void newExplosion(Animation* animation, AnimationPool* pool, Vector2 position) {
    addNewAnimation(pool, animation, position, 0.0f);
}