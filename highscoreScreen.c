#include "colors.h"
#include "drawing.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fader.h"
#include "player.h"
#include "constants.h"
#include "score.h"
#include <string.h>
#include "gameContext.h"
#include "gameloop.h"

GameResult highscoreScreen(GameContext* ctx) {

    GameResult result = EXIT_TO_MENU;
    NewHighscore newHighscoreResult = checkHighscore(ctx);
    if (!newHighscoreResult.hasNewHighscore) return result;

    float fadeInValue = 1.0f;
    float fadeOutValue = 0.0f;
    bool isFadeInComplete = false;
    bool isFadeOutComplete = false;
    bool exit = false;

    char title[] = "CONGRATULATIONS!";
    char text1[] = "YOU MADE THE HIGHSCORE LIST";
    char text2[] = "ENTER YOUR NAME:";
    char buttonText[] = "ENTER";
    char inputText[MAX_NAME_LENGTH + 1] = "\0";
    int buttonPadding = 10;
    int letterCount = 0;
    int titleFontSize = 96;
    int textFontSize = 36;
    int inputFontSize = 24;
    int buttonFontSize = 20;
    int titleFontSpacing = 6;
    int textFontSpacing = 4;
    int gap = 30; 
    int gapX = 10; 

    Vector2 inputMargin = {12, 20};

    Vector2 titleSize = MeasureTextEx(GetFontDefault(), title, titleFontSize, titleFontSpacing);
    Vector2 text1Size = MeasureTextEx(GetFontDefault(), text1, textFontSize, textFontSpacing);
    Vector2 text2Size = MeasureTextEx(GetFontDefault(), text2, textFontSize, textFontSpacing);
    Vector2 buttonTextSize = MeasureTextEx(GetFontDefault(), buttonText, buttonFontSize, textFontSpacing);
    Vector2 inputTextSizeMax = MeasureTextEx(GetFontDefault(), "AAAAAAAAAAAAAAAA", inputFontSize, textFontSpacing);

    Rectangle inputBox;
    Rectangle button;

    inputBox.width = inputTextSizeMax.x + (inputMargin.x * 2);
    inputBox.height = inputTextSizeMax.y + inputMargin.y;

    button.height = inputBox.height;
    button.width = buttonTextSize.x + (buttonPadding * 2);

    int totalInputWidth = inputBox.width + gapX + button.width;
    int totalHeight = titleSize.y + text1Size.y + text2Size.y + inputBox.height + (gap * 5);   

    Vector2 titlePosition = {SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) - (totalHeight / 2)};
    Vector2 text1Position = {SCREEN_WIDTH / 2, titlePosition.y + titleSize.y + (gap * 2)};
    Vector2 text2Position = {SCREEN_WIDTH / 2, text1Position.y + text1Size.y + gap};

    inputBox.x = (SCREEN_WIDTH / 2 - (totalInputWidth / 2));
    inputBox.y = text2Position.y + (text2Size.y) + (gap * 2);

    button.x = inputBox.x + inputBox.width + gapX;
    button.y = inputBox.y;

    Vector2 inputTextPosition = {inputBox.x + inputMargin.x, inputBox.y + (inputBox.height / 2)};
    Vector2 buttonTextPosition = {button.x + (button.width / 2) - (buttonTextSize.x / 2), button.y + (button.height / 2) - (buttonTextSize.y / 2)};
    Vector2 origin = {0,0};
    Vector2 titleOrigin = {titleSize.x / 2, 0}; 
    Vector2 text1Origin = {text1Size.x / 2, 0};
    Vector2 text2Origin = {text2Size.x / 2, 0};
    Vector2 inputTextOrigin = {0, inputTextSizeMax.y / 2};

    float inputBoxRoundness = getRoundness(inputBox, 8.0f);
    float buttonRoundness = getRoundness(button, 8.0f);
    int inputBoxsegments = 10;

    bool isCursorVisible = true;
    float cursorBlinkTimer = 0.0f;
    float cursorBlinkDelay = 0.5f;
    int cursorLineThickness = 2;
    int cursorGap = 3;

    bool isButtonHovered = false;

    while(!WindowShouldClose())
    {
        updateRendering(&ctx->rendering);

        if (!exit) {

            cursorBlinkTimer += GetFrameTime();

            if (cursorBlinkTimer >= cursorBlinkDelay) {
                isCursorVisible = !isCursorVisible;
                cursorBlinkTimer = 0.0f;
            }

            int key = GetCharPressed();
            bool submit = false;

            while (key > 0) {
                
                if ((key >= 32) && (key <= 125) && (letterCount < MAX_NAME_LENGTH)) {
                    inputText[letterCount] = (char)key;
                    inputText[letterCount + 1] = '\0';
                    letterCount++;
                }

                key = GetCharPressed();
            }

            if (CheckCollisionPointRec(getVirtualMousePosition(), button) && letterCount > 0) {
                isButtonHovered = true;
                SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
                if (IsMouseButtonPressed(0)) {
                    submit = true;
                }
                
            } else {
                isButtonHovered = false;
                SetMouseCursor(MOUSE_CURSOR_DEFAULT);
            }

            if (IsKeyPressed(KEY_BACKSPACE)) {
                letterCount--;
                if (letterCount < 0) letterCount = 0;
                inputText[letterCount] = '\0';
            } else if ((IsKeyPressed(KEY_ENTER) && letterCount > 0) || submit) {
            
                Highscore newHighscore;
            
                newHighscore.level = ctx->player.level;
                newHighscore.score = ctx->player.score;

                strcpy(newHighscore.name, inputText);

                addHighscore(ctx->highscores.scores, newHighscore);

                ctx->highscores.hasNewHighscore = true;
                ctx->highscores.latestScoreIndex = newHighscoreResult.scoreIndex;
                
                isCursorVisible = false;

                SetMouseCursor(MOUSE_CURSOR_DEFAULT);

                exit = true;
            }
        }

        Vector2 inputTextSize = MeasureTextEx(GetFontDefault(), inputText, inputFontSize, textFontSpacing);
        Vector2 cursorStartPos = {inputTextPosition.x + inputTextSize.x + textFontSpacing, inputTextPosition.y - (inputTextSizeMax.y / 2)};
        Vector2 cursorEndPos = {cursorStartPos.x, inputTextPosition.y + (inputTextSizeMax.y / 2)};

        Color buttonColor = isButtonHovered ? primaryColorDimmed60 : primaryColorDimmed50;
        Color buttonBorderColor = primaryColor;
        Color buttonTextColor = primaryColor;

        BeginTextureMode(ctx->rendering.renderTexture);
            ClearBackground(BLACK);
            DrawTextPro(GetFontDefault(), title, titlePosition, titleOrigin, 0, titleFontSize, titleFontSpacing, primaryColor);
            DrawTextPro(GetFontDefault(), text1, text1Position, text1Origin, 0, textFontSize, textFontSpacing, primaryColor);
            DrawTextPro(GetFontDefault(), text2, text2Position, text2Origin, 0, textFontSize, textFontSpacing, primaryColor);

            DrawRectangleRounded(button, buttonRoundness, inputBoxsegments, buttonColor);
            DrawRectangleRoundedLinesEx(button, buttonRoundness, inputBoxsegments, 2, buttonBorderColor);

            DrawTextPro(GetFontDefault(), buttonText, buttonTextPosition, origin, 0, buttonFontSize, textFontSpacing, buttonTextColor);

            DrawRectangleRounded(inputBox, inputBoxRoundness, inputBoxsegments, primaryColorDimmed30);
            DrawRectangleRoundedLinesEx(inputBox, inputBoxRoundness, inputBoxsegments, 2, primaryColor);
            DrawTextPro(GetFontDefault(), inputText, inputTextPosition, inputTextOrigin, 0, inputFontSize, textFontSpacing, primaryColor);
            
            if (isCursorVisible) {
                DrawLineEx(cursorStartPos, cursorEndPos, cursorLineThickness, primaryColor);
            }
            
            if (!isFadeInComplete) {
                isFadeInComplete = fadeIn(&fadeInValue);
            } else if (exit && !isFadeOutComplete) {
                isFadeOutComplete = fadeOut(&fadeOutValue);
            }

        EndTextureMode();

        renderToScreen(&ctx->rendering);

        if (exit && isFadeOutComplete) break;
    }

    if (WindowShouldClose()) result = EXIT_TO_DESKTOP;

    return result;
}