#include "explosion.h"
#include "raylib.h"
#include "animation.h"

void newExplosion(Animation* animation, AnimationPool* pool, Sound* sample, Vector2 position) {
    addNewAnimation(pool, animation, position, 0.0f);
    PlaySound(*sample);
}