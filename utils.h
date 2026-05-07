#ifndef UTILS_H
#define UTILS_H

typedef struct Rectangle Rectangle;

typedef struct FloatRange {
    float min;
    float max;
}FloatRange;

float scaleFloat(float oldMin, float oldMax, float newMin, float newMax, float value);
int getNumberOfAsteroids(int gameLevel);
Vector2 getRandomVelocity(FloatRange range);
float getRoundness(Rectangle rect, float radiusPx);

#endif