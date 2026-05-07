#include "utils.h"
#include "raylib.h"

float scaleFloat(float oldMin, float oldMax, float newMin, float newMax, float value) {
    return (value - oldMin) / (oldMax - oldMin) * (newMax - newMin) + newMin;
}

int getNumberOfAsteroids(int gameLevel) {
    return gameLevel + 2;
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
