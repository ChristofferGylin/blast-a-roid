#include "raylib.h"
#include "gameloop.h"

void gameLoop(int level) {

    Texture2D shipSprite = LoadTexture("ship.png");

    while(!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(BLACK);
        EndDrawing();
    }
}