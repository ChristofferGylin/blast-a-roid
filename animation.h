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

#endif