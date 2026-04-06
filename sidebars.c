#include "raylib.h"
#include "constants.h"
#include "sidebars.h"
#include "player.h"
#include <inttypes.h>
#include <stdio.h>

Color topColor = {0, 25, 38, 255};
Color bottomColor = {0, 13, 36, 255};
Color lineColor = {156, 192, 255, 128};

int topOffset = 20;
int titleOffset = 35;
int sideOffset = 20;
int blockOffset = 100;
int fontSize = 22;
int fontSpacing = 4;

void renderSidebars(Player* player) {

    
    char scoreTitle[] = "SCORE";
    char bonusTitle[] = "BONUS";
    char levelTitle[] = "LEVEL";
    char scoreValue[32];
    char bonusValue[32];
    char levelValue[12];

    snprintf(scoreValue, sizeof(scoreValue), "%" PRIu64, player->score);
    snprintf(bonusValue, sizeof(bonusValue), "%" PRIu64, player->levelBonus);
    snprintf(levelValue, sizeof(levelValue), "%03d", player->level);

    Vector2 scoreSize = MeasureTextEx(GetFontDefault(), scoreValue, fontSize, fontSpacing);
    Vector2 scoreTitleSize = MeasureTextEx(GetFontDefault(), scoreTitle, fontSize, fontSpacing);
    float scoreX = (SIDEBAR_WIDTH - sideOffset) - scoreSize.x;

    Vector2 bonusSize = MeasureTextEx(GetFontDefault(), bonusValue, fontSize, fontSpacing);
    float bonusX = (SIDEBAR_WIDTH - sideOffset) - bonusSize.x;

    DrawRectangleGradientV(0, 0, SIDEBAR_WIDTH, SCREEN_HEIGHT, topColor, bottomColor);
    DrawRectangleGradientV(SCREEN_WIDTH - SIDEBAR_WIDTH, 0, SIDEBAR_WIDTH, SCREEN_HEIGHT, topColor, bottomColor);
    DrawLine(SIDEBAR_WIDTH, 0, SIDEBAR_WIDTH, SCREEN_HEIGHT, lineColor);
    DrawLine(SCREEN_WIDTH - SIDEBAR_WIDTH, 0, SCREEN_WIDTH - SIDEBAR_WIDTH, SCREEN_HEIGHT, lineColor);

    int scoreTitleOffset = (SIDEBAR_WIDTH - scoreTitleSize.x) / 2;

    DrawTextPro(GetFontDefault(), scoreTitle, (Vector2){x: SIDEBAR_WIDTH / 2, y: topOffset}, (Vector2){x: scoreTitleSize.x / 2, y: 0}, 0, fontSize, fontSpacing, RAYWHITE);
    DrawLine(scoreTitleOffset, topOffset + scoreTitleSize.y + 2, SIDEBAR_WIDTH - scoreTitleOffset, topOffset + scoreTitleSize.y + 2, RAYWHITE);
    DrawTextPro(GetFontDefault(), scoreValue, (Vector2){x: scoreX, y: topOffset + titleOffset}, (Vector2){x: 0, y: 0}, 0, fontSize, fontSpacing, RAYWHITE);
}
