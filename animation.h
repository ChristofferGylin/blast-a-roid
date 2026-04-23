#ifndef ANIMATION
#define ANIMATION

#include "raylib.h"
#include "stdbool.h"

typedef struct Frame {
    Rectangle rect;
}Frame;

typedef struct Animation {
    Texture2D texture;
    Vector2 position;
    Vector2 size;
    Frame* frames;
    int frameCount;
    int currentFrame;
    double startTime;
    float fps;
    bool isLoop;
    bool isFinished;
}Animation;

void initAnimation(Animation* animation, Texture2D spritesheet, const char* jsonPath, float fps);
void renderAnimation(Animation* animation);
void updateAnimation(Animation* animation);

#endif