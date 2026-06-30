#ifndef UTILS_H
#define UTILS_H

#include "raylib.h"

typedef struct Rectangle Rectangle;

typedef struct FloatRange {
    float min;
    float max;
}FloatRange;

typedef enum PositionChoice {
    TOP_POS,
    BOTTOM_POS,
    LEFT_POS,
    RIGHT_POS
}PositionChoice;

typedef struct PositionVelocity {
    Vector2 position;
    Vector2 velocity;
}PositionVelocity;

typedef enum VisualType {
    VISUAL_SPRITE,
    VISUAL_ANIMATION
}VisualType;

void applyGForce(Vector2 position, Vector2  destination, Vector2* velocity, float maxVelocity, float minAcceleration, float maxAcceleration, float maxDistance);
float scaleFloat(float oldMin, float oldMax, float newMin, float newMax, float value);
int getNumberOfAsteroids(int gameLevel);
Vector2 getRandomPosition();
Vector2 getRandomPositionOffScreen(int size);
Vector2 getRandomVelocity(FloatRange range);
float getRoundness(Rectangle rect, float radiusPx);
void goToDestination(Vector2 position, Vector2  destination, Vector2* velocity, float maxVelocity, float acceleration);
void knockback(Vector2 targetPosition, Vector2* targetVelocity, Vector2 forcePosition, int force);
void knockbackByImpact(Vector2 targetPosition, Vector2* targetVelocity, Vector2 forcePosition, Vector2 forceVelocity);
void playSoundPositioned(Sound sound, float positionX);
void shake(Vector2* position , double startTime, float duration);
void updatePosition(Vector2* position, Vector2 velocity);
void updateRotation(float* rotation, float rotationSpeed);

#endif