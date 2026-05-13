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

static Bonuses bonuses = {0};

GameResult gameLoop(GameContext* ctx) {

    GameResult result = GAME_CONTINUE;

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
    initBonuses(&bonuses);
    initSpawning(ctx);

    double waitForExitTime = 0;
    bool waitForExit = false;

    float fadeInValue = 1.0f;
    float fadeOutValue = 0.0f;
    
    resetShip(&ctx->ship);

    while(!WindowShouldClose())
    {

        if (IsKeyPressed(KEY_ESCAPE)) {
            isPaused = !isPaused;
        }

        if (isPaused) {
            updatePausMenu(&pauseMenu);

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

            spawnEnemy(ctx);
            resetTimeBonusMultiplier(&ctx->player);
            updateLevelBonus(&ctx->player);
            handleBonuses(ctx, &bonuses);
            clearShots(&ctx->objectPools.shots);
            handleShooting(ctx);
            handleShipControls(ctx);
            handleShield(ctx);
            handleAsteroidsMovement(&ctx->objectPools.asteroids);
            handleEnemiesMovement(ctx);
            handleShotsMovement(&ctx->objectPools.shots);
            handleEnemiesHitDetection(ctx);
            handleDestroyedPiecesMovement(&ctx->ship);
            handleAsteroidCollisions(ctx);
            handleDestroyedAsteroids(ctx);
            handleBonusesCollisions(ctx, &bonuses);
            handleFinishedAnimations(&ctx->objectPools.explosions);
            updateAnimationPool(&ctx->objectPools.explosions);
            updateEnemies(ctx);
            updateBonuses(ctx);

            if (ctx->ship.isShieldActive) {
                updateShieldAnimation();
            }
        }        

        if (ctx->ship.destroyed && GetTime() >= ctx->ship.timeDestroyed + WAIT_TIME && !exit && !reset) {
            ctx->player.lives--;
            isFadeOutComplete = false;
            if (ctx->player.lives < 0) {    
                exit = true;
            } else {
                reset = true;
            }
        } else if (ctx->objectPools.asteroids.activeCount == 0 && !waitForExit) {
            waitForExit = true;
            waitForExitTime = GetTime() + WAIT_TIME;
        }

        if (waitForExit && waitForExitTime < GetTime()) {
            isFadeOutComplete = false;
            exit = true;
        }
        
        BeginDrawing();
            ClearBackground(BLACK);
            renderShip(ctx);
            renderAsteroids(ctx);
            renderShots(ctx);
            renderBonuses(&bonuses, &ctx->objectPools.bonuses);
            renderEnemies(&ctx->objectPools.enemies);
            renderAnimationPool(&ctx->objectPools.explosions);
            renderSidebars(ctx);
            
            if (isPaused) drawPausMenu(&pauseMenu);

            if (!isFadeInComplete) {
                isFadeInComplete = fadeIn(&fadeInValue);
            } else if (!isFadeOutComplete) {
                isFadeOutComplete = fadeOut(&fadeOutValue);
            }
            
        EndDrawing();

        if (exit && isFadeOutComplete) {
            break;
        } else if (reset && isFadeOutComplete) {
            resetShip(&ctx->ship);
            resetAllAsteroids(&ctx->objectPools.asteroids);
            resetPowerups(&ctx->player);
            initBonusSpawnPool(ctx);

            fadeOutValue = 0.0f;
            fadeInValue = 1.0f;

            isFadeInComplete = false;
            reset = false;
        }
    }

    if (WindowShouldClose()) result = EXIT_TO_DESKTOP;
    return result;
}