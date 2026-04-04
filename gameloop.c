#include "raylib.h"
#include "gameloop.h"
#include "asteroid.h"
#include "ship.h"
#include <math.h>
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>

void gameLoop(Player* player) {
   
    initAsteroids(player->level);

    Texture2D asteroidSprite = LoadTexture("./assets/asteroid.png");
    
    Ship ship;
    ship.sprite = LoadTexture("./assets/ship.png");
    resetShip(&ship);

    while(!WindowShouldClose())
    {

        if (ship.destroyed) {
            player->lives--;

            if (player->lives < 0) {
                // game over
            } else {
                resetShip(&ship);
                resetAllAsteroids();
            }
        }

        handleShipMovement(&ship);
        handleAsteroidsMovement();
        handleAsteroidCollisions(&ship);
        handleDestroyedAsteroids();

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
            renderAsteroids(&asteroidSprite);
            
        EndDrawing();
    }

    UnloadTexture(ship.sprite);
}