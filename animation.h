#ifndef ANIMATION
#define ANIMATION

#include "raylib.h"

typedef struct Frame {
    Rectangle rect;
}Frame;

typedef struct Animation {
    Texture2D texture;
    Frame* frames;
    int frameCount;
    int currentFrame;
    double startTime;
    float fps;
}Animation;

void initAnimation(Animation* animation, Texture2D spritesheet, const char* jsonPath, float fps);

#endif