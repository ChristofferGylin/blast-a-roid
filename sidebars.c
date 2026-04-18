#include "raylib.h"
#include "constants.h"
#include "bonuses.h"
#include "sidebars.h"
#include "player.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>

Color topColor = {0, 25, 38, 255};
Color bottomColor = {0, 13, 36, 255};
Color lineColor = {156, 192, 255, 128};

int renderStats(uint64_t value, char title[], int startY, int bonusMultiplierLevel, bool isMultiplierRendered) {
    
    int sideOffset = 30;
    int fontSize = 22;
    int fontSpacing = 4;
    int gap = 12;
    int underlineGap = 2;

    char valueText[32];
    snprintf(valueText, sizeof(valueText), "%" PRIu64, value);

    Vector2 valueSize = MeasureTextEx(GetFontDefault(), valueText, fontSize, fontSpacing);
    Vector2 titleSize = MeasureTextEx(GetFontDefault(), title, fontSize, fontSpacing);

    Vector2 valuePosition = {
        (SIDEBAR_WIDTH - sideOffset) - valueSize.x,
        startY + titleSize.y + gap
    };

    Vector2 titlePosition = {
        SIDEBAR_WIDTH / 2,
        startY
    };

    Vector2 titleOrigin = {
        titleSize.x / 2,
        0
    };

    Vector2 valueOrigin = {0, 0};

    float underlineY = titlePosition.y + titleSize.y + underlineGap;
    float underLineStartX = titlePosition.x - (titleSize.x / 2);
    float underLineEndX = titlePosition.x + (titleSize.x / 2);

    DrawTextPro(GetFontDefault(), title, titlePosition, titleOrigin, 0, fontSize, fontSpacing, RAYWHITE);
    DrawLine(underLineStartX, underlineY, underLineEndX, underlineY, RAYWHITE);
    DrawTextPro(GetFontDefault(), valueText, valuePosition, valueOrigin, 0, fontSize, fontSpacing, RAYWHITE);

    if (bonusMultiplierLevel > 1 && isMultiplierRendered) {
        Vector2 multiplierIconPosition = {
            SIDEBAR_WIDTH - (sideOffset / 2),
            valuePosition.y + BONUS_MULTIPLIER_RADIUS
        };

        renderBonusMultiplier(bonusMultiplierLevel, multiplierIconPosition);
    }

    return valuePosition.y + valueSize.y;
}

void renderSidebars(Player* player) {
    int startY = 20;
    int statsBlockGap = 25;

    DrawRectangleGradientV(0, 0, SIDEBAR_WIDTH, SCREEN_HEIGHT, topColor, bottomColor);
    DrawRectangleGradientV(SCREEN_WIDTH - SIDEBAR_WIDTH, 0, SIDEBAR_WIDTH, SCREEN_HEIGHT, topColor, bottomColor);
    DrawLine(SIDEBAR_WIDTH, 0, SIDEBAR_WIDTH, SCREEN_HEIGHT, lineColor);
    DrawLine(SCREEN_WIDTH - SIDEBAR_WIDTH, 0, SCREEN_WIDTH - SIDEBAR_WIDTH, SCREEN_HEIGHT, lineColor);

    startY = renderStats(player->score, "SCORE", startY, player->powerups.levelBonusMultiplier, false) + statsBlockGap;
    startY = renderStats(player->levelBonus, "BONUS", startY, player->powerups.levelBonusMultiplier, true) + statsBlockGap;
    startY = renderStats((uint64_t)player->level, "LEVEL", startY, player->powerups.levelBonusMultiplier, false) + statsBlockGap;   
}
