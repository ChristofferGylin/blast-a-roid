#include "raylib.h"
#include "constants.h"
#include "bonuses.h"
#include "sidebars.h"
#include "player.h"
#include "utils.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>

Color topColor = {0, 25, 38, 255};
Color bottomColor = {0, 13, 36, 255};
Color lineColor = {156, 192, 255, 128};
Color primaryColor = {0, 218, 255, 255};
Color primaryColorDimmed = {0, 218, 255, 78};

void renderShieldPower(float shieldPower, Vector2 position, Vector2 size) {

    int barLevelWidth = size.x * shieldPower;
    float roundnessRadius = 8.0f;
    int segments = 10;

    Rectangle rect = {
        position.x,
        position.y,
        size.x,
        size.y
    };

    float roundness = getRoundness(rect, roundnessRadius);

    DrawRectangleRounded(rect, roundness, segments, primaryColorDimmed);
    BeginScissorMode(rect.x, rect.y, barLevelWidth, rect.height);
        DrawRectangleRounded(rect, roundness, segments, primaryColor);
    EndScissorMode();
}

RenderPositions renderBlock(char text[], int startY, bool isLeftSide) {
    
    int margin = 4;
    int padding = 6;
    float lineThickness = 2;
    int fontSize = 18;
    int fontSpacing = 8;
    int underlineGap = 2;
    int contentHeight = 20;

    int zeroX = isLeftSide ? 0 : SCREEN_WIDTH - SIDEBAR_WIDTH;

    Vector2 textSize = MeasureTextEx(GetFontDefault(), text, fontSize, fontSpacing);

    Rectangle container = {
        zeroX + margin,
        startY + margin,
        SIDEBAR_WIDTH - (margin * 2),
        (padding * 4) + textSize.y + contentHeight
    };

    Vector2 textOrigin = {textSize.x / 2, 0};
    Vector2 textPosition = {zeroX + (SIDEBAR_WIDTH / 2), startY + lineThickness + padding};

    float roundnessRadius = 8.0f;
    int segments = 10;

    float roundness = getRoundness(container, roundnessRadius);

    Vector2 lineStart = {container.x, textPosition.y + textSize.y + padding};
    Vector2 lineEnd = {container.x + container.width, textPosition.y + textSize.y + padding};

    DrawRectangleRoundedLinesEx(container, roundness, segments, lineThickness, primaryColor);
    DrawLineEx(lineStart, lineEnd, lineThickness, primaryColor);
    DrawTextPro(GetFontDefault(), text, textPosition, textOrigin, 0, fontSize, fontSpacing, primaryColor);
    
    Vector2 contentPosition = {
        container.x + padding,
        lineStart.y + lineThickness + padding
    };

    Vector2 contentSize = {
        container.width - (padding * 2),
        contentHeight
    };

    int endYPosition = container.y + container.height;

    return (RenderPositions){contentPosition, contentSize, endYPosition};
}

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

    RenderPositions shieldPowerPosition = renderBlock("SHIELD", 4, false);
    renderShieldPower(player->shieldPower, shieldPowerPosition.contentPosition, shieldPowerPosition.contentSize);

    startY = renderStats(player->score, "SCORE", startY, player->powerups.levelBonusMultiplier, false) + statsBlockGap;
    startY = renderStats(player->levelBonus, "BONUS", startY, player->powerups.levelBonusMultiplier, true) + statsBlockGap;
    startY = renderStats((uint64_t)player->level, "LEVEL", startY, player->powerups.levelBonusMultiplier, false) + statsBlockGap;
}
