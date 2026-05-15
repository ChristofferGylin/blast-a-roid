#include "utils.h"
#include "raylib.h"
#include "math.h"
#include "constants.h"
#include "stdio.h"

float scaleFloat(float oldMin, float oldMax, float newMin, float newMax, float value) {
    return (value - oldMin) / (oldMax - oldMin) * (newMax - newMin) + newMin;
}

int getNumberOfAsteroids(int gameLevel) {
    return gameLevel + 2;
}

Vector2 getRandomPosition() {
    return (Vector2){GetRandomValue(SIDEBAR_WIDTH + 10, SCREEN_WIDTH - 10 - SIDEBAR_WIDTH), GetRandomValue(10, SCREEN_HEIGHT - 10)};
}

Vector2 getRandomPositionOffScreen(int size) {

    PositionChoice pos = GetRandomValue(0,3);

    const int MIN_X = SIDEBAR_WIDTH;
    const int MAX_X = SCREEN_WIDTH - SIDEBAR_WIDTH;
    const int MIN_Y = 0;
    const int MAX_Y = SCREEN_HEIGHT;

    int x = 0;
    int y = 0;

    switch (pos)
    {
    case TOP_POS:
        y = -((size / 2) - 1);
        x = GetRandomValue(MIN_X, MAX_X);
        break;

    case BOTTOM_POS:
        y = MAX_Y + ((size / 2) - 1);
        x = GetRandomValue(MIN_X, MAX_X);
        break;

    case LEFT_POS:
        y = GetRandomValue(MIN_Y, MAX_Y);
        x = -((size / 2) - 1);
        break;
    
    case RIGHT_POS:
        y = GetRandomValue(MIN_Y, MAX_Y);
        x = MAX_X + ((size / 2) - 1);
        break;
    
    default:
        printf("Error: Invalid PosChoice in getRandomPositionOffScreen");
        break;
    }

    return (Vector2){x, y};
}

Vector2 getRandomVelocity(FloatRange range) {
    bool directionX = GetRandomValue(0,1);
    bool directionY = GetRandomValue(0,1);
    int tempVelocityX = GetRandomValue(range.min, range.max);
    int tempVelocityY = GetRandomValue(range.min, range.max);
    
    return (Vector2){directionX ? tempVelocityX : -tempVelocityX, directionY ? tempVelocityY : -tempVelocityY};
}

float getRoundness(Rectangle rect, float radiusPx) {
    float minDim = rect.width < rect.height ? rect.width : rect.height;
    return (radiusPx * 2.0f) / minDim;
}

void updatePosition(Vector2* position, Vector2 velocity) {
    position->x += GetFrameTime() * velocity.x;
    position->y += GetFrameTime() * velocity.y;
}

void updateRotation(float* rotation, float rotationSpeed) {
        *rotation += GetFrameTime() * rotationSpeed;
        *rotation = fmodf(*rotation, 360.0f);

        if (*rotation < 0.0f)
        {
            *rotation += 360.0f;
        }
}