#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "fader.h"
#include "player.h"
#include "constants.h"
#include <inttypes.h>

int renderScoreLine(uint64_t value, char title[], int startY) {
    
    int sideOffset = 20;
    int fontSize = 22;
    int fontSpacing = 4;
    int gap = 100;
    int underlineGap = 2;

    char valueText[32];
    snprintf(valueText, sizeof(valueText), "%" PRIu64, value);

    Vector2 valueSize = MeasureTextEx(GetFontDefault(), valueText, fontSize, fontSpacing);
    Vector2 titleSize = MeasureTextEx(GetFontDefault(), title, fontSize, fontSpacing);

    Vector2 valuePosition = {
        (SCREEN_WIDTH / 2) + gap - valueSize.x,
        startY
    };

    Vector2 titlePosition = {
        (SCREEN_WIDTH / 2) - titleSize.x,
        startY
    };

    Vector2 origin = {0, 0};

    float underlineY = titlePosition.y + titleSize.y + underlineGap;
    float underLineStartX = titlePosition.x - (titleSize.x / 2);
    float underLineEndX = titlePosition.x + (titleSize.x / 2);

    DrawTextPro(GetFontDefault(), title, titlePosition, origin, 0, fontSize, fontSpacing, RAYWHITE);
    DrawLine(titlePosition.x, underlineY, valuePosition.x + valueSize.x, underlineY, RAYWHITE);
    DrawTextPro(GetFontDefault(), valueText, valuePosition, origin, 0, fontSize, fontSpacing, RAYWHITE);

    return valuePosition.y + valueSize.y;
}

void scoreScreen(Player* player) {

    uint64_t bonus = player->levelBonus;
    uint64_t score = player->score;

    player->score += bonus;
    player->level++;
    player->timeBonusMultiplier = 1;
    player->powerups.levelBonusMultiplier = 1;

    bool fadeIn = true;
    bool fadeComplete = false;
    bool waiting = false;
    bool exit = false;
    float lastUpdate = GetTime();
    const int WAIT_TIME = 5;
    const float UPDATE_WAIT_TIME = 0.1f;
    double timer = GetTime();

    char text[] = "GAME OVER";
    int fontSize = 96;
    int fontSpacing = 6;

    Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, fontSpacing);
    Vector2 textPosition = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2}; 
    Vector2 textOrigin = {textSize.x / 2, textSize.y / 2}; 
    while(!WindowShouldClose())
    {
        int yOffset = SCREEN_HEIGHT / 2;

        if (fadeComplete && fadeIn && !waiting) {
            
            float currentTime = GetTime();

             if ((currentTime >= lastUpdate + UPDATE_WAIT_TIME) && bonus > 0) {
                lastUpdate = currentTime;

                if (bonus > 100) {
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
                fadeIn = false;
                fadeComplete = false;
                waiting = false;
            }
            
        } else if (fadeComplete && !fadeIn) {
            printf("fadeComplete && !fadeIn\n");
            exit = true;
        }

        BeginDrawing();
            ClearBackground(BLACK);
            yOffset = renderScoreLine(bonus, "BONUS:", yOffset);
            yOffset = renderScoreLine(score, "SCORE:", yOffset);
            fadeComplete = fader(fadeIn);
        EndDrawing();

        if (exit) break;
    }
}