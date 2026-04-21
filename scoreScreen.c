#include "raylib.h"
#include <stdbool.h>
#include "fader.h"
#include "player.h"
#include "constants.h"
#include <inttypes.h>
#include "bonuses.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "scoreScreen.h"

#define HEADING_FONT_SIZE 48
#define FONT_SPACING 6
#define SCORE_FONT_SIZE 32
#define GAP 10
#define TEXT_BLOCK_HEIGHT 84
#define STEP_DELAY 0.1f
#define MULTIPLY_DELAY 0.5f
#define NEXT_PHASE_DELAY 1
#define EXIT_DELAY 1

void bonusCountUp(uint64_t* src, uint64_t* target, BonusCountUpState* state) {
    
    state->isZeroPadded = false;
    
    if (state->bonusBuffer == 0) {

        if (state->bonusBufferMultiplier == 0) {
            state->bonusBufferMultiplier = 1;
        } else {
            state->bonusBufferMultiplier = state->bonusBufferMultiplier * 10;
        }

        state->bonusBuffer = *src % 10;
        *src = *src / 10;

        if (state->bonusBuffer == 0) {
            state->isZeroPadded = true;
            return;
        }
    }

    *target += state->bonusBufferMultiplier;
    state->bonusBuffer--;
}

void bonusCountDown(uint64_t* src, uint64_t* target) {

    int scoreCountSteps[] = {100000, 10000, 1000, 100, 10, 1};
    int stepCount = sizeof(scoreCountSteps) / sizeof(scoreCountSteps[0]);
    
    for (int i = 0; i < stepCount; i++) {
        if (*src >= scoreCountSteps[i]) {

            int step = scoreCountSteps[i];

            *target += step;
            *src -= step;
            break;
        }
    }
}

int renderScoreLine(uint64_t value, char title[], int startY, bool hasUnderline, int bonusMultiplierLevel, bool isZeroPadded) {
    
    int gap = 200;
    int underlineGap = 10;

    char valueText[32];

    if (isZeroPadded) {
        snprintf(valueText, sizeof(valueText), "%01" PRIu64, value);
    } else {
        snprintf(valueText, sizeof(valueText), "%" PRIu64, value);
    }

    Vector2 valueSize = MeasureTextEx(GetFontDefault(), valueText, SCORE_FONT_SIZE, FONT_SPACING);
    Vector2 titleSize = MeasureTextEx(GetFontDefault(), title, SCORE_FONT_SIZE, FONT_SPACING);

    Vector2 valuePosition = {
        (SCREEN_WIDTH / 2) + gap - valueSize.x,
        startY
    };

    Vector2 titlePosition = {
        (SCREEN_WIDTH / 2) - gap,
        startY
    };

    Vector2 origin = {0, 0};

    DrawTextPro(GetFontDefault(), title, titlePosition, origin, 0, SCORE_FONT_SIZE, FONT_SPACING, RAYWHITE);
    DrawTextPro(GetFontDefault(), valueText, valuePosition, origin, 0, SCORE_FONT_SIZE, FONT_SPACING, RAYWHITE);

    if (bonusMultiplierLevel > 1) {
        Vector2 multiplierIconPosition = {
            valuePosition.x + valueSize.x + 15,
            valuePosition.y + BONUS_MULTIPLIER_RADIUS
        };

        renderBonusMultiplier(bonusMultiplierLevel, multiplierIconPosition);
    }

    if (hasUnderline) {
        float underlineY = titlePosition.y + titleSize.y + underlineGap;
        float underLineStartX = titlePosition.x;
        float underLineEndX = valuePosition.x + valueSize.x;
        DrawLine(underLineStartX, underlineY, underLineEndX, underlineY, RAYWHITE);

        return underlineY;
    }

    return valuePosition.y + valueSize.y;
}

void scoreScreen(Player* player) {
    
    FaderArgs faderArgs;
    initFaderArgs(&faderArgs);

    uint64_t displayBonus = 0;
    uint64_t levelBonus = player->levelBonus;
    uint64_t levelBonusMultiplied = player->levelBonus * player->powerups.levelBonusMultiplier;
    uint64_t displayScore = player->score;

    double lastUpdate = GetTime();
    float delayTime = 0.1f;
    int levelBonusMultiplier = player->powerups.levelBonusMultiplier;
    int currentMultiplier = 1;

    BonusCountUpState bonusCountUpState = {0};

    bool isWaiting = false;
    bool isCountUpFinished = false;
    bool isMultiplyFinished = false;
    bool isCountDownFinished = false;
    bool exit = false;

    char headingText[18];
    snprintf(headingText, sizeof(headingText), "LEVEL %d CLEARED", player->level);

    Vector2 headingSize = MeasureTextEx(GetFontDefault(), headingText, HEADING_FONT_SIZE, FONT_SPACING);
    Vector2 headingPosition = {(SCREEN_WIDTH - headingSize.x) / 2, (SCREEN_HEIGHT / 2) - (TEXT_BLOCK_HEIGHT * 2)}; 
    Vector2 headingOrigin = {0, 0};

    player->score += levelBonusMultiplied;
    player->level++;
    player->timeBonusMultiplier = 1;
    player->powerups.levelBonusMultiplier = 1;

    while (!WindowShouldClose()) {

        if (faderArgs.fadeComplete && faderArgs.fadeIn && !isWaiting) {

            if (!isCountUpFinished) {

                delayTime = STEP_DELAY;
                isWaiting = true;
                lastUpdate = GetTime();

                bonusCountUp(&levelBonus, &displayBonus, &bonusCountUpState);

                if (displayBonus == player->levelBonus) {
                    isCountUpFinished = true;
                    bonusCountUpState.bonusBufferMultiplier = 0;
                    delayTime = NEXT_PHASE_DELAY;
                }

            } else if (!isMultiplyFinished) {
                delayTime = MULTIPLY_DELAY;
                isWaiting = true;
                lastUpdate = GetTime();

                if (levelBonusMultiplier == 1) {
                    isMultiplyFinished = true;
                } else {
                    currentMultiplier++;
                    displayBonus = player->levelBonus * currentMultiplier;

                    if (currentMultiplier == levelBonusMultiplier) {
                        isMultiplyFinished = true;
                        delayTime = NEXT_PHASE_DELAY;
                    }
                }
            } else if (!isCountDownFinished) {
                delayTime = STEP_DELAY;
                isWaiting = true;
                lastUpdate = GetTime();

                bonusCountDown(&displayBonus, &displayScore);

                if (displayBonus == 0) {
                    isCountDownFinished = true;
                    delayTime = EXIT_DELAY;
                }
            } else {
                faderArgs.fadeIn = false;
            }

        } else if (isWaiting) {
            
            if (GetTime() >= lastUpdate + delayTime) {
                isWaiting = false;
            }

        } else if (faderArgs.fadeComplete && !faderArgs.fadeIn) {
            exit = true;
        }

        int yOffset = (SCREEN_HEIGHT / 2) - (TEXT_BLOCK_HEIGHT / 2);

        BeginDrawing();
            ClearBackground(BLACK);
            DrawTextPro(GetFontDefault(), headingText, headingPosition, headingOrigin, 0, HEADING_FONT_SIZE, FONT_SPACING, RAYWHITE);
            yOffset = renderScoreLine(displayBonus, "BONUS:", yOffset, true, currentMultiplier, bonusCountUpState.isZeroPadded);
            yOffset = yOffset + GAP;
            yOffset = renderScoreLine(displayScore, "SCORE:", yOffset, false, 0, false);
            fader(&faderArgs);
        EndDrawing();

        if (exit) break;
    }
}   