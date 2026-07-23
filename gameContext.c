#include <stdbool.h>
#include <string.h>
#include "animation.h"
#include "asteroid.h"
#include "config.h"
#include "constants.h"
#include "debug.h"
#include "gameContext.h"
#include "player.h"
#include "raylib.h"
#include "score.h"
#include "shooting.h"
#include "specials.h"
#include "ship.h"

static const int FPS_TITLE_FONT_SIZE = 18;
static const int FPS_TITLE_FONT_SPACING = 8;
static const int FPS_CURRENT_VALUE_FONT_SIZE = 24;
static const int FPS_VALUE_FONT_SIZE = 12;
static const int FPS_GAP = 8;

void initSpawning(GameContext* ctx) {
    setSpawnDelay(ctx);
    setNextEnemySpawnTime(ctx);
}

void initFps(Fps* fps) {

    strcpy(fps->render.titles.current, "CURRENT");
    strcpy(fps->render.titles.highest, "HIGHEST");
    strcpy(fps->render.titles.lowest, "LOWEST");

    char value[] = "100";
    
    fps->currentFps = 0;
    fps->highestFps = 0;
    fps->lowestFps = 0;

    Vector2 currentTitleSize = MeasureTextEx(GetFontDefault(), fps->render.titles.current, FPS_TITLE_FONT_SIZE, FPS_TITLE_FONT_SPACING);
    Vector2 highestTitleSize = MeasureTextEx(GetFontDefault(), fps->render.titles.highest, FPS_TITLE_FONT_SIZE, FPS_TITLE_FONT_SPACING);
    Vector2 lowestTitleSize = MeasureTextEx(GetFontDefault(), fps->render.titles.lowest, FPS_TITLE_FONT_SIZE, FPS_TITLE_FONT_SPACING);
    
    Vector2 valueSize = MeasureTextEx(GetFontDefault(), value, FPS_CURRENT_VALUE_FONT_SIZE, FPS_TITLE_FONT_SPACING);

    float totalHeight =
        currentTitleSize.y +
        highestTitleSize.y +
        lowestTitleSize.y +
        (valueSize.y * 3) + 
        (FPS_GAP * 9); 

    float center = (SIDEBAR_WIDTH / 2.0f);

    fps->render.positions.currentTitle = (Vector2){
        center - (currentTitleSize.x / 2.0f),
        SCREEN_HEIGHT - totalHeight
    };

    fps->render.positions.currentValue = (Vector2){
        center + (valueSize.x / 2),
        fps->render.positions.currentTitle.y + currentTitleSize.y + FPS_GAP
    };

    fps->render.positions.highestTitle = (Vector2){
        center - (highestTitleSize.x / 2.0f),
        fps->render.positions.currentValue.y + valueSize.y + (FPS_GAP * 2)
    };

    fps->render.positions.highestValue = (Vector2){
        center + (valueSize.x / 2),
        fps->render.positions.highestTitle.y + highestTitleSize.y + FPS_GAP
    };

    fps->render.positions.lowestTitle = (Vector2){
        center - (lowestTitleSize.x / 2.0f),
        fps->render.positions.highestValue.y + valueSize.y + (FPS_GAP * 2)
    };

    fps->render.positions.lowestValue = (Vector2){
        center + (valueSize.x / 2),
        fps->render.positions.lowestTitle.y + lowestTitleSize.y + FPS_GAP
    };

}

void initGameContext(GameContext* ctx, bool debugActive) {
    initPlayer(&ctx->player);
    initObjectPools(ctx);
    loadAssets(ctx);
    initShip(ctx, &ctx->ship);
    initSpawning(ctx);
    initHighScores(&ctx->highscores);
    initDebug(&ctx->debug, debugActive);
    initConfig(ctx);
    initFps(&ctx->fps);
    ctx->pausTimer = 0;
    ctx->supernova = (Supernova){false, 0.0f, 0.0f};
    ctx->isBlackHoleActive = false;
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
    initSpecialsPool(&ctx->objectPools.specials);
    initSpecialsSpawnPool(ctx);
}

void loadAssets(GameContext* ctx) {
    initAnimation(&ctx->assets.animations.blackHole, "./assets/animations/black_hole.png", "./assets/animations/black_hole.json", 20.0f, (Vector2){BLACK_HOLE_SIZE, BLACK_HOLE_SIZE}, true);
    initAnimation(&ctx->assets.animations.comet, "./assets/animations/comet.png", "./assets/animations/comet.json", 12.0f, (Vector2){COMET_RENDER_SIZE_X, COMET_RENDER_SIZE_Y}, true);
    initAnimation(&ctx->assets.animations.crate, "./assets/animations/crate.png", "./assets/animations/crate.json", 24.0f, (Vector2){CRATE_RENDER_SIZE, CRATE_RENDER_SIZE}, true);
    initAnimation(&ctx->assets.animations.explosion, "./assets/animations/explosion.png", "./assets/animations/explosion.json", 24.0f, (Vector2){EXPLOSION_SIZE, EXPLOSION_SIZE}, false);
    initAnimation(&ctx->assets.animations.metalAsteroid, "./assets/animations/metal_asteroid.png", "./assets/animations/metal_asteroid.json", 24.0f, (Vector2){METAL_ASTEROID_SIZE, METAL_ASTEROID_SIZE}, true);
    initAnimation(&ctx->assets.animations.multiplier, "./assets/animations/multiplier.png", "./assets/animations/multiplier.json", 24.0f, (Vector2){MULTIPLIER_RENDER_SIZE, MULTIPLIER_RENDER_SIZE}, true);
    initAnimation(&ctx->assets.animations.spikyAsteroid, "./assets/animations/spiky_asteroid.png", "./assets/animations/spiky_asteroid.json", 30.0f, (Vector2){SPIKY_ASTEROID_SIZE, SPIKY_ASTEROID_SIZE}, true);
    initAnimation(&ctx->assets.animations.supernova, "./assets/animations/supernova.png", "./assets/animations/supernova.json", 30.0f, (Vector2){SUPERNOVA_RENDER_SIZE, SUPERNOVA_RENDER_SIZE}, false);
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
    ctx->assets.samples.supernova = LoadSound("./assets/samples/supernova.wav");

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
    unloadAnimation(&ctx->assets.animations.blackHole);
    unloadAnimation(&ctx->assets.animations.comet);
    unloadAnimation(&ctx->assets.animations.crate);
    unloadAnimation(&ctx->assets.animations.explosion);
    unloadAnimation(&ctx->assets.animations.metalAsteroid);
    unloadAnimation(&ctx->assets.animations.multiplier);
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
    UnloadSound(ctx->assets.samples.supernova);

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