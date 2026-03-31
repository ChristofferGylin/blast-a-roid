#include "raylib.h"
#include "gameloop.h"
#include <math.h>
#include "utils.h"

void gameLoop(int level) {

    const float ROTATION_SPEED = 150.0f;

    Texture2D shipSprite = LoadTexture("./assets/ship.png");

    Vector2 shipPosition = { GetScreenWidth() / 2, GetScreenHeight() / 2 };
    float shipRotation = 0.0f;

    while(!WindowShouldClose())
    {
        if (IsKeyDown(KEY_A))
        {
            shipRotation -= GetFrameTime() * ROTATION_SPEED;
        }

        if (IsKeyDown(KEY_D))
        {
            shipRotation += GetFrameTime() * ROTATION_SPEED;
        }

        shipRotation = fmodf(shipRotation, 360.0f);

        if (shipRotation < 0.0f) {
            shipRotation += 360.0f;
        }

        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexturePro(
                shipSprite,
                (Rectangle){0, 0, shipSprite.width, shipSprite.height},
                (Rectangle){shipPosition.x, shipPosition.y, shipSprite.width, shipSprite.height},
                (Vector2){ shipSprite.width / 2.0f, shipSprite.height / 2.0f},
                shipRotation,
                WHITE
            );
        EndDrawing();
    }

    UnloadTexture(shipSprite);
}