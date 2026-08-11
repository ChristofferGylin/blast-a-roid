#include "constants.h"
#include "drawing.h"
#include "raylib.h"

static const float ASPECT_WIDTH = 16.0f;
static const float ASPECT_HEIGHT = 9.0f;

Rectangle getRenderRect(float aspectWidth, float aspectHeight) {

    Rectangle rect = {0};

    float screenWidth = GetScreenWidth();
    float screenHeight = GetScreenHeight();

    rect.width = (screenHeight * aspectWidth) / aspectHeight;
    rect.height = screenHeight;

    if (rect.width > screenWidth) {
        rect.width = screenWidth;
        rect.height = (screenWidth * aspectHeight) / aspectWidth;
    }

    rect.x = (screenWidth / 2.0f) - (rect.width / 2.0f);
    rect.y = (screenHeight / 2.0f) - (rect.height / 2.0f);

    return rect;
}

void initRendering(Rendering* rendering) {
    rendering->srcRect.x = 0;
    rendering->srcRect.y = 0;
    rendering->srcRect.width = SCREEN_WIDTH;
    rendering->srcRect.height = -SCREEN_HEIGHT;

    rendering->dstRect = getRenderRect(ASPECT_WIDTH, ASPECT_HEIGHT);

    rendering->renderTexture = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    SetTextureFilter(rendering->renderTexture.texture, TEXTURE_FILTER_POINT);
}

void renderToScreen(Rendering* rendering) {
    BeginDrawing();
        ClearBackground(BLACK);
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

void updateRendering(Rendering* rendering) {
    if (IsWindowResized()) {
        rendering->dstRect = getRenderRect(ASPECT_WIDTH, ASPECT_HEIGHT);
    }
}