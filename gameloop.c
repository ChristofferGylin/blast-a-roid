#include "raylib.h"
#include "gameloop.h"
#include "ship.h"
#include <math.h>

void gameLoop(int level) {

    Ship ship = {
        LoadTexture("./assets/ship.png"),
        { GetScreenWidth() / 2, GetScreenHeight() / 2 },
        {0.0f,0.0f},
        0.0f
    };

    while(!WindowShouldClose())
    {
        handleShipMovement(&ship);

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
        EndDrawing();
    }

    UnloadTexture(ship.sprite);
}