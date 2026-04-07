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

static AsteroidPool asteroidObjectPool = {0};
static DestroyedAsteroidPool destroyedAsteroidsObjectPool = {0};
static ShotObjectPool shotsObjectPool = {0};

void gameLoop(Player* player) {

    player->levelBonus = (player->level + 1) * 1000;
   
    initShotObjectPool(&shotsObjectPool);
    initAsteroids(&asteroidObjectPool, player->level);

    bool fadeIn = true;
    bool fadeComplete = false;

    Texture2D asteroidSprite = LoadTexture("./assets/asteroid.png");
    Texture2D shotSprite = LoadTexture("./assets/shot.png");
    
    Ship ship;
    ship.sprite = LoadTexture("./assets/ship.png");
    resetShip(&ship);

    while(true)
    {

        if (ship.destroyed) {
            player->lives--;

            if (player->lives < 0) {
                fadeComplete = false;
                fadeIn = false;
            } else {
                resetShip(&ship);
                resetAllAsteroids(&asteroidObjectPool);
            }
        }

        if (fadeComplete) {
            resetTimeBonusMultiplier(player);
            updateLevelBonus(player);
            clearShots(&shotsObjectPool);
            handleShooting(&ship, &shotsObjectPool);
            handleShipMovement(&ship);
            handleAsteroidsMovement(&asteroidObjectPool);
            handleShotsMovement(&shotsObjectPool);
            handleAsteroidCollisions(&asteroidObjectPool, &destroyedAsteroidsObjectPool, &shotsObjectPool, &ship, player);
            handleDestroyedAsteroids(&asteroidObjectPool, &destroyedAsteroidsObjectPool);
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
            renderAsteroids(&asteroidObjectPool, &asteroidSprite);
            renderShots(&shotsObjectPool, &shotSprite);
            renderSidebars(player);
            fadeComplete = fader(fadeIn);
            
        EndDrawing();

        if (fadeComplete && player->lives < 0 || asteroidObjectPool.activeCount == 0) break;
    }

    UnloadTexture(asteroidSprite);
    UnloadTexture(ship.sprite);
    UnloadTexture(shotSprite);
}