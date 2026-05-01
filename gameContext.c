#include "animation.h"
#include "asteroid.h"
#include "constants.h"
#include "gameContext.h"
#include "player.h"
#include "raylib.h"
#include "shooting.h"
#include "ship.h"

void initGameContext(GameContext* ctx) {
    initPlayer(&ctx->player);
    initObjectPools(ctx);
    loadAssets(ctx);
    initShip(ctx);
}

void initObjectPools(GameContext* ctx) {
    initAnimationPool(&ctx->objectPools.explosions);
    initAsteroidPool(&ctx->objectPools.asteroids);
    initDestroyedAsteroidPool(&ctx->objectPools.destroyedAsteroids);
    initEnemyPool(&ctx->objectPools.enemies);
    initShotObjectPool(&ctx->objectPools.shots);
}

void loadAssets(GameContext* ctx) {
    initAnimation(&ctx->assets.animations.explosion, "./assets/explosion.png", "./assets/explosion.json", 24.0f, (Vector2){EXPLOSION_SIZE, EXPLOSION_SIZE}, false);
    initAnimation(&ctx->assets.animations.ufo1, "./assets/ufo1.png", "./assets/ufo1.json", 24.0f, (Vector2){UFO_1_SIZE, UFO_1_SIZE}, true);
    
    ctx->assets.samples.explosion = LoadSound("./assets/samples/explosion.wav");
    ctx->assets.samples.multiplier_collect = LoadSound("./assets/samples/multiplier_collect.wav");
    ctx->assets.samples.multiplier_spawn = LoadSound("./assets/samples/multiplier_spawn.wav");
    ctx->assets.samples.shot = LoadSound("./assets/samples/laser_pew.wav");

    ctx->assets.sprites.asteroid = LoadTexture("./assets/asteroid.png");
    ctx->assets.sprites.destroyedShip1 = LoadTexture("./assets/ship_destroyed_1.png");
    ctx->assets.sprites.destroyedShip2 = LoadTexture("./assets/ship_destroyed_2.png");
    ctx->assets.sprites.destroyedShip3 = LoadTexture("./assets/ship_destroyed_3.png");
    ctx->assets.sprites.ship = LoadTexture("./assets/ship.png");
    ctx->assets.sprites.shot = LoadTexture("./assets/shot.png");
}

void unloadAssets(GameContext* ctx) {
    unloadAnimation(&ctx->assets.animations.explosion);
    unloadAnimation(&ctx->assets.animations.ufo1);

    UnloadSound(ctx->assets.samples.explosion);
    UnloadSound(ctx->assets.samples.shot);

    UnloadTexture(ctx->assets.sprites.asteroid);
    UnloadTexture(ctx->assets.sprites.destroyedShip1);
    UnloadTexture(ctx->assets.sprites.destroyedShip2);
    UnloadTexture(ctx->assets.sprites.destroyedShip3);
    UnloadTexture(ctx->assets.sprites.ship);
    UnloadTexture(ctx->assets.sprites.shot);
}