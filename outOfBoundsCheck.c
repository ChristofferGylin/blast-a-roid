#include "raylib.h"

void outOfBoundsCheck(Vector2* position, int size) {
    if (position->x < 0.0f - size)
    {
        position->x = GetScreenWidth() + size;
    } else if (position->x > GetScreenWidth() + size)
    {
        position->x = 0.0f - size; 
    }

    if (position->y < 0.0f - size)
    {
        position->y = GetScreenHeight() + size;
    } else if (position->y > GetScreenHeight() + size)
    {
        position->y = 0.0f - size; 
    }
}