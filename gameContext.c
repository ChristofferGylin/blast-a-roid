#include "animation.h"
#include "asteroid.h"
#include "constants.h"
#include "gameContext.h"
#include "player.h"
#include "raylib.h"
#include "shooting.h"
#include "ship.h"

void initSpawning(GameContext* ctx) {
    setSpawnDelay(ctx);
    setNextEnemySpawnTime(ctx);
}

void initGameContext(GameContext* ctx) {
    initPlayer(&ctx->player);
    initObjectPools(ctx);
    loadAssets(ctx);
    initShip(ctx);
    initSpawning(ctx);
}

void initObjectPools(GameContext* ctx) {
    initAnimationPool(&ctx->objectPools.explosions);
    initAsteroidPool(&ctx->objectPools.asteroids);
    initBonusPool(&ctx->objectPools.bonuses);
    initBonusSpawnPool(ctx);
    initDestroyedAsteroidPool(&ctx->objectPools.destroyedAsteroids);
    initEnemyPool(&ctx->objectPools.enemies);
    initEnemySpawnPool(ctx);
    initShotObjectPool(&ctx->objectPools.shots);
}

void loadAssets(GameContext* ctx) {
    initAnimation(&ctx->assets.animations.crate, "./assets/animations/crate.png", "./assets/animations/crate.json", 24.0f, (Vector2){CRATE_RENDER_SIZE, CRATE_RENDER_SIZE}, true);
    initAnimation(&ctx->assets.animations.explosion, "./assets/animations/explosion.png", "./assets/animations/explosion.json", 24.0f, (Vector2){EXPLOSION_SIZE, EXPLOSION_SIZE}, false);
    initAnimation(&ctx->assets.animations.metalAsteroid, "./assets/animations/metal_asteroid.png", "./assets/animations/metal_asteroid.json", 24.0f, (Vector2){METAL_ASTEROID_SIZE, METAL_ASTEROID_SIZE}, true);
    initAnimation(&ctx->assets.animations.spikyAsteroid, "./assets/animations/spiky_asteroid.png", "./assets/animations/spiky_asteroid.json", 30.0f, (Vector2){SPIKY_ASTEROID_SIZE, SPIKY_ASTEROID_SIZE}, true);
    initAnimation(&ctx->assets.animations.supernova, "./assets/animations/supernova.png", "./assets/animations/supernova.json", 24.0f, (Vector2){SPIKY_ASTEROID_SIZE, SPIKY_ASTEROID_SIZE}, true);
    initAnimation(&ctx->assets.animations.ufo1, "./assets/animations/ufo1.png", "./assets/animations/ufo1.json", 24.0f, (Vector2){UFO_1_SIZE, UFO_1_SIZE}, true);
    initAnimation(&ctx->assets.animations.ufo2, "./assets/animations/ufo2.png", "./assets/animations/ufo2.json", 24.0f, (Vector2){UFO_1_SIZE, UFO_1_SIZE}, true);
    initAnimation(&ctx->assets.animations.ufo3, "./assets/animations/ufo3.png", "./assets/animations/ufo3.json", 24.0f, (Vector2){UFO_3_RENDER_SIZE, UFO_3_RENDER_SIZE}, true);
    
    ctx->assets.samples.alarm = LoadSound("./assets/samples/alarm.wav");
    ctx->assets.samples.enemyShot1 = LoadSound("./assets/samples/laser_pew2.wav");
    ctx->assets.samples.explosion = LoadSound("./assets/samples/explosion.wav");
    ctx->assets.samples.metalPlink = LoadSound("./assets/samples/metal_plink.wav");
    ctx->assets.samples.multiplier_collect = LoadSound("./assets/samples/multiplier_collect.wav");
    ctx->assets.samples.multiplier_spawn = LoadSound("./assets/samples/multiplier_spawn.wav");
    ctx->assets.samples.shieldUp = LoadSound("./assets/samples/shield_up.wav");
    ctx->assets.samples.shot = LoadSound("./assets/samples/laser_pew1.wav");

    ctx->assets.sprites.asteroid = LoadTexture("./assets/sprites/asteroid.png");
    ctx->assets.sprites.autoShotIcon = LoadTexture("./assets/sprites/auto_shot_icon.png");
    ctx->assets.sprites.blueGem = LoadTexture("./assets/sprites/blue_gem.png");
    ctx->assets.sprites.destroyedShip1 = LoadTexture("./assets/sprites/ship_destroyed_1.png");
    ctx->assets.sprites.destroyedShip2 = LoadTexture("./assets/sprites/ship_destroyed_2.png");
    ctx->assets.sprites.destroyedShip3 = LoadTexture("./assets/sprites/ship_destroyed_3.png");
    ctx->assets.sprites.enemyShot1 = LoadTexture("./assets/sprites/enemy_shot_1.png");
    ctx->assets.sprites.lockIcon = LoadTexture("./assets/sprites/lock_icon.png");
    ctx->assets.sprites.longShotIcon = LoadTexture("./assets/sprites/long_shot_icon.png");
    ctx->assets.sprites.ship = LoadTexture("./assets/sprites/ship.png");
    ctx->assets.sprites.shot = LoadTexture("./assets/sprites/shot.png");
    ctx->assets.sprites.stopIcon = LoadTexture("./assets/sprites/stop_icon.png");
    ctx->assets.sprites.triShotIcon = LoadTexture("./assets/sprites/tri_shot_icon.png");
}

void unloadAssets(GameContext* ctx) {
    unloadAnimation(&ctx->assets.animations.crate);
    unloadAnimation(&ctx->assets.animations.explosion);
    unloadAnimation(&ctx->assets.animations.metalAsteroid);
    unloadAnimation(&ctx->assets.animations.spikyAsteroid);
    unloadAnimation(&ctx->assets.animations.supernova);
    unloadAnimation(&ctx->assets.animations.ufo1);
    unloadAnimation(&ctx->assets.animations.ufo2);
    unloadAnimation(&ctx->assets.animations.ufo3);

    UnloadSound(ctx->assets.samples.alarm);
    UnloadSound(ctx->assets.samples.explosion);
    UnloadSound(ctx->assets.samples.enemyShot1);
    UnloadSound(ctx->assets.samples.metalPlink);
    UnloadSound(ctx->assets.samples.multiplier_collect);
    UnloadSound(ctx->assets.samples.multiplier_spawn);
    UnloadSound(ctx->assets.samples.shieldUp);
    UnloadSound(ctx->assets.samples.shot);

    UnloadTexture(ctx->assets.sprites.asteroid);
    UnloadTexture(ctx->assets.sprites.autoShotIcon);
    UnloadTexture(ctx->assets.sprites.blueGem);
    UnloadTexture(ctx->assets.sprites.destroyedShip1);
    UnloadTexture(ctx->assets.sprites.destroyedShip2);
    UnloadTexture(ctx->assets.sprites.destroyedShip3);
    UnloadTexture(ctx->assets.sprites.enemyShot1);
    UnloadTexture(ctx->assets.sprites.lockIcon);
    UnloadTexture(ctx->assets.sprites.longShotIcon);
    UnloadTexture(ctx->assets.sprites.ship);
    UnloadTexture(ctx->assets.sprites.shot);
    UnloadTexture(ctx->assets.sprites.stopIcon);
    UnloadTexture(ctx->assets.sprites.triShotIcon);
}