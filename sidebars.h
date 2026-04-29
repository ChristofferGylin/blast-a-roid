#ifndef SIDEBARS_H
#define SIDEBARS_H

#include <inttypes.h>
#include "raylib.h"

typedef struct Player Player;

typedef struct RenderPositions {
    Vector2 contentPosition;
    int contentWidth;
    int endYPosition;
}RenderPositions;

RenderPositions renderBlock(char text[], int startY, bool isLeftSide);
void renderShieldPower(float shieldPower, Vector2 position, int width);
void renderSidebars(Player* player);
int renderStats(uint64_t value, char title[], int startY, int bonusMultiplierLevel, bool renderMultiplierIcon);

#endif