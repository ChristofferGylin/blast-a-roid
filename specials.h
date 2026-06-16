#ifndef SPECIALS_H
#define SPECIALS_H

#include "raylib.h"
#include "utils.h"
#include "animation.h"

static const int NUMBER_OF_SPECIALS = 4;

typedef enum SpecialType {
    MULTIPLIER,
    COMET,
    EXTRA_LIFE,
    SUPERNOVA
}SpecialType;

typedef struct Special {
    SpecialType type;
    double spawnTime;
    Vector2 position;
    Vector2 velocity;
    Vector2 size;
    float rotation;
    float rotationSpeed;
    float value;
    VisualType visualType;
    union {
        Texture2D* sprite;
        AnimationInstance animation;
    };
}Special;

#endif