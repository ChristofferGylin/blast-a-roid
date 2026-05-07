#ifndef UTILS_H
#define UTILS_H

#include "raylib.h"

typedef struct Rectangle Rectangle;

typedef struct FloatRange {
    float min;
    float max;
}FloatRange;

typedef enum VisualType {
    VISUAL_SPRITE,
    VISUAL_ANIMATION
}VisualType;

float scaleFloat(float oldMin, float oldMax, float newMin, float newMax, float value);
int getNumberOfAsteroids(int gameLevel);
Vector2 getRandomVelocity(FloatRange range);
float getRoundness(Rectangle rect, float radiusPx);
void updateRotation(float* rotation, float rotationSpeed);

#endif