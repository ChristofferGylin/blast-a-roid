#include "raylib.h"
#include "gameloop.h"
#include <math.h>

void gameLoop(int level) {

    const float ROTATION_SPEED = 270.0f;
    const float THRUST_FACTOR = 2.5f;
    const float MAX_VELOCITY = 300.0f;

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

        if (IsKeyDown(KEY_W))
        {
            float radians = (shipRotation - 90.0f) * (PI / 180.0f);

            shipVelocity.x += cosf(radians) * THRUST_FACTOR;
            shipVelocity.y += sinf(radians) * THRUST_FACTOR;

            if (shipVelocity.x < -MAX_VELOCITY)
            {
                shipVelocity.x = -MAX_VELOCITY;
            } else if (shipVelocity.x > MAX_VELOCITY)
            {
                shipVelocity.x = MAX_VELOCITY;
            }

            if (shipVelocity.y < -MAX_VELOCITY)
            {
                shipVelocity.y = -MAX_VELOCITY;
            } else if (shipVelocity.y > MAX_VELOCITY)
            {
                shipVelocity.y = MAX_VELOCITY;
            }
        }

        shipPosition.x += GetFrameTime() * shipVelocity.x;
        shipPosition.y += GetFrameTime() * shipVelocity.y;

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