#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fader.h"
#include "player.h"
#include "constants.h"

void gameOver(Player* player) {

    FaderArgs faderArgs;
    initFaderArgs(&faderArgs);
    bool exit = false;
    int waitTime = 5;
    double timer = GetTime();

    char text[] = "GAME OVER";
    int fontSize = 96;
    int fontSpacing = 6;

    Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, fontSpacing);
    Vector2 textPosition = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2}; 
    Vector2 textOrigin = {textSize.x / 2, textSize.y / 2}; 
    while(!WindowShouldClose())
    {
        if (faderArgs.fadeComplete && faderArgs.fadeIn) {
            if (timer + waitTime <= GetTime()) {
                faderArgs.fadeIn = false;
            }
        }  else if (faderArgs.fadeComplete && !faderArgs.fadeIn) {
            exit = true;
        }

        BeginDrawing();
            ClearBackground(BLACK);
            DrawTextPro(GetFontDefault(), "GAME OVER", textPosition, textOrigin, 0, fontSize, fontSpacing, RAYWHITE);
            fader(&faderArgs);            
        EndDrawing();

        if (exit) break;
    }
}