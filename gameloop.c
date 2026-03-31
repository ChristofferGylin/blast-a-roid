#include "raylib.h"
#include "gameloop.h"
#include <math.h>
#include "utils.h"

void gameLoop(int level) {

    const float ROTATION_SPEED = 150.0f;
    const float THRUST_FACTOR = 100.0f;
    const float MAX_VELOCITY = 100.0f;

    Texture2D shipSprite = LoadTexture("./assets/ship.png");

    Vector2 shipPosition = { GetScreenWidth() / 2, GetScreenHeight() / 2 };
    Vector2 shipVelocity = {0,0};
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

        if (shipRotation < 0.0f)
        {
            shipRotation += 360.0f;
        }

        if (shipPosition.x < 0.0f - shipSprite.width)
        {
            shipPosition.x = GetScreenWidth() + shipSprite.width;
        } else if (shipPosition.x > GetScreenWidth() + shipSprite.width)
        {
            shipPosition.x = 0.0f - shipSprite.width; 
        }

        if (shipPosition.y < 0.0f - shipSprite.height)
        {
            shipPosition.y = GetScreenHeight() + shipSprite.height;
        } else if (shipPosition.y > GetScreenHeight() + shipSprite.height)
        {
            shipPosition.y = 0.0f - shipSprite.height; 
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