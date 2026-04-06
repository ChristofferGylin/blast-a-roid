#include "raylib.h"
#include "constants.h"
#include "sidebars.h"
#include "player.h"

Color topColor = {0, 25, 38, 255};
Color bottomColor = {0, 13, 36, 255};
Color lineColor = {156, 192, 255, 128};

void renderSidebars(Player* player) {
    
    DrawRectangleGradientV(0, 0, SIDEBAR_WIDTH, SCREEN_HEIGHT, topColor, bottomColor);
    DrawRectangleGradientV(SCREEN_WIDTH - SIDEBAR_WIDTH, 0, SIDEBAR_WIDTH, SCREEN_HEIGHT, topColor, bottomColor);
    DrawLine(SIDEBAR_WIDTH, 0, SIDEBAR_WIDTH, SCREEN_HEIGHT, lineColor);
    DrawLine(SCREEN_WIDTH - SIDEBAR_WIDTH, 0, SCREEN_WIDTH - SIDEBAR_WIDTH, SCREEN_HEIGHT, lineColor);
}
