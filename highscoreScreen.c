#include "colors.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fader.h"
#include "player.h"
#include "constants.h"
#include "score.h"

void highscoreScreen(GameContext* ctx) {

    if (!checkHighscore(ctx)) return;

    float fadeInValue = 1.0f;
    float fadeOutValue = 0.0f;
    bool isFadeInComplete = false;
    bool isFadeOutComplete = false;
    bool exit = false;

    char title[] = "CONGRATULATIONS!";
    char text[] = "YOU MADE THE HIGHSCORE LIST\nENTER YOUR NAME:";
    char inputText[33] = "\0";
    int titleFontSize = 96;
    int textFontSize = 36;
    int inputFontSize = 24;
    int titleFontSpacing = 6;
    int textFontSpacing = 4;
    int gap = 30; 

    Vector2 inputMargin = {8, 4};

    Vector2 titleSize = MeasureTextEx(GetFontDefault(), title, titleFontSize, titleFontSpacing);
    Vector2 textSize = MeasureTextEx(GetFontDefault(), text, textFontSize, textFontSpacing);
    Vector2 inputTextSize = MeasureTextEx(GetFontDefault(), "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA", inputFontSize, textFontSpacing);

    Vector2 titlePosition = {SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) - (titleSize.y / 2)};
    Vector2 textPosition = {SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) + (titleSize.y / 2)};

    Vector2 titleOrigin = {titleSize.x / 2, titleSize.y / 2}; 
    Vector2 textOrigin = {textSize.x / 2, textSize.y / 2};

    Rectangle inputBox;

    inputBox.width = inputTextSize.y + inputMargin.x;
    inputBox.height = inputTextSize.y + inputMargin.y;
    inputBox.x = SCREEN_WIDTH / 2;
    inputBox.y = textPosition.y + (textSize.y / 2) + gap;

    while(!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(BLACK);
            DrawTextPro(GetFontDefault(), title, titlePosition, titleOrigin, 0, titleFontSize, titleFontSpacing, primaryColor);
            DrawTextPro(GetFontDefault(), text, textPosition, textOrigin, 0, textFontSize, textFontSpacing, primaryColor);

            DrawRectangleRounded(inputBox, 10, 10, primaryColorDimmed);
            DrawRectangleRoundedLinesEx(inputBox, 10, 10, 2, primaryColor);
            
            if (!isFadeInComplete) {
                isFadeInComplete = fadeIn(&fadeInValue);
            } else if (exit && !isFadeOutComplete) {
                isFadeOutComplete = fadeOut(&fadeOutValue);
            }

        EndDrawing();

        if (exit && isFadeOutComplete) break;
    }
}