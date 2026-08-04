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

void initRendering(Rendering* rendering) {
    rendering->srcRect.x = 0;
    rendering->srcRect.y = 0;
    rendering->srcRect.width = SCREEN_WIDTH;
    rendering->srcRect.height = -SCREEN_HEIGHT;

    Vector2 renderSize = getRenderSize(16.0f, 9.0f);

    rendering->dstRect.x = 0;
    rendering->dstRect.y = 0;
    rendering->dstRect.width = renderSize.x;
    rendering->dstRect.height = renderSize.y;

    rendering->renderTexture = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
}

void renderToScreen(Rendering* rendering) {
    BeginDrawing();
        DrawTexturePro(
            rendering->renderTexture.texture,
            rendering->srcRect,
            rendering->dstRect,
            (Vector2){0,0},
            0,
            WHITE
        );
    EndDrawing();
}