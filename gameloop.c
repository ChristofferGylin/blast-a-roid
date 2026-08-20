#include "debug.h"
#include "raylib.h"
#include "gameContext.h"
#include "gameloop.h"
#include "asteroid.h"
#include "ship.h"
#include <math.h>
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include "shooting.h"
#include "score.h"
#include "sidebars.h"
#include <stdbool.h>
#include "fader.h"
#include "pauseMenu.h"
#include "shield.h"
#include "bonuses.h"
#include "animation.h"
#include "enemies.h"
#include "gameContext.h"
#include "specials.h"
#include "showFps.h"

GameResult gameLoop(GameContext* ctx) {

    HideCursor();

    GameResult result = GAME_CONTINUE;
    Fps fps;

    initFps(&fps);
    initFrameTime(&fps.frameTime);

    const int WAIT_TIME = 2;

    bool isFadeInComplete = false;
    bool isFadeOutComplete = true;
    bool isPaused = false;
    bool reset = false;
    bool exit = false;
    bool wait = false;

    ctx->player.levelBonus = (ctx->player.level + 1) * 1000;

    PausMenu pauseMenu;
    initPausMenu(&pauseMenu);
   
    initObjectPools(ctx);
    initAsteroids(ctx);
    initSpawning(ctx);
    resetSupernova(&ctx->supernova);
    ctx->pausTimer = 0;

    if (ctx->debug.active) {
        resetDebug(&ctx->debug);
        printf("\n\n --- Start level %d ---\n\n", ctx->player.level);
    }

    double waitForExitTime = 0;
    bool waitForExit = false;

    float fadeInValue = 1.0f;
    float fadeOutValue = 0.0f;
    
    resetShip(&ctx->ship);

    while(!WindowShouldClose())
    {
        updateRendering(&ctx->rendering);

        if (IsKeyPressed(KEY_ESCAPE)) {
            isPaused = !isPaused;
        }

        if (isPaused) {
            ShowCursor();
            updatePausMenu(&pauseMenu);
            ctx->pausTimer += GetFrameTime();

            if (pauseMenu.selected != -1) {
                switch (pauseMenu.selected) {
                    case 0: 
                        isPaused = false;
                        break;
                    case 1:
                        // TODO: Options
                        break;
                    case 2:
                        exit = true;
                        result = EXIT_TO_MENU;
                        break;
                    case 3:
                        return EXIT_TO_DESKTOP;
                    default:
                        break;
                }

                pauseMenu.selected = -1;
            }
        } else if (isFadeInComplete) {

            HideCursor();

            if (!waitForExit) {
                spawnEnemy(ctx);
                spawnSpecials(ctx);
            }

            resetTimeBonusMultiplier(ctx);
            updateLevelBonus(&ctx->player);
            clearShots(ctx);
            handleShooting(ctx);
            handleShipControls(ctx);
            handleShield(ctx);
            handleAsteroidsMovement(ctx);
            handleEnemiesMovement(ctx);
            handleShotsMovement(&ctx->objectPools.shots);
            handleSpecialsMovement(ctx);
            updateSpecials(ctx);
            handleSpecialsHitDetection(ctx);
            handleSpecialsCollisions(ctx);
            handleEnemiesHitDetection(ctx);
            handleEnemiesCollisions(ctx);
            handleDestroyedPiecesMovement(&ctx->ship);
            handleAsteroidCollisions(ctx);
            handleDestroyedAsteroids(ctx);
            handleBonusesCollisions(ctx);
            handleFinishedAnimations(&ctx->objectPools.explosions);
            updateAnimationPool(&ctx->objectPools.explosions);
            updateEnemies(ctx);
            updateBonuses(ctx);
            updateAsteroidsAnimations(ctx);
            updateShip(&ctx->ship);

            if (ctx->ship.isShieldActive) {
                updateShieldAnimation();
            }
            updateFps(ctx, &fps);
            updateFrameTime(ctx, &fps.frameTime);
        }        

        if (ctx->ship.destroyed && GetTime() >= ctx->ship.timeDestroyed + WAIT_TIME && !exit && !reset) {
            ctx->player.lives--;
            isFadeOutComplete = false;
            if (ctx->player.lives < 0) {    
                exit = true;
            } else {
                reset = true;
            }
        } else if (
            countAsteroids(&ctx->objectPools.asteroids) == 0 &&
            countSpecials(ctx) == 0 &&
            ctx->objectPools.bonuses.activeCount == 0 &&
            ctx->objectPools.enemies.activeCount == 0 &&
            !waitForExit
        ) {
            waitForExit = true;
            waitForExitTime = GetTime() + WAIT_TIME;
        }

        if (waitForExit && waitForExitTime < GetTime()) {
            isFadeOutComplete = false;
            exit = true;
        }

        if (!isPaused) {
            updateDebug(ctx);
        }
        
        BeginTextureMode(ctx->rendering.renderTexture);
            ClearBackground(BLACK);
            renderShip(&ctx->ship);
            renderAsteroids(ctx);
            renderShots(ctx);
            renderBonuses(&ctx->objectPools.bonuses);
            renderSpecials(&ctx->objectPools.specials);
            renderEnemies(&ctx->objectPools.enemies);
            renderAnimationPool(&ctx->objectPools.explosions);
            renderSidebars(ctx, &fps);
            
            if (isPaused) drawPausMenu(&pauseMenu);

            if (!isFadeInComplete) {
                isFadeInComplete = fadeIn(&fadeInValue);
            } else if (!isFadeOutComplete) {
                isFadeOutComplete = fadeOut(&fadeOutValue);
            }
            
        EndTextureMode();

        renderToScreen(&ctx->rendering);

        if (exit && isFadeOutComplete) {
            break;
        } else if (reset && isFadeOutComplete) {
            resetShip(&ctx->ship);
            resetAllAsteroids(&ctx->objectPools.asteroids);
            updateAsteroidsAnimations(ctx);
            resetPowerups(&ctx->player);
            initBonusSpawnPool(ctx);
            initShotObjectPool(&ctx->objectPools.shots);

            fadeOutValue = 0.0f;
            fadeInValue = 1.0f;

            isFadeInComplete = false;
            reset = false;
        }
    }

    if (WindowShouldClose()) result = EXIT_TO_DESKTOP;
    return result;
}