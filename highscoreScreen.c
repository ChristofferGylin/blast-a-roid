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

    // if (!checkHighscore(ctx)) return;

    float fadeInValue = 1.0f;
    float fadeOutValue = 0.0f;
    bool isFadeInComplete = false;
    bool isFadeOutComplete = false;
    bool exit = false;

    char title[] = "CONGRATULATIONS!";
    char text1[] = "YOU MADE THE HIGHSCORE LIST";
    char text2[] = "ENTER YOUR NAME:";
    char inputText[33] = "Players Name";
    int titleFontSize = 96;
    int textFontSize = 36;
    int inputFontSize = 24;
    int titleFontSpacing = 6;
    int textFontSpacing = 4;
    int gap = 30; 

    Vector2 inputMargin = {12, 20};

    Vector2 titleSize = MeasureTextEx(GetFontDefault(), title, titleFontSize, titleFontSpacing);
    Vector2 text1Size = MeasureTextEx(GetFontDefault(), text1, textFontSize, textFontSpacing);
    Vector2 text2Size = MeasureTextEx(GetFontDefault(), text2, textFontSize, textFontSpacing);
    Vector2 inputTextSize = MeasureTextEx(GetFontDefault(), "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA", inputFontSize, textFontSpacing);

    Rectangle inputBox;

    inputBox.width = inputTextSize.x + inputMargin.x;
    inputBox.height = inputTextSize.y + inputMargin.y;

    int totalHeight = titleSize.y + text1Size.y + text2Size.y + inputBox.height + (gap * 5);   

    Vector2 titlePosition = {SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) - (totalHeight / 2)};
    Vector2 text1Position = {SCREEN_WIDTH / 2, titlePosition.y + titleSize.y + (gap * 2)};
    Vector2 text2Position = {SCREEN_WIDTH / 2, text1Position.y + text1Size.y + gap};

    inputBox.x = (SCREEN_WIDTH / 2 - (inputBox.width / 2));
    inputBox.y = text2Position.y + (text2Size.y) + (gap * 2);

    Vector2 inputTextPosition = {inputBox.x + inputMargin.x, inputBox.y + (inputBox.height / 2)};

    Vector2 titleOrigin = {titleSize.x / 2, 0}; 
    Vector2 text1Origin = {text1Size.x / 2, 0};
    Vector2 text2Origin = {text2Size.x / 2, 0};
    Vector2 inputTextOrigin = {0, inputTextSize.y / 2};

    float inputBoxRoundness = getRoundness(inputBox, 8.0f);
    int inputBoxsegments = 10;

    while(!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(BLACK);
            DrawTextPro(GetFontDefault(), title, titlePosition, titleOrigin, 0, titleFontSize, titleFontSpacing, primaryColor);
            DrawTextPro(GetFontDefault(), text1, text1Position, text1Origin, 0, textFontSize, textFontSpacing, primaryColor);
            DrawTextPro(GetFontDefault(), text2, text2Position, text2Origin, 0, textFontSize, textFontSpacing, primaryColor);

            DrawRectangleRounded(inputBox, inputBoxRoundness, inputBoxsegments, primaryColorDimmed);
            DrawRectangleRoundedLinesEx(inputBox, inputBoxRoundness, inputBoxsegments, 2, primaryColor);
            DrawTextPro(GetFontDefault(), inputText, inputTextPosition, inputTextOrigin, 0, inputFontSize, textFontSpacing, primaryColor);
            
            if (!isFadeInComplete) {
                isFadeInComplete = fadeIn(&fadeInValue);
            } else if (exit && !isFadeOutComplete) {
                isFadeOutComplete = fadeOut(&fadeOutValue);
            }

        EndDrawing();

        if (exit && isFadeOutComplete) break;
    }
}