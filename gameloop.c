#include "raylib.h"
#include "gameloop.h"
#include <math.h>
void gameLoop(int level) {

    const float ROTATION_SPEED = 150.0f;

    Texture2D shipSprite = LoadTexture("./assets/ship.png");

    Vector2 shipPos = { GetScreenWidth() / 2, GetScreenHeight() / 2 };
    float shipRot = 0.0f;

    while(!WindowShouldClose())
    {
        if (IsKeyDown(KEY_A))
        {
            shipRot -= GetFrameTime() * ROTATION_SPEED;
        }

        if (IsKeyDown(KEY_D))
        {
            shipRot += GetFrameTime() * ROTATION_SPEED;
        }

        shipRot = fmodf(shipRot, 360.0f);

        if (shipRot < 0.0f) {
            shipRot += 360.0f;
        }

        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexturePro(
                shipSprite,
                (Rectangle){0, 0, shipSprite.width, shipSprite.height},
                (Rectangle){shipPos.x, shipPos.y, shipSprite.width, shipSprite.height},
                (Vector2){ shipSprite.width / 2.0f, shipSprite.height / 2.0f},
                shipRot,
                WHITE
            );
        EndDrawing();
    }

    UnloadTexture(shipSprite);
}