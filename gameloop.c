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

static Bonuses bonuses = {0};

GameResult gameLoop(GameContext* ctx) {

    GameResult result = GAME_CONTINUE;

    bool isRunning = true;
    bool isPaused = false;
    bool exit = false;

    ctx->player.levelBonus = (ctx->player.level + 1) * 1000;

    PausMenu pauseMenu;
    initPausMenu(&pauseMenu);
   
    initObjectPools(ctx);
    initAsteroids(ctx);
    initBonuses(&bonuses);

    FaderArgs faderArgs;
    initFaderArgs(&faderArgs);
    
    resetShip(&ctx->ship);

    addNewEnemy(ctx, UFO_1);

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
                        faderArgs.fadeIn = false;
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
        } else if (faderArgs.fadeComplete || !faderArgs.fadeIn) {

            resetTimeBonusMultiplier(&ctx->player);
            updateLevelBonus(&ctx->player);
            handleBonuses(ctx, &bonuses);
            clearShots(&ctx->objectPools.shots);
            handleShooting(ctx);
            handleShipControls(&ctx->ship);
            handleShield(ctx);
            handleAsteroidsMovement(&ctx->objectPools.asteroids);
            handleEnemiesMovement(ctx);
            handleShotsMovement(&ctx->objectPools.shots);
            handleDestroyedPiecesMovement(&ctx->ship);
            handleAsteroidCollisions(ctx);
            handleDestroyedAsteroids(ctx);
            handleBonusesCollisions(ctx, &bonuses);
            handleFinishedAnimations(&ctx->objectPools.explosions);
            updateAnimationPool(&ctx->objectPools.explosions);
            updateEnemies(&ctx->objectPools.enemies);

            if (ctx->ship.isShieldActive) {
                updateShieldAnimation();
            }
        }        

        if (ctx->ship.destroyed && GetTime() >= ctx->ship.timeDestroyed + 2) {
            ctx->player.lives--;

            if (ctx->player.lives < 0) {
                faderArgs.fadeIn = false;
                exit = true;
            } else {
                resetShip(&ctx->ship);
                resetAllAsteroids(&ctx->objectPools.asteroids);
            }
        } else if (ctx->objectPools.asteroids.activeCount == 0) {
            faderArgs.fadeIn = false;
            exit = true;
        }
        
        BeginDrawing();
            ClearBackground(BLACK);
            renderShip(ctx);
            renderAsteroids(ctx);
            renderShots(ctx);
            renderBonuses(&bonuses);
            renderEnemies(&ctx->objectPools.enemies);
            renderAnimationPool(&ctx->objectPools.explosions);
            renderSidebars(ctx);
            
            if (isPaused) drawPausMenu(&pauseMenu);
            fader(&faderArgs);
            
        EndDrawing();

        if (faderArgs.fadeComplete && exit) break;
    }

    if (WindowShouldClose()) result = EXIT_TO_DESKTOP;
    return result;
}