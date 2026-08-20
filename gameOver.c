#include "colors.h"
#include "drawing.h"
#include "gameContext.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fader.h"
#include "player.h"
#include "constants.h"

void gameOver(GameContext* ctx) {

    HideCursor();

    Player* player = &ctx->player;

    float fadeInValue = 1.0f;
    float fadeOutValue = 0.0f;
    bool isFadeInComplete = false;
    bool isFadeOutComplete = false;
    bool exit = false;
    int waitTime = 2;
    double timer = GetTime();

    char text[] = "GAME OVER";
    int fontSize = 96;
    int fontSpacing = 6;

    Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, fontSpacing);
    Vector2 textPosition = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2}; 
    Vector2 textOrigin = {textSize.x / 2, textSize.y / 2}; 
    while(!WindowShouldClose())
    {

        updateRendering(&ctx->rendering);

        if (isFadeInComplete) {
            if (timer + waitTime <= GetTime()) {
                exit = true;
            }
        }

        BeginTextureMode(ctx->rendering.renderTexture);
            ClearBackground(BLACK);
            DrawTextPro(GetFontDefault(), "GAME OVER", textPosition, textOrigin, 0, fontSize, fontSpacing, primaryColor);
            
            if (!isFadeInComplete) {
                isFadeInComplete = fadeIn(&fadeInValue);
            } else if (exit && !isFadeOutComplete) {
                isFadeOutComplete = fadeOut(&fadeOutValue);
            }

        EndTextureMode();

        renderToScreen(&ctx->rendering);

        if (exit && isFadeOutComplete) break;
    }
}