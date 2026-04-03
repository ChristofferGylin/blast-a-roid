#include "raylib.h"
#include "gameloop.h"
#include "asteroid.h"
#include "ship.h"
#include <math.h>
#include "constants.h"

void gameLoop(Player* player) {

    AsteroidArray asteroids;

    initAsteroidArray(&asteroids, 3);
    initAsteroids(&asteroids, 3);

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
                resetAllAsteroids(&asteroids);
            }
        }

        handleCollisions(&asteroids, &ship);
        handleShipMovement(&ship);
        handleAsteroidsMovement(&asteroids);

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

            for (int i = 0; i < asteroids.size; i++) {
                Asteroid* ast = &asteroids.data[i];

                DrawTexturePro(
                    asteroidSprite,
                    (Rectangle){0, 0, asteroidSprite.width, asteroidSprite.height},
                    (Rectangle){ast->position.x, ast->position.y, asteroidSprite.width, asteroidSprite.height},
                    (Vector2){asteroidSprite.width / 2.0f, asteroidSprite.height / 2.0f},
                    ast->rotation,
                    WHITE  
                );
            }
        EndDrawing();
    }

    UnloadTexture(ship.sprite);
    freeAsteroidArray(&asteroids);
}