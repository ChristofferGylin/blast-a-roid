#include "raylib.h"
#include "gameloop.h"
#include "asteroid.h"
#include "ship.h"
#include <math.h>
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>

void gameLoop(Player* player) {

    // AsteroidPool* asteroids = malloc(sizeof(AsteroidPool));

    // initAsteroidPool(asteroids);    
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
                resetAllAsteroids(asteroids);
            }
        }

        handleShipMovement(&ship);
        handleAsteroidsMovement(asteroids);
        handleCollisions(asteroids, &ship);
        handleDestroyedAsteroids();

        BeginDrawing();
            ClearBackground(BLACK);
            DrawCircle(ship.position.x, ship.position.y, SHIP_SIZE / 2.0f, RAYWHITE);
            DrawTexturePro(
                ship.sprite,
                (Rectangle){0, 0, ship.sprite.width, ship.sprite.height},
                (Rectangle){ship.position.x, ship.position.y, SHIP_SIZE, SHIP_SIZE},
                (Vector2){ SHIP_SIZE / 2.0f, SHIP_SIZE / 2.0f},
                ship.rotation,
                WHITE
            );

            for (int i = 0; i < asteroids->activeCount; i++) {
                Asteroid* ast = &asteroids->asteroids[asteroids->active[i]];

                int asteroidSize = 0;

                switch (ast->level)
                {
                case 1:
                    asteroidSize = ASTEROID_SIZE_1;
                    break;
                case 2:
                    asteroidSize = ASTEROID_SIZE_2;
                    break;
                case 3:
                    asteroidSize = ASTEROID_SIZE_3;
                    break;
                
                default:
                    printf("Invalid asteroid level %d\n", ast->level);
                    break;
                }

                DrawTexturePro(
                    asteroidSprite,
                    (Rectangle){0, 0, asteroidSprite.width, asteroidSprite.height},
                    (Rectangle){ast->position.x, ast->position.y, asteroidSize, asteroidSize},
                    (Vector2){asteroidSprite.width / 2.0f, asteroidSprite.height / 2.0f},
                    ast->rotation,
                    WHITE  
                );
            }
        EndDrawing();
    }

    UnloadTexture(ship.sprite);
    free(asteroids);
}