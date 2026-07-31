#include "drawing.h"

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