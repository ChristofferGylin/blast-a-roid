#include "raylib.h"
#include "gameloop.h"
#include "asteroid.h"
#include "ship.h"
#include <math.h>

void gameLoop(Player* player) {

    AsteroidArray asteroids;

    initAsteroidArray(&asteroids, 3);
    initAsteroids(&asteroids, 3);

    Texture2D asteroidSprite = LoadTexture("./assets/asteroid.png");

    Ship ship = {
        LoadTexture("./assets/ship.png"),
        { GetScreenWidth() / 2, GetScreenHeight() / 2 },
        {0.0f,0.0f},
        0.0f,
        false
    };

    while(!WindowShouldClose())
    {
        handleCollisions(&asteroids, &ship);
        handleShipMovement(&ship);
        handleAsteroidsMovement(&asteroids);

        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexturePro(
                ship.sprite,
                (Rectangle){0, 0, ship.sprite.width, ship.sprite.height},
                (Rectangle){ship.position.x, ship.position.y, ship.sprite.width, ship.sprite.height},
                (Vector2){ ship.sprite.width / 2.0f, ship.sprite.height / 2.0f},
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