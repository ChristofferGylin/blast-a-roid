#include "animation.h"
#include "asteroid.h"
#include "constants.h"
#include "gameContext.h"
#include "player.h"
#include "raylib.h"
#include "shooting.h"

void initGameContext(GameContext* ctx) {
    initPlayer(&ctx->player);
    initObjectPools(ctx);
    loadAssets(ctx);
}

void initObjectPools(GameContext* ctx) {
    initAnimationPool(&ctx->objectPools.explosions);
    initAsteroidPool(&ctx->objectPools.asteroids);
    initDestroyedAsteroidPool(&ctx->objectPools.destroyedAsteroids);
    initShotObjectPool(&ctx->objectPools.shots);
}

void loadAssets(GameContext* ctx) {
    initAnimation(&ctx->assets.animations.explosion, "./assets/explosion.png", "./assets/explosion.json", 24.0f, (Vector2){EXPLOSION_SIZE, EXPLOSION_SIZE}, false);
    
    ctx->assets.samples.explosion = LoadSound("./assets/samples/explosion.wav");
    ctx->assets.samples.shot = LoadSound("./assets/samples/laser_pew.wav");

    ctx->assets.sprites.asteroid = LoadTexture("./assets/asteroid.png");
    ctx->assets.sprites.ship = LoadTexture("./assets/ship.png");
    ctx->assets.sprites.shot = LoadTexture("./assets/shot.png");
}

void unloadAssets(GameContext* ctx) {
    unloadAnimation(&ctx->assets.animations.explosion);

    UnloadSound(ctx->assets.samples.explosion);
    UnloadSound(ctx->assets.samples.shot);

    UnloadTexture(ctx->assets.sprites.asteroid);
    UnloadTexture(ctx->assets.sprites.ship);
    UnloadTexture(ctx->assets.sprites.shot);
}