#include "constants.h"
#include "drawing.h"
#include "raylib.h"

void renderToScreen(RenderTexture2D renderTexture) {
    BeginDrawing();

        Rectangle src = {
            0,
            0,
            SCREEN_WIDTH,
            -SCREEN_HEIGHT
        };

        Rectangle dst = {
            0,
            0,
            GetScreenWidth(),
            GetScreenHeight()
        };

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