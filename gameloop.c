#include "raylib.h"
#include "gameloop.h"

void gameLoop(int level) {

    Texture2D shipSprite = LoadTexture("./assets/ship.png");

    float shipPosX = (GetScreenWidth() / 2) - (shipSprite.width / 2);
    float shipPosY = (GetScreenHeight() / 2) - (shipSprite.height / 2);
    float shipRot = 0;

    while(!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(BLACK);
            DrawTexturePro(
                shipSprite,
                (Rectangle){0, 0, shipSprite.width, shipSprite.height},
                (Rectangle){shipPosX, shipPosY, shipSprite.width, shipSprite.height},
                (Vector2){ shipSprite.width / 2.0f, shipSprite.height / 2.0f},
                shipRot,
                WHITE
            );
        EndDrawing();
    }

    UnloadTexture(shipSprite);
}