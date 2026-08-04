#include "constants.h"
#include "drawing.h"
#include "raylib.h"

Vector2 getRenderSize(float aspectWidth, float aspectHeight) {

    Vector2 screenSize = {0};

    float screenWidth = GetScreenWidth();
    float screenHeight = GetScreenHeight();

    screenSize.y = screenHeight;
    screenSize.x = (screenHeight * aspectWidth) / aspectHeight;

    if (screenSize.x > screenWidth) {
        screenSize.x = screenWidth;
        screenSize.y = (screenWidth * aspectHeight) / aspectWidth;
    }

    return screenSize;
}

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