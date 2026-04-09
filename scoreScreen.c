#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fader.h"
#include "player.h"
#include "constants.h"
#include <inttypes.h>

int numberPrinted = 0;

int renderScoreLine(uint64_t value, char title[], int startY, bool hasUnderline) {
    
    int sideOffset = 20;
    int fontSize = 32;
    int fontSpacing = 6;
    int gap = 200;
    int underlineGap = 10;

    char valueText[32];
    snprintf(valueText, sizeof(valueText), "%" PRIu64, value);

    Vector2 valueSize = MeasureTextEx(GetFontDefault(), valueText, fontSize, fontSpacing);
    Vector2 titleSize = MeasureTextEx(GetFontDefault(), title, fontSize, fontSpacing);

    Vector2 valuePosition = {
        (SCREEN_WIDTH / 2) + gap - valueSize.x,
        startY
    };

    Vector2 titlePosition = {
        (SCREEN_WIDTH / 2) - gap,
        startY
    };

    Vector2 origin = {0, 0};

    DrawTextPro(GetFontDefault(), title, titlePosition, origin, 0, fontSize, fontSpacing, RAYWHITE);
    DrawTextPro(GetFontDefault(), valueText, valuePosition, origin, 0, fontSize, fontSpacing, RAYWHITE);

    if (hasUnderline) {
        float underlineY = titlePosition.y + titleSize.y + underlineGap;
        float underLineStartX = titlePosition.x - (titleSize.x / 2);
        float underLineEndX = titlePosition.x + (titleSize.x / 2);
        DrawLine(titlePosition.x, underlineY, valuePosition.x + valueSize.x, underlineY, RAYWHITE);

        return underlineY;
    }

    return valuePosition.y + valueSize.y;
}

void scoreScreen(Player* player) {

    uint64_t bonus = player->levelBonus;
    uint64_t score = player->score;

    FaderArgs faderArgs;
    initFaderArgs(&faderArgs);
    bool waiting = false;
    bool exit = false;
    float lastUpdate = GetTime();
    const int WAIT_TIME = 2;
    const float UPDATE_WAIT_TIME = 0.1f;
    const int GAP = 10;
    const int TEXT_BLOCK_HEIGHT = 84;
    double timer = GetTime();

    char headingText[18];
    snprintf(headingText, sizeof(headingText), "LEVEL %d CLEARED", player->level);

    int headingFontSize = 48;
    int headingFontSpacing = 6;

    Vector2 headingSize = MeasureTextEx(GetFontDefault(), headingText, headingFontSize, headingFontSpacing);
    Vector2 headingPosition = {(SCREEN_WIDTH - headingSize.x) / 2, (SCREEN_HEIGHT / 2) - (TEXT_BLOCK_HEIGHT * 2)}; 
    Vector2 headingOrigin = {0, 0};

    player->score += bonus;
    player->level++;
    player->timeBonusMultiplier = 1;
    player->powerups.levelBonusMultiplier = 1;

    while(!WindowShouldClose())
    {
        int yOffset = (SCREEN_HEIGHT / 2) - (TEXT_BLOCK_HEIGHT / 2);

        if (faderArgs.fadeComplete && faderArgs.fadeIn && !waiting) {
            
            float currentTime = GetTime();

             if ((currentTime >= lastUpdate + UPDATE_WAIT_TIME) && bonus > 0) {
                lastUpdate = currentTime;
                
                if (bonus > 100000) {
                    bonus -= 100000;
                    score += 100000;
                } else if (bonus > 10000) {
                    bonus -= 10000;
                    score += 10000;
                } else if (bonus > 1000) {
                    bonus -= 1000;
                    score += 1000;
                } else if (bonus > 100) {
                    bonus -= 100;
                    score += 100;
                } else  if (bonus > 10) {
                    bonus -= 10;
                    score += 10;
                } else {
                    bonus--;
                    score++;
                }
                
             }

             if (bonus <= 0) {
                bonus = 0;
                timer = GetTime();
                waiting = true;
             }
        }  else if (waiting) {
            
            if (GetTime() >= timer + WAIT_TIME) {
                faderArgs.fadeIn = false;
                waiting = false;
            }
            
        } else if (faderArgs.fadeComplete && !faderArgs.fadeIn) {
            exit = true;
        }

        BeginDrawing();
            ClearBackground(BLACK);
            DrawTextPro(GetFontDefault(), headingText, headingPosition, headingOrigin, 0, headingFontSize, headingFontSpacing, RAYWHITE);
            yOffset = renderScoreLine(bonus, "BONUS:", yOffset, true);
            yOffset = yOffset + GAP;
            yOffset = renderScoreLine(score, "SCORE:", yOffset, false);
            fader(&faderArgs);
        EndDrawing();

        if (exit) break;
    }
}