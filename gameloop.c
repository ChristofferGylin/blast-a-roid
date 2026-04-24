#include "raylib.h"
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

static AsteroidPool asteroidObjectPool = {0};
static DestroyedAsteroidPool destroyedAsteroidsObjectPool = {0};
static ShotObjectPool shotsObjectPool = {0};
static AnimationPool explosionPool = {0};
static Bonuses bonuses = {0};

GameResult gameLoop(Player* player) {

    GameResult result = GAME_CONTINUE;

    bool isRunning = true;
    bool isPaused = false;
    bool exit = false;

    player->levelBonus = (player->level + 1) * 1000;

    PausMenu pauseMenu;
    initPausMenu(&pauseMenu);
   
    initAsteroidPool(&asteroidObjectPool);
    initDestroyedAsteroidPool(&destroyedAsteroidsObjectPool);
    initShotObjectPool(&shotsObjectPool);
    initAnimationPool(&explosionPool);
    initAsteroids(&asteroidObjectPool, player->level);
    initBonuses(&bonuses);

    FaderArgs faderArgs;
    initFaderArgs(&faderArgs);

    Texture2D asteroidSprite = LoadTexture("./assets/asteroid.png");
    Texture2D shotSprite = LoadTexture("./assets/shot.png");

    Animation explosion;
    initAnimation(&explosion, "./assets/explosion.png", "./assets/explosion.json", 24.0f, (Vector2){EXPLOSION_SIZE, EXPLOSION_SIZE}, false);

    Sound explosionSample = LoadSound("./assets/samples/explosion.wav");
    Sound shotSample = LoadSound("./assets/samples/laser_pew.wav");
    
    Ship ship;
    ship.sprite = LoadTexture("./assets/ship.png");
    resetShip(&ship);

    while(!WindowShouldClose())
    {

        if (IsKeyPressed(KEY_ESCAPE)) {
            isPaused = !isPaused;
        }

        if (faderArgs.fadeComplete && !isPaused) {
            resetTimeBonusMultiplier(player);
            updateLevelBonus(player);
            handleBonuses(&bonuses, player);
            clearShots(&shotsObjectPool);
            handleShooting(&ship, &shotsObjectPool, &shotSample);
            handleShipControls(&ship);
            handleShield(&ship, player);
            handleAsteroidsMovement(&asteroidObjectPool);
            handleShotsMovement(&shotsObjectPool);
            handleAsteroidCollisions(&asteroidObjectPool, &destroyedAsteroidsObjectPool, &shotsObjectPool, &explosionPool, &explosion, &explosionSample, &ship, player);
            handleDestroyedAsteroids(&asteroidObjectPool, &destroyedAsteroidsObjectPool);
            handleBonusesCollisions(&shotsObjectPool, &bonuses, player);
            handleFinishedAnimations(&explosionPool);
            updateAnimationPool(&explosionPool);

            if (ship.isShieldActive) {
                updateShieldAnimation();
            }
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
        }

        if (ship.destroyed) {
            player->lives--;

            if (player->lives < 0) {
                faderArgs.fadeIn = false;
                exit = true;
            } else {
                resetShip(&ship);
                resetAllAsteroids(&asteroidObjectPool);
            }
        } else if (asteroidObjectPool.activeCount == 0) {
            faderArgs.fadeIn = false;
            exit = true;
        }
        
        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexturePro(
                ship.sprite,
                (Rectangle){0, 0, ship.sprite.width, ship.sprite.height},
                (Rectangle){ship.position.x, ship.position.y, SHIP_SIZE, SHIP_SIZE},
                (Vector2){ SHIP_SIZE / 2.0f, SHIP_SIZE / 2.0f},
                ship.rotation,
                WHITE
            );
            renderShield(&ship);
            renderAsteroids(&asteroidObjectPool, &asteroidSprite);
            renderShots(&shotsObjectPool, &shotSprite);
            renderBonuses(&bonuses);
            renderAnimationPool(&explosionPool);
            renderSidebars(player);
            
            if (isPaused) drawPausMenu(&pauseMenu);
            fader(&faderArgs);
            
        EndDrawing();

        if (faderArgs.fadeComplete && exit) break;
    }

    UnloadTexture(asteroidSprite);
    UnloadTexture(ship.sprite);
    UnloadTexture(shotSprite);
    unloadAnimation(&explosion);
    UnloadSound(explosionSample);

    if (WindowShouldClose()) result = EXIT_TO_DESKTOP;
    return result;
}