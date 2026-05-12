#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fader.h"
#include "player.h"
#include "constants.h"

void gameOver(Player* player) {

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
        if (isFadeInComplete) {
            if (timer + waitTime <= GetTime()) {
                exit = true;
            }
        }

        BeginDrawing();
            ClearBackground(BLACK);
            DrawTextPro(GetFontDefault(), "GAME OVER", textPosition, textOrigin, 0, fontSize, fontSpacing, RAYWHITE);
            
            if (!isFadeInComplete) {
                isFadeInComplete = fadeIn(&fadeInValue);
            } else if (exit && !isFadeOutComplete) {
                isFadeOutComplete = fadeOut(&fadeOutValue);
            }

        EndDrawing();

        if (exit && isFadeOutComplete) break;
    }
}