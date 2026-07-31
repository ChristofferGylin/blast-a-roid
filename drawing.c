#include "constants.h"
#include "drawing.h"
#include "raylib.h"

void drawGame(Rectangle src, Rectangle dst, RenderTexture2D renderTexture) {
    BeginDrawing();

    DrawTexturePro(
        renderTexture.texture,
        src,
        dst,
        (Vector2){0,0},
        0,
        WHITE
    );

    EndDrawing();
}

void initDrawing(Drawing* drawing) {
    drawing->renderTexture = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    drawing->srcRect = (Rectangle) {
        0,
        0,
        SCREEN_WIDTH,
        -SCREEN_HEIGHT
    };
    drawing->dstRect = (Rectangle) {
        0,
        0,
        GetScreenWidth(),
        GetScreenHeight()
    };
}