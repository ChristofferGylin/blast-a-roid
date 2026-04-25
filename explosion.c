#include "explosion.h"
#include "raylib.h"
#include "animation.h"
#include "gameContext.h"

void newExplosion(GameContext* ctx, Vector2 position) {
    addNewAnimation(&ctx->objectPools.explosions, &ctx->assets.animations.explosion, position, 0.0f);
    PlaySound(ctx->assets.samples.explosion);
}