#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fader.h"
#include "player.h"
#include "constants.h"

void gameOver(Player* player) {

    bool fadeIn = true;
    bool fadeComplete = false;
    bool exit = false;
    int waitTime = 5;
    double timer = GetTime();

    if (fadeComplete && fadeIn) {
        if (timer + waitTime > GetTime()) {
            fadeIn = false;
            fadeComplete = false;
        }
    } else if (fadeComplete && !fadeIn) {
        exit = true;
    }

    char text[] = "GAME OVER";
    int fontSize = 56;
    int fontSpacing = 6;

    Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, fontSpacing);
    Vector2 textPosition = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2}; 
    Vector2 textOrigin = {textSize.x / 2, textSize.y / 2}; 
    while(!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(BLACK);
            DrawTextPro(GetFontDefault(), "GAME OVER", textPosition, textOrigin, 0, fontSize, fontSpacing, RAYWHITE);
            fadeComplete = fader(fadeIn);            
        EndDrawing();

        if (exit) break;
    }
}