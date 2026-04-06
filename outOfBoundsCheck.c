#include "raylib.h"
#include "constants.h"

void outOfBoundsCheck(Vector2* position, int size) {
    if (position->x < SIDEBAR_WIDTH - size / 2)
    {
        position->x = SCREEN_WIDTH - SIDEBAR_WIDTH + size / 2;
    } else if (position->x > SCREEN_WIDTH - SIDEBAR_WIDTH + size / 2)
    {
        position->x = SIDEBAR_WIDTH - size / 2; 
    }

    if (position->y < 0.0f - size / 2)
    {
        position->y = SCREEN_HEIGHT + size / 2;
    } else if (position->y > SCREEN_HEIGHT + size / 2)
    {
        position->y = 0.0f - size / 2; 
    }
}