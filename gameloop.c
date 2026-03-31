#include "raylib.h"
#include "gameloop.h"

void gameLoop(int level) {

    Texture2D shipSprite = LoadTexture("./assets/ship.png");

    Vector2 shipPos = { GetScreenWidth() / 2, GetScreenHeight() / 2 };
    float shipRot = 0;

    while(!WindowShouldClose())
    {
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